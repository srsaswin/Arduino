#include <WiFi.h> 
#include <ThingESP.h>

ThingESP32 thing("srsaswin", "demo0002", "demo0002");

const int medicalPin = 19; // GPIO pin for medical emergency
const int firePin = 18;    // GPIO pin for fire emergency

volatile bool medicalEmergency = false;
volatile bool fireEmergency = false;

// Declare ISR functions
void IRAM_ATTR medicalISR();
void IRAM_ATTR fireISR();
 int h = 100;
void setup() {
  Serial.begin(115200);
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
  
  String count = "count: " + String(h) + "  \n";
  if (query == "count") {
    return count;
  } else {
    return "Your query was invalid.";
  }
}

// Define ISR functions
void IRAM_ATTR medicalISR() {
  medicalEmergency = true;
}

void IRAM_ATTR fireISR() {
  fireEmergency = true;
}

void handleEmergency() {
  if (medicalEmergency) {
    thing.sendMsg("+919025806629", "Medical Emergency \ncount : "+String(h) );
   // thing.sendMsg("+919025806629", );
    medicalEmergency = false; // Reset the flag
  }
  
  if (fireEmergency) {
    Serial.println("hhi");
    thing.sendMsg("+919025806629", "Fire Emergency\ncount : "+String(h));
   // thing.sendMsg("+919025806629", "count : "+String(h));
    fireEmergency = false; // Reset the flag
  }
}

void loop() { 
  thing.Handle();
 handleEmergency(); // Handle any emergency messages
}