#include <UIPEthernet.h> // Used for Ethernet
#include <WString.h>
#include "Adafruit_WS2801.h"
#include "SPI.h" // Comment out this line if using Trinket or Gemma
uint8_t dataPin  = 5;    // Yellow wire on Adafruit Pixels
uint8_t clockPin = 6;    // Green wire on Adafruit Pixels
Adafruit_WS2801 strip = Adafruit_WS2801(64, dataPin, clockPin);

// **** ETHERNET SETTING ****
byte mac[] = { 0x54, 0x34, 0x41, 0x30, 0x30, 0x31 };                                      
IPAddress ip(192, 168, 2, 179);                        
EthernetServer server(80);
String readString = String(10);

void setup() {
  strip.begin();
  strip.show();
  Serial.begin(9600);

  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();

  //Serial.print("IP Address: ");
  //Serial.println(Ethernet.localIP());
  readString="";
  int i;
  for (i=1; i < 5; i++) {
      area(Color(255, 0, 0), i);
      
   }
   delay(500);
   for (i=1; i < 5; i++) {
      area(Color(0, 255, 0), i);
   }
   delay(500);
   for (i=1; i < 5; i++) {
      area(Color(0, 0, 0), i);
  }
}

void loop() {
  // listen for incoming clients
  EthernetClient client = server.available();

  if (client)
  {  
    //Serial.println("-> New Connection");

    // an http request ends with a blank line
    boolean currentLineIsBlank = true;

    while (client.connected())
    {
      if (client.available())
      {
        char c = client.read();
        //read char by char HTTP request
        if (readString.length() < 10) {

          //store characters to string
        readString+= c;
        }

        //if HTTP request has ended
        if (c == '\n') {
          uint8_t zahl = atoi (readString.substring(6, 5).c_str ());
          Serial.println(readString);
           //Serial.println(readString.substring(10, 7));
            if (readString.substring(10, 7)=="ein"){
                area(Color(0,255,0),zahl);
            }
            if(readString.substring(10, 7)=="aus"){
                area(Color(255,0,0),zahl);
            }
            if(readString.substring(10, 7)=="0ff"){
                area(Color(0,0,0),zahl);
            }
            
        }
        if (c == '\n' && currentLineIsBlank)
        {
          client.println("<html><title>GAS!</title><body><h3>GAS!</h3><a href=\"/1/ein\">1 AN </a><a href=\"/2/ein\">2 AN </a><a href=\"/3/ein\">3 AN </a><a href=\"/4/ein\">4 AN </a><p><a href=\"/1/aus\">1 AUS </a><a href=\"/2/aus\">2 AUS </a><a href=\"/3/aus\">3 AUS </a><a href=\"/4/aus\">4 AUS </a></body>");
          break;
        }

        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        }
        else if (c != '\r')
        {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }

    // give the web browser time to receive the data
    delay(10);

    // close the connection:
    client.stop();
    //Serial.println("   Disconnected\n");
    readString="";
    
  }
}

void area(uint32_t c, uint8_t number){
   int i;
   int startpixel = 3+(28/4)*number-7;
   for (i=startpixel; i < startpixel+7; i++) {
      strip.setPixelColor(i, c);
   }
   int zweiter = 56-(number*7)+3;
   for (int j=zweiter; j < zweiter+7;j++){
      strip.setPixelColor(j, c);
  }
  strip.show();
}

/* Helper functions */

// Create a 24 bit color value from R,G,B
uint32_t Color(byte r, byte g, byte b){
  uint32_t c;
  c = r;
  c <<= 8;
  c |= g;
  c <<= 8;
  c |= b;
  return c;
}

//Input a value 0 to 255 to get a color value.
//The colours are a transition r - g -b - back to r
uint32_t Wheel(byte WheelPos)
{
  if (WheelPos < 85) {
   return Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if (WheelPos < 170) {
   WheelPos -= 85;
   return Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170; 
   return Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

