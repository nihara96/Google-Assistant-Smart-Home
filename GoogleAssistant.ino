/*Author : Nihara Mayurawasala 
 Google Assistant home automation system 
 ADAFRUIT/Google */
 
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include<LiquidCrystal_I2C.h>
#include<Wire.h>

//Pin connected to ST_CP of 74HC595
#define latchPin          D0
//Pin connected to SH_CP of 74HC595
#define clockPin          D1
//Pin connected to DS of 74HC595
#define dataPin           D2

#define LCD_SDA_pin       D3
#define LCD_SCL_pin       D4

#define enableLine        D5
#define wlanLed           D6
int arrayLength = 8; 

int dataArray[8];

char WLAN_SSID[] = "OHB02313";  
char WLAN_PASS[]= ""; 

/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    "nihara"            // Replace it with your username
#define AIO_KEY         "YOUR_API_KEY"   // Replace with your Project Auth Key

/************ Global State (you don't need to change this!) ******************/

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;
// or... use WiFiFlientSecure for SSL
//WiFiClientSecure client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
LiquidCrystal_I2C lcd(0x27,16,2);
/****************************** Feeds ***************************************/


// Setup a feed called 'onoff' for subscribing to changes.
Adafruit_MQTT_Subscribe output_1 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME"/feeds/Relay_1");
Adafruit_MQTT_Subscribe output_2 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME"/feeds/Relay_2");// FeedName
Adafruit_MQTT_Subscribe output_3 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME"/feeds/Relay_3");
Adafruit_MQTT_Subscribe output_4 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME"/feeds/Relay_4");
Adafruit_MQTT_Subscribe output_5 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME"/feeds/Relay_5");
Adafruit_MQTT_Subscribe output_6 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME"/feeds/Relay_6");
Adafruit_MQTT_Subscribe output_7 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME"/feeds/Relay_7");
Adafruit_MQTT_Subscribe output_8 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME"/feeds/Relay_8");

//Adafruit_MQTT_Publish statusMonitor = Adafruit_MQTT_Publish(&mqtt,AIO_USERNAME"/feeds/Relay_2_isON");

void MQTT_connect();
void writereg(); 

void setup() {
  Serial.begin(115200);
  Wire.begin(LCD_SDA_pin,LCD_SCL_pin);
  lcd.begin(); 
  lcd.setCursor(6,0);
 // Serial.println("Xsoft Smart Home");
  //lcd.print("Xsoft");
  lcd.setCursor(3,0);
  lcd.print("Smart Home");
delay(2000);
lcd.clear(); 
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(enableLine,OUTPUT);
  pinMode(wlanLed,OUTPUT);
digitalWrite(enableLine,HIGH); 
   for(int i=0;i<8;i++){
    
    dataArray[i]=1;   // put all values to HIGH 
    
    }
    writereg();
    Serial.println("Register Cleared !");
   digitalWrite(enableLine,LOW); 
   
  // Connect to WiFi access point.
  lcd.setCursor(0,0);
  Serial.print("Connecting to ");
  lcd.print("Connecting to ");
  lcd.setCursor(0,1);
  Serial.print(WLAN_SSID);
  lcd.print(WLAN_SSID);
  delay(1000); 
  lcd.clear(); 
 
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    lcd.print("."); 
    digitalWrite(wlanLed,HIGH);
     delay(500);
     digitalWrite(wlanLed,LOW);
     delay(500); 
  }
  lcd.clear(); 
  digitalWrite(wlanLed,HIGH);
  Serial.println("WiFi connected");
  lcd.print("WiFi connected");
  
  delay(1000);
  lcd.clear();
  lcd.print("IP address");
  Serial.println("IP address: "); 
  lcd.setCursor(0,1);
  lcd.print(WiFi.localIP());
  Serial.println(WiFi.localIP());
  delay(2000);
  lcd.clear(); 

  // Setup MQTT subscription for onoff feed.
  mqtt.subscribe(&output_1);
  mqtt.subscribe(&output_2);
  mqtt.subscribe(&output_3);
  mqtt.subscribe(&output_4);
  mqtt.subscribe(&output_5);
  mqtt.subscribe(&output_6);
  mqtt.subscribe(&output_7);
  mqtt.subscribe(&output_8);
  
}


void loop() {
 
  MQTT_connect();
  
  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(20000))) {

    // Relays are active LOW
    if (subscription == &output_1) {
      lcd.clear();
      lcd.print(F("Com 1: "));
      lcd.print((char *)output_1.lastread); 
      dataArray[0] = !atoi((char *)output_1.lastread);
      Serial.println("Output 1:");
      Serial.print((char *)output_1.lastread);
       //  statusMonitor.publish(atoi((char *)output_1.lastread));
      
  
    
      
    } if (subscription == &output_2) {  
      lcd.clear();
      lcd.print(F("Com 2: "));
      lcd.print((char *)output_2.lastread);
      dataArray[1] = !atoi((char *)output_2.lastread);
      Serial.println("Output 2:");
      Serial.print((char *)output_2.lastread);
     //  statusMonitor.publish(atoi((char *)output_2.lastread));

      
    } if (subscription == &output_3) { 
      lcd.clear();
      lcd.print(F("Com 3: "));
      lcd.print((char *)output_3.lastread);
      dataArray[2] = !atoi((char *)output_3.lastread);
      Serial.println("Output 3:");
      Serial.print((char *)output_3.lastread);
     //  statusMonitor.publish(atoi((char *)output_3.lastread));
 
      
    } if (subscription == &output_4) { 
       lcd.clear();
      lcd.print(F("Com 4: "));
      lcd.print((char *)output_4.lastread);
      dataArray[3] = !atoi((char *)output_4.lastread);
      Serial.println("Output 4:");
      Serial.print((char *)output_4.lastread);
      // statusMonitor.publish(atoi((char *)output_4.lastread));
   
      
    } if (subscription == &output_5) { 
      lcd.clear();
      lcd.print(F("Com 5: "));
      lcd.print((char *)output_5.lastread);
      dataArray[4] = !atoi((char *)output_5.lastread); 
      Serial.println("Output 5:");
      Serial.print((char *)output_5.lastread);
     //  statusMonitor.publish(atoi((char *)output_5.lastread));
  
      
    } if (subscription == &output_6) { 
       lcd.clear();
      lcd.print(F("Com 6: "));
      lcd.print((char *)output_6.lastread);
      dataArray[5] = !atoi((char *)output_6.lastread);
      Serial.println("Output 6:");
      Serial.print((char *)output_6.lastread); 
   //  statusMonitor.publish(atoi((char *)output_6.lastread));
      
    } if (subscription == &output_7) { 
      lcd.clear();
      lcd.print(F("Com 7: "));
      lcd.print((char *)output_7.lastread);
      dataArray[6] = !atoi((char *)output_7.lastread);
      Serial.println("Output 7:");
      Serial.print((char *)output_7.lastread);
     //  statusMonitor.publish(atoi((char *)output_7.lastread)); 

      
    } if (subscription == &output_8) {
      lcd.clear(); 
      lcd.print(F("Com 8: "));
      lcd.print((char *)output_8.lastread);
      dataArray[7] = !atoi((char *)output_8.lastread);
      Serial.println("Output 8:");
      Serial.print((char *)output_8.lastread);
     //  statusMonitor.publish(atoi((char *)output_8.lastread));
   
    } 
  writereg();
  }

 
}

void writereg(){      //LSBFIRST


digitalWrite(latchPin,LOW); 

for(int i =0;i<9;i++){
  digitalWrite(clockPin,HIGH); 
digitalWrite(dataPin,dataArray[i]); 
  digitalWrite(clockPin,LOW);
  Serial.println(i);
 
  }

  digitalWrite(latchPin,HIGH); 
  
  
  }


void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }
  Serial.println("Connecting to MQTT Server ...");
  lcd.print("Connecting to");
  lcd.setCursor(0,1);
  lcd.print("Server...");

  uint8_t retries = 3;
  
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
    lcd.print("System : Offline");
    delay(1000);
    lcd.clear();
    Serial.println("Retrying MQTT");
    lcd.print("Retrying...");
    delay(1000);
    lcd.clear();
    mqtt.disconnect(); 
    delay(5000);  // wait 5 seconds
    retries--;
    if (retries == 0) {
      // basically die and wait for WDT to reset me
      while (1);
    }
  }
  lcd.clear();
   lcd.print("System : Online");
 
  Serial.println("MQTT Connected!");
  delay(2000);
  
}
