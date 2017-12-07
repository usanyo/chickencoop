#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Button.h>
#include <index.html.h>

#define DAY   0
#define NIGHT 1

int dayPhase = 0;
int dayLight = 500;
int nightLight = 1023;
int currentLight = 0;
bool nothingPressed = true;

MDNSResponder mdns;
ESP8266WebServer server(80);
Button upButton = Button(D7, LOW);
Button downButton = Button(D8, LOW);
//ETSTimer stoppingTimer;

#include <credentials.h>
// The content of this header is the following:
// const char* ssid = "your_ssid";
// const char* password = "your_passoword";


void openDoor();
void closeDoor();
void moveUp();
void moveDown();
void stopMoving();
String printDoorStatus();

String printDoorStatus() {
  if (digitalRead(D5) == HIGH)
    return "moving up";
  if (digitalRead(D6) == HIGH)
    return "moving down";
  return "stop moving";
}

String printDayPhase() {
  return dayPhase == DAY ? "day" : "night";
}

void respondWebPage() {
    server.send(200, "text/html", index_html);
}

void setup(void){

  upButton.setup();
  downButton.setup();

  // preparing GPIOs
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  digitalWrite(D5, 0);
  digitalWrite(D6, 0);
  
  delay(1000);
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  Serial.println("");

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

  server.on("/", respondWebPage);
  server.on("/up", [](){
    moveUp();
    nothingPressed = false;
    server.send(200, "text/raw", "up");
  });
  server.on("/down", [](){
    moveDown();
    server.send(200, "text/raw", "down");
  });
  server.on("/stop", [](){
    stopMoving();
    server.send(200, "text/raw", "stopped");
    nothingPressed = true;
  });

  server.on("/setDay", []() {
    respondWebPage();
    dayLight = currentLight;
  });
  
  server.on("/setNight", []() {
    respondWebPage();
    nightLight = currentLight;
  });

  server.on("/open", []() {
    openDoor();
    server.send(200, "text/raw", "open");
  });
  
  server.on("/close", []() {
    closeDoor();
    server.send(200, "text/raw", "closed");
  });
  
  server.begin();
  Serial.println("HTTP server started");

  //os_timer_disarm(&stoppingTimer);
  //os_timer_setfn(&stoppingTimer, callback, nullptr);
}

void openDoor() {
  nothingPressed = false;
  moveUp();
  //os_timer_arm(&stoppingTimer, 1000, 0);
  nothingPressed = true;
}

void closeDoor() {
  nothingPressed = false;
  moveDown();
//  os_timer_arm(&stoppingTimer, 1000, 0);
  nothingPressed = true;
}

void loop(void){
  /*if(digitalRead(D7) == LOW) {
    moveUp();
  }
  else if (digitalRead(D8) == LOW) {
    moveDown();
  } else if(nothingPressed) {
    stopMoving();
  }

  currentLight = analogRead(A0);
  if((currentLight < dayLight) && dayPhase == NIGHT) {
    openDoor();
    dayPhase = DAY;
  }
  if((currentLight > nightLight) && dayPhase == DAY) {
    closeDoor();
    dayPhase = NIGHT;
  }
  delay(500);
  server.handleClient();*/
}

void moveUp() {
    digitalWrite(D5, HIGH);
    digitalWrite(D6, LOW);  
}

void moveDown() {
    digitalWrite(D5, LOW);
    digitalWrite(D6, HIGH);  
}

void stopMoving() {
    digitalWrite(D5, LOW);
    digitalWrite(D6, LOW);  
}