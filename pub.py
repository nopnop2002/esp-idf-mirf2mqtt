#!/usr/bin/env python
# -*- coding: utf-8 -*-
import paho.mqtt.client as mqtt

# MQTT Broker
host = '192.168.10.40'
#host = 'broker.emqx.io'
# MQTT Port
port = 1883
# MQTT Publis Topic
topic = '/mirf'

if __name__=='__main__':
	client = mqtt.Client(protocol=mqtt.MQTTv311)
	client.connect(host, port=port, keepalive=60)
	payload = bytearray(b'\x11\x22\x33\x44')
	client.publish(topic, payload)
	client.disconnect()
