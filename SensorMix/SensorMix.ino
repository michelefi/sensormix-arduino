#include <SPI.h>
#include <Ethernet.h>


// the media access control (ethernet hardware) address for the shield:
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };  
byte packetBuffer[UDP_TX_PACKET_MAX_SIZE]; //buffer to hold incoming packet,
//the IP address for the shield:
byte ip[] = { 192, 168, 1, 100 };   
byte remoteIp[] = { 192, 168, 1, 5 };   
byte dns1[] = { 192, 168, 1, 254 };    
byte gateway[] = { 192, 168, 1, 254 };    
byte subnet[] = { 255, 255, 255, 0 };
unsigned int port = 8888;      // local port to listen on
EthernetUDP Udp;



// named constant for the pin the sensor is connected to
const int sensorPin = A0;
// room temperature in Celcius
const float baselineTemp = 20.0;
char  ReplyBuffer[] = "acknowledged";

void setup(){
  // open a serial connection to display values
  Serial.begin(9600);
  
  Ethernet.begin(mac, ip, dns1, gateway, subnet); 
  Udp.begin(port);
}

void loop(){
  char _buffer[200]; 
  // read the value on AnalogIn pin 0 
  // and store it in a variable
  char temp[10];
  int sensorVal = analogRead(sensorPin);

  // convert the ADC reading to voltage
  float voltage = (sensorVal/1024.0) * 5.0;
  dtostrf(voltage,1,2,temp);
  String voltageAsString = String(temp);


  // convert the voltage to temperature in degrees C
  // the sensor changes 10 mV per degree
  // the datasheet says there's a 500 mV offset
  // ((volatge - 500mV) times 100)
  float temperature = (voltage - .5) * 100;
  //Serial.println(dtostrf(temperature,1,2,temp));
  String temperatureAsString = String(temp);
  
  //String data = "sensor Value: " + sensorVal + ", Volts: " + voltageAsString + ", degrees C: " +  temperatureAsString;
 sprintf(_buffer, "sensor Value: %n, Volts: %s, degrees C: %s \n", sensorVal, temp,temp);
  
 Serial.print(_buffer);

  
  Udp.beginPacket(remoteIp, port);
  Udp.write(ReplyBuffer);
  Udp.endPacket();
  delay(10000);
}
