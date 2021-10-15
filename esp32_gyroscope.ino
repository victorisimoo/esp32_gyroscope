// Libraries required for the project
#include <WiFi.h>
#include <ArduinoJson.h>
#include <FirebaseESP32.h>
#include <MPU9250_asukiaaa.h>
#include <ezTime.h>

// Definition ot internet connection variables
#define wifi_network "victortilla"
#define wifi_password "victor123"
#define wifi_timeout_ms 20000

//firebase connection constants
#define firebase_host "https://esp32-lab02-default-rtdb.firebaseio.com/"
#define firebase_auth "q2sba6hwjcUfKjzU0kNmpC1rHT24txOcwmikfFzF"

//firebase data object
FirebaseData firebaseData;
StaticJsonDocument<200> doc;
JsonArray  root = doc.to<JsonArray>();

//variable para iniciar
bool state = false;

//time object definition
Timezone myTZ;

//Firebase ubication
String nodo = "/DataSensor";

// Definition of the connection between the ESP32 module and the MPU9250
#ifdef _ESP32_HAL_I2C_H_
#define SDA_PIN 21
#define SCL_PIN 22
#endif

// Variables required for the MPU9250 module
MPU9250_asukiaaa mySensor;
// variables de estado actual
float actual_gx, actual_gy, actual_gz, actual_mDirection, actual_mx, actual_my, actual_mz;
// lectura actual
float lecture_gx, lecture_gy, lecture_gz, lecture_mDirection, lecture_mx, lecture_my, lecture_mz;

// Pin number configuration
const int pinledwifi = 15;
const int pinpower = 2;

//method for pin configure
void pinConfigure(){
  pinMode(pinpower, INPUT);
  pinMode(pinledwifi, OUTPUT);
}

// constant coordinate reading method
void readUbicationXYZ(){
  uint8_t sensorId;
  if(mySensor.readId(&sensorId) == 0){
    Serial.println("sensor id: "+String(sensorId));
  }else {
    Serial.println("Cannot read sensorId");
  }

  if (mySensor.magUpdate() == 0) {
    lecture_mx = mySensor.magX();
    lecture_my = mySensor.magY();
    lecture_mz = mySensor.magZ();
    lecture_mDirection = mySensor.magHorizDirection();
  } else {
    Serial.println("Cannot read mag values");
  }
}

// constant coordinate reading method
void readUbicationGyroscope(){
  uint8_t sensorId;
  if(mySensor.readId(&sensorId) == 0){
    Serial.println("sensor id: "+String(sensorId));
  }else {
    Serial.println("Cannot read sensorId");
  }

  if (mySensor.gyroUpdate() == 0) {
    lecture_gx = mySensor.gyroX();
    lecture_gy = mySensor.gyroY();
    lecture_gz = mySensor.gyroZ();
  } else {
    Serial.println("Cannot read gyro values");
  }
}

// constant coordinate initial reading method
void initialUbicationXYZ(){
  readUbicationXYZ();
  readUbicationGyroscope();
  actual_mx = lecture_mx;
  actual_my = lecture_my;
  actual_mz = lecture_mz;
  actual_mDirection = lecture_mDirection;
  actual_gx = lecture_gx;
  actual_gy = lecture_gy;
  actual_gz = lecture_gz;

  Firebase.pushString(firebaseData, nodo + "/Initial", "x: " + String(actual_mx));
  Firebase.pushString(firebaseData, nodo + "/Initial", "y: " + String(actual_my));
  Firebase.pushString(firebaseData, nodo + "/Initial", "z: " + String(actual_mz));
  Firebase.pushString(firebaseData, nodo + "/Initial", "h: " + String(actual_mDirection));
  Firebase.pushString(firebaseData, nodo + "/Initial", "t: " +myTZ.dateTime());
}

//method for initial sensor configuration
void sensorInitialConfigure(){
  #ifdef _ESP32_HAL_I2C_H_ // For ESP32
    Wire.begin(SDA_PIN, SCL_PIN);
    mySensor.setWire(&Wire);
  #endif

  mySensor.beginAccel();
  mySensor.beginGyro();
  mySensor.beginMag();
}

//time configuration
void timeConfigure(){
  waitForSync();
  myTZ.setLocation(F("gt"));
}

void changeState(){
  if(!state){
    initialUbicationXYZ();
    state = true;
  }
}

//method for connection to wifi
void connectionToWifi(){
  Serial.print("Connecting to Wifi: ");
  Serial.print(wifi_network);
  WiFi.mode(WIFI_STA);

  //starting the connection to firebase
  WiFi.begin(wifi_network, wifi_password);
  unsigned long startAttempTime = millis();

  while(WiFi.status() != WL_CONNECTED && millis() - startAttempTime < wifi_timeout_ms){
    Serial.print(".");
    delay(100);
  }

  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Failed connection!");
  }else {
    Serial.println(" ");
    Serial.print("Connected -----> IP: ");
    Serial.print(WiFi.localIP());
    Serial.println("");
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
  sensorInitialConfigure();
  connectionToWifi();
  connectionToFirebase();
  timeConfigure();
}

void loop() {
  if(digitalRead(pinpower) == HIGH){
    changeState();
  }
}
