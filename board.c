#include <SPI.h>  
#include <RFID.h>  
#define SS_PIN 10  
#define RST_PIN 5 
 
const int sensorPin = A0; //temp
const float cutoffVolt = 0; // degrees Celsius

RFID rfid(SS_PIN,RST_PIN);  
int serNum[5], serNumf[5];
int sw; // halting printing of the same values, for the RFID module

void setup() {
  while (!Serial) {} // Wait for the serial to connect
  
  Serial.begin (9600);
  Serial.print ("The Serial has been initialised!\n");

  SPI.begin();  
  rfid.init();

  for (int i = 0; i < 5; ++i) serNumf[i] = 0;
}

void loop() {

   sw = 0;
   if(rfid.isCard()){  
     if(rfid.readCardSerial()) {

        for (int i = 0; i < 5 && sw == 0; ++i) if (rfid.serNum[i] != serNumf[i]) sw = 1;
        
        if (sw == 1) {
          Serial.print(rfid.serNum[0],DEC);  
          Serial.print(" ");  
          Serial.print(rfid.serNum[1],DEC);  
          Serial.print(" ");  
          Serial.print(rfid.serNum[2],DEC);  
          Serial.print(" ");  
          Serial.print(rfid.serNum[3],DEC);  
          Serial.print(" ");  
          Serial.print(rfid.serNum[4],DEC);  
          Serial.println(""); 
          }
     }  
  }  
  rfid.halt();  
  for (int i = 0; i < 5; ++i)
        serNumf[i] = rfid.serNum[i];
      

  /*
  int sensorVal = analogRead (sensorPin); // 0 - 1024
  Serial.print ("Sensor value: ");
  Serial.print (sensorVal); // ADC reading 
  float voltage = sensorVal * 5.0 / 1024.0;
  Serial.print ("; Volts: ");
  Serial.print (voltage);
  float tempC = 22 * voltage - 40;
  Serial.print ("; Temperature: ");
  Serial.print (tempC);
  Serial.print (" C\n");
  delay (1000);
  */
}