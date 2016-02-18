// Import required libraries
#include "ESP8266WiFi.h"
#include <Modbus.h>
#include <ModbusIP_ESP8266.h>
#include "dht11.h"

dht11 DHT11;

// WiFi parameters
const char* ssid = "Zender";
const char* password = "ginestous";

//ModbusIP object
ModbusIP mb;

void setup(void)
{ 
  // Start Serial
  Serial.begin(74880,SERIAL_8N1,SERIAL_TX_ONLY);

//  WiFi.begin(ssid, password);
   mb.config(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println(WiFi.localIP());

    mb.addIreg(100);
    mb.addIreg(101);

}
long ts = 0;

void loop() {
  
  mb.task();

  if (millis() < ts + 2000) 
    return;
    
  
  ts = millis();
   
  Serial.print("Read sensor: ");
  switch (DHT11.read(0))
  {
    case DHTLIB_OK: 
      Serial.print("Humidity (%): ");
      Serial.println((float)DHT11.humidity, 2);
    
      Serial.print("Temperature (°C): ");
      Serial.println((float)DHT11.temperature, 2);
      mb.Ireg(100, DHT11.temperature);
      mb.Ireg(101, DHT11.humidity);
      break;
      
    case DHTLIB_ERROR_CHECKSUM: 
      Serial.println("Checksum error"); 
    break;
    
    case DHTLIB_ERROR_TIMEOUT: 
      Serial.println("Time out error"); 
    break;
    
    default: 
      Serial.println("Unknown error"); 
    break;
  }
}
