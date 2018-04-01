#!/bin/python

import paho.mqtt.publish as publish
import time

for i in range(10):
  print("Sending 0...")
  publish.single("ledStatus", "0", hostname="test.mosquitto.org")

  time.sleep(1)

  print("Sending 1...")
  publish.single("ledStatus", "1", hostname="test.mosquitto.org")
