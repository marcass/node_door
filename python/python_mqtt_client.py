#!/usr/bin/env python
import time
import paho.mqtt.client as mqtt
import os

broker = "nuc"
sound = '/root/bin/sounds/Cow-moo-sound.mp3'

# The callback for when the client receives a CONNACK response from the server.
def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))
    # Subscribing in on_connect() means that if we lose the connection and
    # reconnect then subscriptions will be renewed.
    client.subscribe("door/state")

# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
    print(msg.topic+' '+str(msg.payload))
    #allowed_passthrough_msg = ['Opened']
    if 'Opened' in msg.payload:
        os.system('mpg123 ' + sound)
        time.sleep(60)
        print 'Played ' + sound + ' and sleeping for 60s'
        #stops door chime going constantly

if __name__ == "__main__":
    client = mqtt.Client()
    client.username_pw_set(username='esp', password='heating')
    client.on_connect = on_connect
    client.on_message = on_message
    client.connect(broker, 1883, 60)
    # Blocking call that processes network traffic, dispatches callbacks and
    # handles reconnecting.
    # Other loop*() functions are available that give a threaded interface and a
    # manual interface.
    client.loop_forever()
    #client.loop_start()