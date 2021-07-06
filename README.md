# esp-idf-mirf2mqtt
nRF24L01 to mqtt bridge using esp32.   
It's purpose is to be a bridge between a nRF24L01 and a MQTT-Broker.    

![slide0001](https://user-images.githubusercontent.com/6020549/124595955-a33ed300-de9c-11eb-898d-e8cc4e8712fb.jpg)

![slide0002](https://user-images.githubusercontent.com/6020549/124596589-6b845b00-de9d-11eb-8279-83ee578cd872.jpg)


# Installation for ESP32

```
git clone https://github.com/nopnop2002/esp-idf-mirf2mqtt
cd esp-idf-mirf2mqtt
idf.py set-target esp32
idf.py menuconfig
idf.py flash
```

# Installation for ESP32S2

```
git clone https://github.com/nopnop2002/esp-idf-mirf2mqtt
cd esp-idf-mirf2mqtt
idf.py set-target esp32s2
idf.py menuconfig
idf.py flash
```

# Wirering

|nRF24L01||ESP32|ESP32S2|
|:-:|:-:|:-:|:-:|
|MISO|--|GPIO19|GPIO33|
|MOSI|--|GPIO23|GPIO35|
|SCK|--|GPIO18|GPIO36|
|CE|--|GPIO16|GPIO37|
|CSN|--|GPIO17|GPIO38|
|GND|--|GND|GND|
|VCC|--|3.3V|3.3V|

__You can change it to any pin using menuconfig.__   
__However, changing to some pins does not work properly.__


# Configuration

![config-main](https://user-images.githubusercontent.com/6020549/124592504-830d1500-de98-11eb-95b7-9787323a645f.jpg)
![config-app](https://user-images.githubusercontent.com/6020549/124592510-856f6f00-de98-11eb-8413-254b1c448530.jpg)

## nRF24L01 Setting
![config-nrf24l01](https://user-images.githubusercontent.com/6020549/124592589-95874e80-de98-11eb-8953-db44222a685d.jpg)

## WiFi Setting
![config-wifi](https://user-images.githubusercontent.com/6020549/124592651-a46e0100-de98-11eb-90eb-2ded527454fe.jpg)

## MQTT Server Setting
![config-mqtt](https://user-images.githubusercontent.com/6020549/124592695-afc12c80-de98-11eb-9675-814f2ae3d931.jpg)

# Communicate with AtMega/STM32/ESP8266/ESP8285   
I used [this](https://github.com/nopnop2002/Arduino-STM32-nRF24L01) library on Arduino environment.   
Install this library in your Arduino environment.   

# Receive data from AtMega/STM32/ESP8266/ESP8285   
You can receive MQTT data using mosquitto_sub.   
```mosquitto_sub -h 192.168.10.40 -p 1883 -t '/mirf/#' -F %X -d```

![sub-1](https://user-images.githubusercontent.com/6020549/124676650-e5e2c880-def9-11eb-96d7-dd4475cdd2f1.jpg)


You can use Arduino/Emiter/Emitter.ino for transmitter.   
![sub-2](https://user-images.githubusercontent.com/6020549/124676646-e4b19b80-def9-11eb-95f2-d56050d75cce.jpg)

# Transmit data to AtMega/STM32/ESP8266/ESP8285   
You can transmit MQTT data using mosquitto_pub.   
```echo -ne "\x01\x02\x03" | mosquitto_pub -h 192.168.10.40 -p 1883 -t '/mirf' -s```

![pub-1](https://user-images.githubusercontent.com/6020549/124676609-d2cff880-def9-11eb-80ab-f49fea19f6d6.jpg)

You can use Arduino/Receive/Receive.ino for receiver.   
![pub-2](https://user-images.githubusercontent.com/6020549/124676606-d2376200-def9-11eb-86ab-c0f17e958e55.jpg)

# Limitation   
The maximum payload size of nRF24L01 is 32 bytes.   
When publishing data exceeding 32 bytes, only 32 bytes are used.   

# Receive MQTT data using python
```
python -m pip install -U paho-mqtt
python sub.py
```

# MQTT client Example
Example code in various languages.   
https://github.com/emqx/MQTT-Client-Examples


# Visualize data from nRF24L01   

## Using python
There is a lot of information on the internet about the Python + visualization library.   
- [matplotlib](https://matplotlib.org/)
- [seaborn](https://seaborn.pydata.org/index.html)
- [chart.js](https://www.chartjs.org/)
- [bokeh](https://bokeh.org/)
- [plotly](https://plotly.com/python/)

## Using node.js
There is a lot of information on the internet about the node.js + __real time__ visualization library.   
- [epoch](https://epochjs.github.io/epoch/real-time/)
- [plotly](https://plotly.com/javascript/streaming/)
- [pusher](https://pusher.com/tutorials/graph-javascript/)


# Important
When changing the settings of the nRF24L01, it is necessary to power cycle the nRF24L01 before executing.   
Because nRF24L01 remembers the previous setting.   
nRF24L01 does not have Software Reset function.   

# Reference

https://github.com/nopnop2002/esp-idf-mirf


