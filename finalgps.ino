#include <ESP8266WiFi.h>
#include <ThingESP.h>
#include <SoftwareSerial.h>

 
ThingESP8266 thing("srsaswin", "demo0002", "demo0002");

 
const int medicalPin = 0;  
const int firePin = 2;    

 
volatile bool medicalEmergency = false;
volatile bool fireEmergency = false;

 
SoftwareSerial ss(4, 5);  

 
String gpsLocation = "";
 
void ICACHE_RAM_ATTR medicalISR();
void ICACHE_RAM_ATTR fireISR();

void setup() {
  Serial.begin(9600);
  ss.begin(9600);  
  thing.SetWiFi("hi", "12345678");
  thing.initDevice();

 
  pinMode(medicalPin, INPUT_PULLUP);
  pinMode(firePin, INPUT_PULLUP);

 
  attachInterrupt(digitalPinToInterrupt(medicalPin), medicalISR, FALLING);
  attachInterrupt(digitalPinToInterrupt(firePin), fireISR, FALLING);
}

String HandleResponse(String query) {
  int h = 0;
  String count = "count: " + String(h) + "  \n";
  if (query == "count") {
    return count;
  } else {
    return "Your query was invalid.";
  }
}

// Define ISR functions
void ICACHE_RAM_ATTR medicalISR() {
  medicalEmergency = true;
}

void ICACHE_RAM_ATTR fireISR() {
  fireEmergency = true;
}

void handleEmergency() {
  int h = 0;
  if (medicalEmergency) {
    Serial.println("Medical Emergency detected!");
    thing.sendMsg("+919025806629", "Medical Emergency  \nLocation: " + gpsLocation);
    medicalEmergency = false;  
  }

  if (fireEmergency) {
    Serial.println("Fire Emergency detected!");
    thing.sendMsg("+919025806629", "Fire Emergency \nLocation: " + gpsLocation);
    fireEmergency = false;  
  }
}

void readGPSData() {
  while (ss.available() > 0) {
    char c = ss.read();
    gpsLocation += c; 
    if (c == '\n') {
      if (gpsLocation.startsWith("$GPGGA")) {
 
        int latIndex = gpsLocation.indexOf(',', 17); 
        int lonIndex = gpsLocation.indexOf(',', latIndex + 1);  

        String latitude = gpsLocation.substring(latIndex + 1, latIndex + 10); // Extract latitude
        String longitude = gpsLocation.substring(lonIndex + 1, lonIndex + 11);  

        gpsLocation = "Lat: " + latitude + ", Lon: " + longitude;
        Serial.println("GPS Location: " + gpsLocation);
      }
      gpsLocation = ""; 
    }
  }
}

void loop() {
  thing.Handle();
  handleEmergency();  
  readGPSData();   
}
