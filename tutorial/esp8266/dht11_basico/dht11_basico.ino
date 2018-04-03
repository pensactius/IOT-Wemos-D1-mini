// Incluímos la librería
#include <DHT.h>

// Definimos el pin digital donde se conecta el sensor (D7 = Arduino 13)
const int DHTPIN = 13;  // D7 en la Wemos D1

// Tipo de sensor
const int DHTTYPE = DHT11;

// Crea e inicializa una variable para el sensor DHT11
DHT dht (DHTPIN, DHTTYPE);

void setup() {
  
  Serial.begin (9600);
  Serial.println ("DHT11 test...");

  dht.begin();

}

void loop() {

  // Wait a few seconds between measurements
  delay (2000);

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

  Serial.print ("Humidity: "); Serial.print (h); Serial.print (" %\t");
  Serial.print ("Temperature: "); Serial.print (t); Serial.println (" C ");
 
}
