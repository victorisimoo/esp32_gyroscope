// Libraries required for the project
#include <WiFi.h>
#include <FirebaseESP32.h>
#include <MPU9250_asukiaaa.h>

// Definition ot internet connection variables
#define wifi_network = "victortilla";
#define wifi_password = "victor123";
#define wifi_timeout_ms = 20000;

//firebase connection constants
#define firebase_host "https://victorbox-cf2b6.firebaseapp.com/"
#define firebase_auth "IYvv0SRtHDexkaRyENc8wPwvc102CTj8FO73tFkd"

//firebase data object
FirebaseData firebaseData;

// Definition of the connection between the ESP32 module and the MPU9250
#ifdef _ESP32_HAL_I2C_H_
#define SDA_PIN 21
#define SCL_PIN 22
#endif

// Variables required for the MPU9250 module
MPU9250_asukiaaa mySensor;

// global variables to return ubication
float gX, gY, gZ, mDirection, mX, mY, mZ;

// Pin number configuration
const int pinledwifi = 15;
const int pinpower = 2;

//method for pin configure
void pinConfigure(){
  pinMode(pinpower, INPUT);
  pinMode(pinledwifi, OUTPUT);
}

//method for internet connection
void connectToWiFi(){
  Serial.print("Connecting to WiFi: ");
  Serial.print(wifi_network);
  WiFi.mode(WIFI_STA);
  WiFi.begin(wifi_network, wifi_password);
  unsigned log startAttemptTime = millis();

  while(WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < wifi_timeout_ms){
    Serial.print(".");
    delay(100);
  }

  if(WiFi.status() != WL_CONNECTED){
    Serial.println("Failed!");
  }else {
    Serial.println("");
    Serial.print("Connected! ---> IP: ");
    Serial.println(WiFi.localIP());
    digitalWrite(pinledwifi, HIGH); 
  }
}

//method for connection to firebase
void connectionToFirebase(){  
  Firebase.begin(firebase_host, firebase_auth),
  Firebase.reconnectWiFi(true);
}

void setup() {
  Serial.begin(115200);
  pinConfigure();
  connectToWiFi();
  connectionToFirebase();
}

void loop() {
  while(digitalRead(pinpower) == HIGH){
    delay(250);
  }
}
