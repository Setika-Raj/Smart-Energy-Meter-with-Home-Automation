import paho.mqtt.client as paho
import sys


import serial


client=paho.Client()
recieved=serial.Serial('COM3',9600)






while True:
    
    
    data=recieved.readline().decode('utf-8')
    print(data);
    if client.connect("localhost",1883,60)!=0:
        print("not connected")
        sys.exit(-1)
    else:
        print("connected")
    client.publish("iot/energy",data,0)#topic,message,qos
    client.disconnect()
    data=""


