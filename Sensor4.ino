 /*
  *  Here's an Arduino uIP demo which shows you how to:
  *   - acquire network configuration via DHCP
  *   - perform a DNS request and save the resulting IP in
  *     a IPAddress object
  *   - perform a HTTP GET request against a remote server
  *     (a Phant.io server in this example)
  *   - save a bit of memory by using a macro around PROGMEM
  *
  * ENC28J60 wiring on ATmega328P based boards:
  *  - SCK -> 13
  *  - SO  -> 12
  *  - SI  -> 11
  *  - CS  -> 10
  *   
  *  by Lorenzo Cafaro (lorenzo@ibisco.net)
  *
  */
 
 #include <UIPEthernet.h>
 #include <Dns.h>
 
 // See: https://www.arduino.cc/en/Reference/PROGMEM
 #define PGMprintln(x) println(F(x))
 #define PGMprint(x) print(F(x))
 
 // MAC address configuration
 const uint8_t mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
 
 // Destination host and port
 const char dsthost[] = "192.168.2.179";
 const uint16_t dstport = 80;
 // defines pins numbers
    const int trigPin = 5;
    const int echoPin = 6;
    // defines variables
    long duration;
    int distance;
    
 // Our three main objects
 EthernetClient client;
 DNSClient DNSclient;
 IPAddress dstip;
 
 void setup() {
   pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
    pinMode(echoPin, INPUT); // Sets the echoPin as an Input
    pinMode(15, OUTPUT);
    pinMode(16, OUTPUT);
   Serial.begin(9600);
 
   // Initializing Ethernet
   Serial.PGMprintln("Initializing Ethernet, waiting for DHCP...");
   Ethernet.begin(mac);
   Serial.PGMprint("IP: ");
   Serial.println(Ethernet.localIP());
   Serial.PGMprint("Subnet mask: ");
   Serial.println(Ethernet.subnetMask());
   Serial.PGMprint("Gateway: ");
   Serial.println(Ethernet.gatewayIP());
   Serial.PGMprint("DNS: ");
   Serial.println(Ethernet.dnsServerIP());
 
   // Initializing DNS
   Serial.PGMprintln("Initializing DNS client...");
   DNSclient.begin(Ethernet.dnsServerIP());
   if (DNSclient.getHostByName(dsthost, dstip)) {
     Serial.PGMprint("Resolved remote host to: ");
     Serial.println(dstip);
   } else {
     Serial.PGMprintln("Error: could not resolve remote host.");
   }
 }
 
 void loop() {
   // Perform your sensor readings here
   
   // Clears the trigPin
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(echoPin, HIGH);
    // Calculating the distance
    distance= duration*0.034/2;
    // Prints the distance on the Serial Monitor
    Serial.print("Distance: ");
    Serial.println(distance);
    if (distance > 400){
      frei();
      }else{
        busy();
        }
        delay(5000);
 }
   void frei(){
      digitalWrite(16, LOW);
      // Connecting to the remote host
   if (client.connect(dstip, dstport)) {
 
     // Prepare the buffers
     char getbuf[96] = {0};
     char hostbuf[32] = {0};
     sprintf(getbuf, "GET /4/aus", 102);
     sprintf(hostbuf, "\n Host: %s\n\n", dsthost);
 
     // Send the data
     client.print(getbuf);
     client.print(hostbuf);
 
     // Disconnect
     client.stop();
 
     Serial.PGMprintln("... sent!");
 
   } else {
     Serial.PGMprintln("... client connection failed!");
   }
 
   
    }
    
    void busy(){
      digitalWrite(15, LOW);
      digitalWrite(16, HIGH);
      // Connecting to the remote host
   if (client.connect(dstip, dstport)) {
 
     // Prepare the buffers
     char getbuf[96] = {0};
     char hostbuf[32] = {0};
     sprintf(getbuf, "GET /4/ein", 100);
     sprintf(hostbuf, "\n Host: %s\n\n", dsthost);
 
     // Send the data
     client.print(getbuf);
     client.print(hostbuf);
 
     // Disconnect
     client.stop();
 
     Serial.PGMprintln("... sent!");
 
   } else {
     Serial.PGMprintln("... client connection failed!");
   }
 
   
    }
   
