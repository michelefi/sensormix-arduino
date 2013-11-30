#include <SPI.h>
#include <Ethernet.h>
#include <LiquidCrystal.h>


// the media access control (ethernet hardware) address for the shield:
byte mac[] = { 
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };  
byte packetBuffer[UDP_TX_PACKET_MAX_SIZE]; //buffer to hold incoming packet,
//the IP address for the shield:
byte ip[] = { 
  192, 168, 1, 202 };   
byte remoteIp[] = { 
  192, 168, 1, 5 };   
byte dns1[] = { 
  192, 168, 1, 254 };    
byte gateway[] = { 
  192, 168, 1, 254 };    
byte subnet[] = { 
  255, 255, 255, 0 };
unsigned int port = 10081;
EthernetUDP Udp;

// named constant for the pin the sensor is connected to
const int temperaturePin = A0;
const int lightPin = A1;

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(9, 8, 5, 4, 3, 2);

void setup(){
  // open a serial connection to display values
  Serial.begin(9600);

  Ethernet.begin(mac, ip, dns1, gateway, subnet); 
  Udp.begin(port);

  lcd.begin(16, 2);
  lcd.print("SensorMix");

  char  initBuffer[] = "{ 'sensor' : {'id':'urn:rixf:org.arduino/sensor_id', type : 'urn:rixf:net.sensormix/device_types/arduino', name: 'Arduino', description : 'Rev. 1.03, Temp and Light sensors'}}\n";      
  Udp.beginPacket(remoteIp, port);
  Udp.write(initBuffer);
  Udp.endPacket();
}

void loop(){
  char _buffer[200];
  char _lcd[16]; 

  // read the value on AnalogIn pin 0 
  // and store it in a variable
  int sensorVal = analogRead(temperaturePin);

  // convert the ADC reading to voltage
  float voltage = (sensorVal/1024.0) * 5.0;

  // convert the voltage to temperature in degrees C
  // the sensor changes 10 mV per degree
  // the datasheet says there's a 500 mV offset
  // ((volatge - 500mV) times 100)
  float temperature = (voltage - .5) * 100;
  char temperatureChar[10];
  dtostrf(temperature,1,2,temperatureChar);

  int lightVal = analogRead(lightPin);
  float lightvoltage = (lightVal/1024.0) * 5.0;
  float lux = lightvoltage*lightvoltage*20;
  char luxChar[10];
  dtostrf(lux,1,2,luxChar);

  //String data = "sensor Value: " + sensorVal + ", Volts: " + voltageAsString + ", degrees C: " +  temperatureAsString;
  sprintf(_buffer, "{'sample': {'device_id':'urn:rixf:org.arduino/sensor_id', 'temp': '%s', 'lux': '%s'}}\n", temperatureChar, luxChar );
  Serial.print(_buffer);

  Udp.beginPacket(remoteIp, port);
  Udp.write(_buffer);
  Udp.endPacket();

  lcd.clear();
  // set the cursor to column 0, line 0     
  lcd.setCursor(0, 0);
  lcd.print("TEMP: ");
  lcd.print(temperatureChar);
  lcd.setCursor(0, 1);
  lcd.print("LUX: ");
  lcd.print(luxChar);
  delay(30000);
}








