import paho.mqtt.client as paho
import sys
import requests


def onMessage(client,userdata,message):
    msg=message.payload.decode()
    print(msg)
    url='https://api.telegram.org/bot1697000834:AAEsSYseP-hNd0WcYdIOZPdWZLHA9imguTQ/sendMessage?chat_id=-567379022&text={}'.format(msg)
    requests.get(url)




client=paho.Client()
client.on_message=onMessage


if client.connect("localhost",1883,60)!=0:
    print("not connected")
    sys.exit(-1)
else:
    print("connected")
client.subscribe("iot/energy")#topic
try:
    print("ctrl+c to disconnect")
    client.loop_forever()
except:
    print("disconnected")
client.disconnect()
