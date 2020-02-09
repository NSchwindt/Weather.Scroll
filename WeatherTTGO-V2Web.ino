#include "WiFi.h"
#include <HTTPClient.h>
#include <iostream> 
#include <string.h> 
#include <SPI.h>
#include "LedMatrix.h"

#define NUMBER_OF_DEVICES 4 //number of led matrix connect in series
#define CS_PIN 15
#define CLK_PIN 13
#define MISO_PIN 2 //we do not use this pin just fill to match constructor
#define MOSI_PIN 12

LedMatrix ledMatrix = LedMatrix(NUMBER_OF_DEVICES, CLK_PIN, MISO_PIN, MOSI_PIN, CS_PIN);

const char* ssid = "gotem";
const char* password =  "hellothere";
String zip_code = "";
String temp = "";
String weatherText = "";
String location = "";
String finalTxt = "";


void setup() {

  ledMatrix.init();
  ledMatrix.clear();
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}
 
void loop() {
 
  if ((WiFi.status() == WL_CONNECTED)) { //Check the current connection status
 
    HTTPClient http;

    Serial.begin(9600);
 
    http.begin("http://dataservice.accuweather.com/currentconditions/v1/349291?apikey=PX7H12JDrYfh3oSh5cxM39quGGGVzlFb&language=en-US&details=false"); //Specify the URL
    int httpCode = http.GET();                                        //Make the request
 
    if (httpCode > 0) { //Check for the returning code
        String payload = http.getString();
        Serial.print(payload);
        weatherText = payload.substring( payload.indexOf("WeatherText")+14,payload.indexOf("WeatherIcon")-3);
		    temp = payload.substring(payload.indexOf("\"Imperial\"")+20,payload.indexOf(",\"Unit\":\"F\"")) + " F";
		    location = payload.substring( payload.indexOf("/en/us")+7 , payload.indexOf("current-weather")-7);
        location.toUpperCase();
        zip_code = payload.substring( payload.indexOf("/en/us")+16 , payload.indexOf("current-weather")-1);
		    finalTxt = location + "  " + zip_code + "   " + temp + "   " + weatherText + "    ";
      }
 
    else {
      Serial.println("Error on HTTP request");
    }
 
    http.end(); //Free the resources
  }

  ledMatrix.setText(finalTxt);
  for(int i=0;i<2400;i++){
  ledMatrix.clear();
  ledMatrix.scrollTextLeft();
  ledMatrix.drawText();
  ledMatrix.commit();
  delay(25);
  }
 
}
