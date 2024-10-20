#include <ESP8266WiFi.h>
#include <ThingESP.h>
#include <SoftwareSerial.h>

// Initialize ThingESP
ThingESP8266 thing("srsaswin", "demo0002", "demo0002");

// Pins for emergency buttons
const int medicalPin = 0; // GPIO pin for medical emergency (D3 on NodeMCU)
const int firePin = 2;    // GPIO pin for fire emergency (D4 on NodeMCU)

// Flags for emergencies
volatile bool medicalEmergency = false;
volatile bool fireEmergency = false;

// GPS module connections
SoftwareSerial ss(4, 5); // RX, TX for GPS module

// GPS data variables
String gpsLocation = "";

// Declare ISR functions
void ICACHE_RAM_ATTR medicalISR();
void ICACHE_RAM_ATTR fireISR();

void setup() {
  Serial.begin(9600);
  ss.begin(9600); // GPS module baud rate
  thing.SetWiFi("hi", "12345678");
  thing.initDevice();

  // Configure the pins as input with internal pull-up resistors
  pinMode(medicalPin, INPUT_PULLUP);
  pinMode(firePin, INPUT_PULLUP);

  // Attach interrupts to the pins
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
    thing.sendMsg("+919025806629", "Medical Emergency \ncount : " + String(h) + "\nLocation: " + gpsLocation);
    medicalEmergency = false; // Reset the flag
  }

  if (fireEmergency) {
    Serial.println("Fire Emergency detected!");
    thing.sendMsg("+919025806629", "Fire Emergency\ncount : " + String(h) + "\nLocation: " + gpsLocation);
    fireEmergency = false; // Reset the flag
  }
}

void readGPSData() {
  while (ss.available() > 0) {
    char c = ss.read();
    gpsLocation += c;
    // Parse GPS data (assuming NMEA format)
    if (c == '\n') {
      if (gpsLocation.startsWith("$GPGGA")) {
        // Extract latitude and longitude from the GPGGA sentence
        int latIndex = gpsLocation.indexOf(',', 17); // Latitude starts after the 17th comma
        int lonIndex = gpsLocation.indexOf(',', latIndex + 1); // Longitude starts after the next comma

        String latitude = gpsLocation.substring(latIndex + 1, latIndex + 10); // Extract latitude
        String longitude = gpsLocation.substring(lonIndex + 1, lonIndex + 11); // Extract longitude

        gpsLocation = "Lat: " + latitude + ", Lon: " + longitude;
        Serial.println("GPS Location: " + gpsLocation);
      }
      gpsLocation = ""; // Clear for next sentence
    }
  }
}

void loop() {
  thing.Handle();
  handleEmergency(); // Handle any emergency messages
  readGPSData(); // Continuously read GPS data
}
