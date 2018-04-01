
#!/bin/python

import paho.mqtt.publish as publish
import time

for i in range(10):
  print("Sending 0...")
  publish.single("ledStatus", "0", hostname="m23.cloudmqtt.com", 
        auth={'username':"bowmxsdl", 'password':"DYbXI6WWOwMe"},
        port=13013)

  time.sleep(1)

  print("Sending 1...")
  publish.single("ledStatus", "1", hostname="m23.cloudmqtt.com",
        auth={'username':"bowmxsdl", 'password':"DYbXI6WWOwMe"},
        port=13013)

  time.sleep(1)
