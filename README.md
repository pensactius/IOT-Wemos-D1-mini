# IOT con Wemos D1
Recopilatorio de apuntes básicos sobre IOT y su aplicación con la placa Wemos D1. 
La mayor parte de la información contenida en este documento está extraída de
otros tutoriales de Internet. Se menciona la URL original de dónde se ha extraído
la información.

# Principios Básicos

En realidad se podría usar simplemente HTTP para IOT, haciendo que los dispositivos
se conecten a un servidor y realicen peticiones HTTP. Pero a la larga se hace
engorroso y por eso nació ` MQTT `

## MQTT machine-to-machine (M2M)/"Internet of Things" connectivity protocol.
http://mqtt.org

MQTT is an easy way for Internet of Things (IoT) devices to communicate with each other.


_La siguiente información se ha extraído de: https://www.baldengineer.com/mqtt-introduction.html_
### Message Brokering Basic Terms

  1. **Broker**: The broker accepts messages from clients and then delivers them to any interested clients. Messages belong to a topic. (Sometimes brokers are called “servers.”)

  1. **Client**: A “device” that either publishes a message to a topic, subscribes to a topic, or both.

  1. **Topic**: A namespace (or place) for messages on the broker. Clients subscribe and publish to a topic.

  1. **Publish**: A client sending a message to the broker, using a topic name.

  1. **Subscribe**: A client tells the broker which topics interest it. Once subscribed, the broker sends messages published to that topic. (In some configurations the broker sends “missed” messages.) A client can subscribe to multiple topics.

  1. **Unsubscribe**: Tell the broker you are bored with this topic. In other words, the broker will stop sending messages on this topic.
  
## MQTT Introduction
In the case of MQTT, there is a broker, and there are clients. The broker is 
the “Post Office”, or in computer terms, the “server.”

Clients can publish (Pub) messages, or send letters, to the post office. Other 
clients can subscribe (Sub) to topics, or receive letters, from the post office. 
Moreover, clients can both publish and subscribe.

That is how MQTT works. There is a broker who communicates with clients. 
There are topics which can have subtopics. When devices connect, they can either 
publish information on those topics or subscribe to receive information.  

### MQTT Brokers
A la hora de elegir un broker podemos elegir entre instalar el nuestro propio en un PC (o Raspberry/Beaglebone) o usar uno online.

### Mosquitto
[Mosquitto](http://mosquitto.org/) es un broker open source que se puede instalar localmente en un PC/Rpi/BBB. 
Si no se desea instalar localmente, existe un servidor de test en [test.mosquitto.org](http://test.mosquitto.org/) 
al que todo el mundo tiene acceso para realizar pruebas de MQTT.

_La siguiente información es un resumen de_:https://www.baldengineer.com/mqtt-tutorial.html
### Install MQTT for Python
The MQTT library I’m using is the Paho Python Client. It’s open source and supports the latest version of MQTT.

Installation is simple. First, install “pip” and then run:
```bash
pip install paho-mqtt
```
### Example MQTT Python Code for Raspberry Pi
Paho makes communicating with your MQTT server very simple.
```python
import paho.mqtt.publish as publish
import time
print("Sending 0...")
publish.single("ledStatus", "0", hostname="test.mosquitto.org")
time.sleep(1)
print("Sending 1...")
publish.single("ledStatus", "1", hostname="test.mosquitto.org")
```

El ejemplo anterior _publica_ en el servidor `test.mosquitto.org` el mensaje "0" o "1" bajo el _topic_ __ledStatus__.
Visualmente no se verá nada, pues únicamente se ha enviado el mensaje al servidor. Qué hacer con ese mensaje
dependerá de nosotros y las herramientas disponibles. Un ejemplo más visual sería el siguiente:

```python
import paho.mqtt.publish as publish
publish.single("temp/random", "20", hostname="test.mosquitto.org")
```
Si previamente cargamos la página http://test.mosquitto.org/gauge/ veremos cómo el _gauge_ se actualiza cada vez
que ejecutamos un _publish_ con un valor diferente.

## MQTT for ESP8266 (and Arduino)
[PubSubClient](https://github.com/knolleary/pubsubclient) is an Arduino-based MQTT client. With just a few lines of code, it is very easy to either subscribe to topics or publish new ones.

### Install MQTT for Arduino IDE
Installing PubSubClient for the Arduino IDE is easy. Either install manually from GitHub or use the Arduino Package Installer. Simple!
![PubsubInstall](https://github.com/pensactius/iot_info/blob/master/assets/PubSubLibrary_Install.PNG)

### Code Example for Arduino MQTT
__Atención__: El siguiente código no pertenece a PensActius, es una copia exacta de https://www.baldengineer.com/mqtt-tutorial.html. Únicamente se ha modificado para que encienda el _LED_BUILTIN_ que es el LED azul de la Wemos D1.

The following code connects to WiFi and subscribes to the topic _ledStatus_. When the ESP8266 receives a message, it acts by turning the “ledPin” on or off.

```C++
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
 
// Connect to the WiFi
const char* ssid = "miSSID";        // Poner aquí la SSID de la WiFi
const char* password = "miPasswd";  // Poner aquí el passwd de la WiFi
const char* mqtt_server = "test.mosquitto.org";
 
WiFiClient espClient;
PubSubClient client(espClient);
 
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
 // Attempt to connect
 if (client.connect("ESP8266 Client")) {
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
 
 client.setServer(mqtt_server, 1883);
 client.setCallback(callback);
 
 pinMode(LED_BUILTIN, OUTPUT);
}
 
void loop()
{
 if (!client.connected()) {
  reconnect();
 }
 client.loop();
}
```
Podemos usar el código anterior en python para encender y apagar el LED
```python
import paho.mqtt.publish as publish
import time
print("Sending 0...")
publish.single("ledStatus", "0", hostname="test.mosquitto.org")
time.sleep(1)
print("Sending 1...")
publish.single("ledStatus", "1", hostname="test.mosquitto.org")
```

El siguiente paso lógico sería tener una interfaz gráfica que permita controlar y monitorizar los dispositivos
conectados, i.e: un botón para encender el LED, un gráfico de temperatura a lo largo del tiempo, etc. Ese es
el tema de la siguiente sección.


## esp8266 con MicroPython
https://www.home-assistant.io/blog/2016/08/31/esp8266-and-micropython-part2/

### Brokers
iot.eclipse.org:443
