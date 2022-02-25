/*Mini weather station
Pedro Blasco pblamar925@g.educaand.es
*/

//Inclusión de librerías
//Sensor DHT11
#include <Adafruit_Sensor.h> //Needed if Adafruit libraries are used
#include <DHT_U.h>
#include <DHT.h> //Humidity sensosr library from Adafruit

//Pantalla
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>


//WIFI
#include <NTPClient.h>
#include <WiFi.h>
#include <WiFiUdp.h>

//Definición de constantes
#define DHTTYPE DHT11 
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino/ESP32 reset pin)

// Declarao SSD1306 display conectado a bus I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);



//Declaro sensor humedad en pin 12
DHT dht(12,DHTTYPE); 

//******** WIFI CONF *******
const char *ssid     = "Andared";
const char *password = "llevalatararaunvestidoblancollenodecascabeles";

//NTP Conf
const long utcOffsetInSeconds = 3600;//UTC+1
char daysOfTheWeek[7][12] = {"Domingo", "Lunes", "Martes", "Miercoles", "Jueves", "Viernes", "Sabado"};

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);


void setup() {
  //Arranco puerto serie
  Serial.begin(115200);
  
  //Arranco pantalla
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 No se encuentra display"));
    for(;;); // Don't proceed, loop forever
  }
  display.clearDisplay();
  
  //Arranco WiFi
  WiFi.mode(WIFI_MODE_STA);//Modo estación
  WiFi.begin(ssid, password);
  Serial.print("Conectando a:\t");
  Serial.println(ssid); 

  // Esperar a que nos conectemos
  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
   Serial.print('.');
  }

  // Mostrar mensaje de exito y dirección IP asignada
  Serial.println();
  Serial.print("Conectado a:\t");
  Serial.println(WiFi.SSID()); 
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());

  
  timeClient.begin();

  
  dht.begin();
}
//*********************************************
void loop() {
  timeClient.update();

  Serial.print(daysOfTheWeek[timeClient.getDay()]);
  Serial.print(", ");
  Serial.print(timeClient.getHours());
  Serial.print(":");
  Serial.print(timeClient.getMinutes());
  Serial.print(":");
  Serial.println(timeClient.getSeconds());
  Serial.print("Son las: ");
  Serial.println(timeClient.getFormattedTime());


  //Leo temperaturas y humedad
  float h = dht.readHumidity();
  float t = dht.readTemperature();
 
  Serial.println("Temperatura: "+ String(t) + "º");
  Serial.println("Humedad: " + String(h) + "%");
  
  
  //Presento en pantalla
  display.clearDisplay();
  display.drawLine(0,0,SCREEN_WIDTH,0,WHITE);
  display.drawRect(0,15,SCREEN_WIDTH, SCREEN_WIDTH-16,WHITE);
  display.setTextSize(1); 
  display.setTextColor(SSD1306_WHITE);
  
  display.setCursor(0,2); 
  display.print(F("MeteoProto1 | "));
  display.println(F(ssid));
  
  display.setCursor(3,17);  
  display.print(F("Temperatura: "));
  display.print((t));
  display.println(F("·C"));  
  
  display.setCursor(3,34);
  display.print(F("Humedad: "));
  display.print((t));
  display.println(F("%"));
  
  display.setCursor(3,51);
  display.print(F("Actualizado:"));
  display.println(timeClient.getFormattedTime());
  display.display();
  delay(10000);
}
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
