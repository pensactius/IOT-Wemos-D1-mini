#include <ESP8266WiFi.h>    // ESP8266 wifi
#include <PubSubClient.h>   // MQTT
#include <DHT.h>            // Sensor Temp/Humedad

// Definimos el pin digital donde se conecta el sensor
const int DHTPIN = 13;

// Tipo de sensor
const int DHTTYPE = DHT11;

// Crea e inicializa una variable para el sensor DHT11
DHT dht (DHTPIN, DHTTYPE);

// Connect to the WiFi
const char* ssid = "........";         // Poner aquí el nombre de la WiFi
const char* password = "............"; // Poner aquí el password de la WiFi
const char* mqtt_server = "test.mosquitto.org"; // Servidor Mosquitto

WiFiClient espClient;           // variable para conectar a la WiFi
PubSubClient client(espClient); // variable para protocolo MQTT

long lastMsg = 0;               // Controla el tiempo entre mensaje y mensaje MQTT

void reconnect() {
 // Loop until we're reconnected
 while (!client.connected()) {
 Serial.print("Attempting MQTT connection...");
 // Attempt to connect,             
 if (client.connect("ESP8266 Client")) {
  Serial.println("connected");

 } else {
  Serial.print("failed, rc=");
  Serial.print(client.state());
  Serial.println(" try again in 5 seconds");
  // Wait 5 seconds before retrying
  delay(5000);
  }
 }
}

void setup() {
  
  Serial.begin (9600);

  // Función definida más abajo para conectar a la WiFi
  setup_wifi();
  
  client.setServer (mqtt_server, 1883);

  dht.begin();

}

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {

  // Intenta conectar al servidor MQTT si se desconecta
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();

  if (now - lastMsg > 2000) {

    lastMsg = now;    

    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (it's a very slow sensor)
    float h = dht.readHumidity();
  
    //Read temperature in Celsius (the default)
    float t = dht.readTemperature();
  
    // Check if any reads failed and exit early (to try again).
    if ( isnan (h) || isnan (t) ) {
      Serial.println ("Failed to read from DHT sensor, trying again...");
      return;
    }

    client.publish ("temp", String (t).c_str());
    client.publish ("humidity", String (h).c_str());
  
    Serial.print ("Humidity: "); Serial.print (h); Serial.print (" %\t");
    Serial.print ("Temperature: "); Serial.print (t); Serial.println (" C ");    
  }
 
 
}
