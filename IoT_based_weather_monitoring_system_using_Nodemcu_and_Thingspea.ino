 /*Weather monitoring system with Thingspeak.
   
*/
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <Wire.h>
#include <ThingSpeak.h>
#include <ESP8266WiFi.h>//ESP8266
#include "DHT.h"

//#include <BMP280.h>
//BMP280 bmp280;




DHT dht(D3, DHT11);
Adafruit_BMP280 bmp280;
double T, P;
char status;
WiFiClient client;
//#define BMP_SCK (13)
//#define BMP_MISO (12)
 //#define BMP_MOST (11)
//#define BMP_CS (10)
                 

unsigned long myChannelNumber = 2161939; // Channel ID here
const int FieldNumber = 1;
String apiKey = "JLCH5LHBBPEIYVGG";
const char *ssid =  "*Pass Vs. *Fail";
const char *pass =  "12345678";
const char* server = "api.thingspeak.com";


void setup() {


  
  Serial.begin(115200);
  delay(10);
  bmp280.begin();
  Wire.begin();
  dht.begin();
 ThingSpeak.begin(client);
  WiFi.begin(ssid, pass);


  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
pinMode(BUILTIN_LED, OUTPUT); 


   bmp280.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */

                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */

                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */

                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */

                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */

}


void loop() {
 
 
 digitalWrite(BUILTIN_LED, HIGH);
  delay(100); 
   
digitalWrite(BUILTIN_LED, LOW);
  delay(100);



//Get pressure value
  uint32_t pressure = bmp280.readPressure();
  float alt = bmp280.readAltitude(1018);
  //float Temp=((temperature*5)/1024)*1000;
      //Temp=Temp/10;
  //float t = Temp;
//Print the results
  Serial.println("altitude //: ");
  Serial.print(alt);
  Serial.println("°C \t");
  Serial.println("Pressure//: ");
  Serial.print(pressure);
  Serial.println("Pa");



//LM35 sensor
 float ADC=analogRead(A0);   //read lm35 analog outputs at A0 and store it on ADC
 float temp=((ADC*5)/1024)*1000;
 temp=temp/10;
float  t = temp+22;
  
  Serial.print("Temperature = ");
  Serial.print(t);
 Serial.println(" *C");
  delay(100);
//  ThingSpeak.writeField(myChannelNumber, FieldNumber, temp, apiKey);
  
                
             //BMP280 sensor
    Serial.print(("Temperature = "));//F

   Serial.print(bmp280.readTemperature());

    Serial.println(" *C");

    Serial.print(("Pressure = "));//F

    Serial.print(pressure);

    Serial.println(" Pa");

        Serial.print(("Approx altitude = "));//F

    Serial.print(bmp280.readAltitude(1018)); /* Adjusted to local forecast! */

    Serial.println(" m");

    Serial.println();

    delay(200);

 
   
 //DHT11 sensor
  float h = dht.readHumidity();
  //float t = digitalRead(D0);     
  
  //Rain sensor
  int r = analogRead(A0);
  r = map(r, 0, 1024, 0, 100);


  if (client.connect(server, 80)) {
    String postStr = apiKey;
    postStr += "&field1=";
    postStr += String(t);
    postStr += "&field2=";
    postStr += String(h);
    postStr += "&field3=";
    postStr += String(pressure);
    postStr += "&field4=";
    postStr += String(r);
    postStr += "\r\n\r\n\r\n\r\n";

    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n\n\n");
    client.print(postStr);

    Serial.print("Temperature: ");
    Serial.println(t);
    Serial.print("Humidity: ");
    Serial.println(h);
    Serial.print("absolute pressure: ");
    Serial.print(pressure);
    Serial.println("mb");
    Serial.print("Rain");
    Serial.println(r);

  }
  client.stop();
  Serial.println("Waiting...");
  delay(2000);
}
