#include <ESP8266WiFi.h>
#include <PubSubClient.h>
 
// Connect to the WiFi
const char* ssid = "WiFi_SSID";         // Poner aquí el nombre de la WiFi
const char* password = "WiFi_Password"; // Poner aquí el password de la WiFi
const char* mqtt_server = "m23.cloudmqtt.com"; // Servidor de la instancia CloudMQTT
 
WiFiClient espClient;
PubSubClient client(espClient);

int temp;
long lastMsg = 0;
 
void callback(char* topic, byte* payload, unsigned int length) {
 Serial.print("Message arrived [");
 Serial.print(topic);
 Serial.print("] ");
 for (int i=0;i<length;i++) {
  char receivedChar = (char)payload[i];
  Serial.print(receivedChar);
  if (receivedChar == '0')
  // ESP8266 Huzzah outputs are "reversed"
  digitalWrite(LED_BUILTIN, HIGH);
  if (receivedChar == '1')
   digitalWrite(LED_BUILTIN, LOW);
  }
  Serial.println();
}
 
 
void reconnect() {
 // Loop until we're reconnected
 while (!client.connected()) {
 Serial.print("Attempting MQTT connection...");
 // Attempt to connect,               <<Usuario>> << Password >>
 if (client.connect("ESP8266 Client", "bowmxsdl", "DYbXI6WWOwMe")) {
  Serial.println("connected");
  // ... and subscribe to topic
  client.subscribe("ledStatus");
 } else {
  Serial.print("failed, rc=");
  Serial.print(client.state());
  Serial.println(" try again in 5 seconds");
  // Wait 5 seconds before retrying
  delay(5000);
  }
 }
}
 
void setup()
{
 Serial.begin(9600);
 
 client.setServer(mqtt_server, 14785);
 client.setCallback(callback);
 
 pinMode(LED_BUILTIN, OUTPUT);
}
 
void loop()
{
 if (!client.connected()) {
  reconnect();
 }
 client.loop();
 
  long now = millis();
  
  if (now - lastMsg > 2000) {
    lastMsg = now;
    temp = map (analogRead (A0), 0, 1023, 0, 40);

    /*
    char buf[20]; String pubString;
    pubString = String (temp);
    pubString.toCharArray (buf, pubString.length()+1);
    Serial.println(buf);
    Serial.println (temp);    
    client.publish ("temp", buf);
    */
    client.publish ("temp", String (temp).c_str());
  }
}
