#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#define DAY   0
#define NIGHT 1

int dayPhase = 0;
int dayLight = 0;
int nightLight = 1023;
int currentLight = 0;
bool nothingPressed = true;

MDNSResponder mdns;

#include <credentials.h>
// The content of this header is the following:
// const char* ssid = "your_ssid";
// const char* password = "your_passoword";

ESP8266WebServer server(80);

void openDoor();
void closeDoor();

String printDayPhase() {
  return dayPhase == DAY ? "day" : "night";
}

String buildWebPage() {
  return
  "<p> " + printDayPhase() + 
    "<a href=\"open\"><button>OPEN</button></a>"
    "&nbsp;"
    "<a href=\"close\"><button>CLOSE</button></a>"
    "&nbsp;"
    "<a href=\"up\"><button>UP</button></a>"
    "&nbsp;"
    "<a href=\"down\"><button>DOWN</button></a>"
    "&nbsp;"
    "<a href=\"stop\"><button>STOP</button></a>"
  "</p>" + String(analogRead(A0));
}

void respondWebPage() {
    server.send(200, "text/html", buildWebPage());
}

void setup(void){
  
  // preparing GPIOs
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  pinMode(D7, INPUT);
  pinMode(D8, INPUT);
  digitalWrite(D5, 0);
  digitalWrite(D6, 0);
  
  delay(1000);
  Serial.begin(115200);
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
  
  if (mdns.begin("esp8266", WiFi.localIP())) {
    Serial.println("MDNS responder started");
  }
  
  server.on("/", respondWebPage);
  server.on("/up", [](){
    respondWebPage();
    digitalWrite(D5, HIGH);
    digitalWrite(D6, LOW);
    nothingPressed = false;
    delay(1000); 
  });
  server.on("/down", [](){
    respondWebPage();
    digitalWrite(D5, LOW);
    digitalWrite(D6, HIGH);
    delay(1000); 
  });
  server.on("/stop", [](){
    respondWebPage();
    digitalWrite(D5, LOW);
    digitalWrite(D6, LOW);
    delay(1000); 
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
    respondWebPage();
    openDoor();
  });
  
  server.on("/close", []() {
    respondWebPage();
    closeDoor();
  });
  
  server.begin();
  Serial.println("HTTP server started");
}

void openDoor() {
  nothingPressed = false;
    digitalWrite(D5, HIGH);
    digitalWrite(D6, LOW);
    delay(22*1000);
    digitalWrite(D5, LOW);
    digitalWrite(D6, LOW);
    nothingPressed = true;
}

void closeDoor() {
  nothingPressed = false;
    digitalWrite(D5, LOW);
    digitalWrite(D6, HIGH);
    delay(13*1000);
    digitalWrite(D5, LOW);
    digitalWrite(D6, LOW);
    nothingPressed = true;
}

void loop(void){
  if(digitalRead(D7) == LOW) {
    digitalWrite(D5, HIGH);
    digitalWrite(D6, LOW);
  }
  else if (digitalRead(D8) == LOW) {
    digitalWrite(D5, LOW);
    digitalWrite(D6, HIGH);
  } else if(nothingPressed) {
    digitalWrite(D5, LOW);
    digitalWrite(D6, LOW);
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
  server.handleClient();
} 