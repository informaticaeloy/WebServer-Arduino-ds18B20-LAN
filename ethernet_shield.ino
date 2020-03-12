/*
  Web Server
 
 A simple web server that shows the value of the analog input pins.
 using an Arduino Wiznet Ethernet shield. 
 
 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13
 * Analog inputs attached to pins A0 through A5 (optional)
 
 created 18 Dec 2009
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe
 
 */
//HOST SHIELD ETHERNET
#include <Arduino.h>
#include <U8g2lib.h>
#include <SPI.h>
#include <Wire.h>
#include <Ethernet.h>

// TEMPERATURA
#include <OneWire.h>
#include <DallasTemperature.h>

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = { 
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192,168,1,249);

const int oneWirePin = 4;
OneWire oneWireBus(oneWirePin);
DallasTemperature sensor(&oneWireBus);

U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0); 

// Initialize the Ethernet server library
// with the IP address and port you want to use 
// (port 80 is default for HTTP):
EthernetServer server(80);

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  sensor.begin(); 
   //while (!Serial) {
   // ; // wait for serial port to connect. Needed for Leonardo only
   //}

  u8g2.begin();
  
  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
}


void loop() {

        //u8g2.clearBuffer();
      u8g2.setFont(u8g2_font_profont22_tn);
      u8g2.drawStr(4,29,"MENSAJE" );
      u8g2.sendBuffer();

      delay(20000);
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      
      Serial.println("Leyendo temperaturas: ");
      sensor.requestTemperatures();
 
      Serial.print("Temperatura en sensor 0: ");
      Serial.print(sensor.getTempCByIndex(0));
      Serial.println(" ºC");
      
      Serial.print("Temperatura en sensor 1: ");
      Serial.print(sensor.getTempCByIndex(1));
      Serial.println(" ºC");
      
      Serial.print("Temperatura en sensor 2: ");
      Serial.print(sensor.getTempCByIndex(2));
      Serial.println(" ºC");
      
      Serial.print("Temperatura en sensor 3: ");
      Serial.print(sensor.getTempCByIndex(3));
      Serial.println(" ºC");
      
      if (client.available()) {

        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          // add a meta refresh tag, so the browser pulls again every 5 seconds:
          client.println("<meta http-equiv=\"refresh\" content=\"5\">");
          // output the value of each analog input pin
          for (int analogChannel = 0; analogChannel < 6; analogChannel++) {
            int sensorReading = analogRead(analogChannel);
            client.print("analog input ");
            client.print(analogChannel);
            client.print(" is ");
            client.print(sensorReading);
            client.println("<br />");       
          }
          client.println("</html>");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } 
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    u8g2.clearBuffer();  
    u8g2.drawStr(10,29,"DISCONNECTED");
    u8g2.sendBuffer();
    
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("client disonnected");
  }
}

String DisplayAddress(IPAddress address)
{
 return String(address[0]) + "." + 
        String(address[1]) + "." + 
        String(address[2]) + "." + 
        String(address[3]);
}
