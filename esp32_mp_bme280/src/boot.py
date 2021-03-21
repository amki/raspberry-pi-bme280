# boot.py - - runs on boot-up

#from machine import Pin, I2C
from time import sleep
#import BME280

# ESP32 - Pin assignment
#i2c = I2C(scl=Pin(22), sda=Pin(21), freq=10000)
#bme = BME280.BME280(i2c=i2c)

import network
sta_if = network.WLAN(network.STA_IF)
sta_if.active(True)
sta_if.scan()                             # Scan for available access points
sta_if.connect("thanksforallthefish", "derkleinekrebskrebstnichtmehr") # Connect to an AP
while not sta_if.isconnected():
    sleep(3)