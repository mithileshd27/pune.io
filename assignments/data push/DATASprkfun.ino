#include <SPI.h>
#include <Ethernet.h>

byte mac[] = { 0x90, 0xA2, 0xDA, 0xDE, 0x9E, 0x74 };
char server[] = "data.sparkfun.com";
IPAddress ip(10,0,1,116);

EthernetClient client;

const String publicKey = "7JQDQ6Z0l5C4KJ0A7Wbr";
const String privateKey = "mzPdP5Bn4aHeAqyDJ98x";

float temp = 0;
float light = 0;
float humid =0;
float pir = 0;
float rain = 0;

void setup() {
  Serial.begin(9600);
  setupEthernet();
  Serial.println(F("=========== Ready to Stream ==========="));
}

void loop() {
  Serial.println("Posting");
  postData();
  delay(30000);
}  
  
void postData() {
  temp = analogRead(0);
  temp = temp*0.488;
  Serial.print("Temp ");
  Serial.println(temp);
  
  light = analogRead(1);
  Serial.print("Light ");
  Serial.println(light);

  humid = analogRead(2);
  humid = humid/1024;
  humid = humid*5;
  humid = humid/2.97;
  humid = humid*90;
  Serial.print("Humidity ");
  Serial.println(humid);
  
  pir = analogRead(3);
  Serial.print("pir ");
  Serial.println(pir);
  
  rain = analogRead(4);
  Serial.print("rain ");
  Serial.println(rain);
   
  if (client.connect(server, 80))
  {
    client.print("GET /input/");
    client.print(publicKey);
    client.print("?private_key=");
    client.print(privateKey);
    client.print("&");
    client.print("temp");
    client.print("=");
    client.print(temp);
    client.print("&");
    client.print("light");
    client.print("=");
    client.print(light);
    client.print("&");
    client.print("humid");
    client.print("=");
    client.print(humid);
    client.print("&");
    client.print("pir");
    client.print("=");
    client.print(pir);
    client.print("&");
    client.print("rain");
    client.print("=");
    client.print(rain);
    
    client.println(" HTTP/1.1");
    client.print("Host: ");
    client.println(server);
    client.println("Connection: close");
    client.println();
    Serial.println("Post Successful!!");
  }
  else
  {
    Serial.println(F("Connection failed"));
  } 

  while (client.connected())
  {
    if ( client.available() )
    {
      char c = client.read();
      Serial.print(c);
    }      
  }
  Serial.println();
  client.stop();  
}
  
void setupEthernet()
{
  Serial.println("Setting up Ethernet...");
  if (Ethernet.begin(mac) == 0) {
    Serial.println(F("Failed to configure Ethernet using DHCP"));
    Ethernet.begin(mac, ip);
  }
  Serial.print("My IP address: ");
  Serial.println(Ethernet.localIP());
  delay(1000);
}
