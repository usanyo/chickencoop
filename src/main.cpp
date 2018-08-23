#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <EEPROM.h>
#include <ArduinoJson.h>
#include <time.h>

#define DAY   0
#define NIGHT 1

bool doorIsOpen = true;

int openingTime = 5 * 60 + 30;
int closingTime = 21 * 60 + 00;
int openingDuration = 18 * 256;
int closingDuration = 13 * 256;

MDNSResponder mdns;
ESP8266WebServer server(80);
ETSTimer stoppingTimer;

#include <credentials.h>
// The content of this header is the following:
// const char* ssid = "your_ssid";
// const char* password = "your_password";


void openDoor();
void closeDoor();
void moveUp();
void moveDown();
void stopMoving();

int timeToInt(const char* input) {
  char h[3];
  h[0] = input[0];
  h[1] = input[1];
  h[2] = '\0';
  char m[3];
  m[0] = input[3];
  m[1] = input[4];
  m[2] = '\0';
  int hours = atoi(h);
  int minutes = atoi(m);
  return hours * 60 + minutes;
}

void intToTime(int input, char* output) {
  int hours = input / 60;
  int minutes = input % 60;
  char h[3];
  char m[3];
  sprintf(h, "%02d", hours);
  sprintf(m, "%02d", minutes);
  output[0] = h[0];
  output[1] = h[1];
  output[2] = ':';
  output[3] = m[0];
  output[4] = m[1];
  output[5] = '\0';
}

void EEPROM_write(int baseAddress, int value) {
  unsigned char* bits = static_cast<unsigned char*>(static_cast<void*>(&value));
  for(unsigned int i = 0; i < sizeof(int); i++)
    EEPROM.write(baseAddress*4 + i, bits[i]);
}

int EEPROM_read(int baseAddress) {
  unsigned char bits[4];
  for(unsigned int i = 0; i < sizeof(int); i++)
    bits[i] = EEPROM.read(baseAddress*4 + i);
  int* value = static_cast<int*>(static_cast<void*>(bits));
  return *value;
}

void setup(void){

  // preparing GPIOs
  pinMode(D2, OUTPUT);
  pinMode(D4, OUTPUT);
  
  stopMoving();
  
  delay(1000);
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  Serial.println("");

  EEPROM.begin(512);

  openingTime = EEPROM_read(0);
  closingTime = EEPROM_read(1);
  openingDuration = EEPROM_read(2);
  closingDuration = EEPROM_read(3);

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  configTime(2 * 3600, 0, "pool.ntp.org", "time.nist.gov");

  if (mdns.begin("esp8266", WiFi.localIP())) {
    Serial.println("MDNS responder started");
  }

  server.on("/connection", []() {
    time_t now = time(nullptr);
    Serial.println(ctime(&now));
    Serial.println("Connection");
    server.sendHeader("Access-Control-Allow-Origin", "*");
    char res[1024];
    char openingTimeStr[6];
    char closingTimeStr[6];
    intToTime(openingTime,openingTimeStr);
    intToTime(closingTime,closingTimeStr);
    sprintf(res, "{ \"status\" : \"connected\",\n\"openingTime\": \"%s\",\n\"closingTime\": \"%s\",\n\"openingDuration\": %.2f,\n\"closingDuration\": %.2f}",
    openingTimeStr, closingTimeStr, openingDuration/256.0, closingDuration/256.0);
    Serial.println(res);
    server.send(200, "application/json", res);
  });

  server.on("/settings", []() {
    Serial.printf("before: %d, %d, %d, %d\n\r", openingTime, closingTime, openingDuration, closingDuration);
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.sendHeader("Access-Control-Allow-Methods", "GET,PUT,POST,DELETE,OPTIONS");
    server.sendHeader("Access-Control-Allow-Headers", "Content-Type, Authorization, Content-Length, X-Requested-With, Accept");

    if(server.hasArg("plain")) {
      Serial.println(server.arg("plain"));
      StaticJsonBuffer<200> jsonBuffer;
      JsonObject& receivedData = jsonBuffer.parseObject(server.arg("plain"));
      openingTime = timeToInt(receivedData["openingTime"]);
      closingTime = timeToInt(receivedData["closingTime"]);
      Serial.printf("request: %s; %s;\n%.2f; %.2f\n", (const char*)receivedData["openingTime"], (const char*)receivedData["closingTime"],(double)receivedData["openingDuration"],(double)receivedData["closingDuration"]);
      openingDuration = (int)((float)receivedData["openingDuration"]*256.0f + 0.5f);
      closingDuration = (int)((float)receivedData["closingDuration"]*256.0f + 0.5f);

      EEPROM_write(0,openingTime);
      EEPROM_write(1,closingTime);
      EEPROM_write(2,openingDuration);
      EEPROM_write(3,closingDuration);
      EEPROM.commit();
      
      Serial.printf("after: %d, %d, %d, %d\n\r", openingTime, closingTime, openingDuration, closingDuration);

      char res[1024];
      char openingTimeStr[6];
      char closingTimeStr[6];
      intToTime(openingTime,openingTimeStr);
      intToTime(closingTime,closingTimeStr);
      sprintf(res, "{ \"status\" : \"success\",\n\"openingTime\": \"%s\",\n\"closingTime\": \"%s\",\n\"openingDuration\": %.2f,\n\"closingDuration\": %.2f}",
      openingTimeStr, closingTimeStr, openingDuration/256.0, closingDuration/256.0);
      Serial.println(res);
      server.send(200, "application/json", res);
    }
    else
      server.send(200, "application/json", R"({"status"} : "error")");
  });


  server.on("/up", [](){
    server.sendHeader("Access-Control-Allow-Origin", "*");
    moveUp();
    server.send(200, "text/raw", "up");
  });
  server.on("/down", [](){
    server.sendHeader("Access-Control-Allow-Origin", "*");
    moveDown();
    server.send(200, "text/raw", "down");
  });
  server.on("/stop", [](){
    server.sendHeader("Access-Control-Allow-Origin", "*");
    stopMoving();
    server.send(200, "text/raw", "stopped");
  });

  server.on("/open", []() {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    openDoor();
    server.send(200, "text/raw", "open");
  });
  
  server.on("/close", []() {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    closeDoor();
    server.send(200, "text/raw", "closed");
  });
  
  server.begin();
  Serial.println("HTTP server started");

  os_timer_disarm(&stoppingTimer);
  os_timer_setfn(&stoppingTimer, [](void* arg) {
    stopMoving();
  }, nullptr);
}

void openDoor() {
  Serial.println("opening");
  moveUp();
  os_timer_arm(&stoppingTimer, openingDuration*1000/256, 0);
  server.send(200, "text/raw", "open");
}

void closeDoor() {
  Serial.println("closing");
  moveDown();
  os_timer_arm(&stoppingTimer, closingDuration*1000/256, 0);
  server.send(200, "text/raw", "closed");
}

void loop(void){
  delay(500);
  
  server.handleClient();

  time_t now = time(nullptr);
  struct tm *tm_struct = localtime(&now);
  int hours = tm_struct->tm_hour;
  int minutes = tm_struct->tm_min;

  if(closingTime <= hours*60 + minutes) {
    if(doorIsOpen) {
      closeDoor();
      doorIsOpen = false;
    }
  }
  else if(openingTime <= hours*60 + minutes) {
    if(!doorIsOpen) {
      openDoor();
      doorIsOpen = true;
    }
  }
}

void moveUp() {
  digitalWrite(D2, HIGH);
  digitalWrite(D4, LOW);  
}

void moveDown() {
  digitalWrite(D2, LOW);
  digitalWrite(D4, HIGH);  
}

void stopMoving() {
  digitalWrite(D2, LOW);
  digitalWrite(D4, LOW);  
}
