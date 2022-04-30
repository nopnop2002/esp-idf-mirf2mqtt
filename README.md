# esp-idf-mirf2mqtt
nRF24L01 to mqtt bridge using esp32.   
It's purpose is to be a bridge between a nRF24L01 and a MQTT-Broker.    

![slide0001](https://user-images.githubusercontent.com/6020549/124595955-a33ed300-de9c-11eb-898d-e8cc4e8712fb.jpg)

![slide0002](https://user-images.githubusercontent.com/6020549/124596589-6b845b00-de9d-11eb-8279-83ee578cd872.jpg)

# Software requirements
esp-idf v4.4 or later.   
This is because this version supports ESP32-C3.   

# Installation

```Shell
git clone https://github.com/nopnop2002/esp-idf-mirf2mqtt
cd esp-idf-mirf2mqtt
idf.py set-target {esp32/esp32s2/esp32s3/esp32c3}
idf.py menuconfig
idf.py flash
```

__Note for ESP32C3__   
For some reason, there are development boards that cannot use GPIO06, GPIO08, GPIO09, GPIO19 for SPI clock pins.   
According to the ESP32C3 specifications, these pins can also be used as SPI clocks.   
I used a raw ESP-C3-13 to verify that these pins could be used as SPI clocks.   


# Wirering

|nRF24L01||ESP32|ESP32-S2/S3|ESP32-C3|
|:-:|:-:|:-:|:-:|:-:|
|MISO|--|GPIO19|GPIO37|GPIO18|
|MOSI|--|GPIO23|GPIO35|GPIO19|
|SCK|--|GPIO18|GPIO36|GPIO10|
|CE|--|GPIO16|GPIO34|GPIO9|
|CSN|--|GPIO17|GPIO33|GPIO8|
|GND|--|GND|GND|GND|
|VCC|--|3.3V|3.3V|3.3V|

__You can change it to any pin using menuconfig.__   

# Configuration for Transceiver
![config-nrf24l01-1](https://user-images.githubusercontent.com/6020549/166088080-8f7bf7a8-f76c-4176-9a85-4308920faf0b.jpg)
![config-nrf24l01-2](https://user-images.githubusercontent.com/6020549/155939063-5f70f146-f73d-4656-86f8-e7d770607b22.jpg)

# Configuration for Application
![config-app-1](https://user-images.githubusercontent.com/6020549/155939157-55604038-27c4-4cdd-9fb5-c1973f668c4f.jpg)
![config-app-2](https://user-images.githubusercontent.com/6020549/155939161-93c64e2a-4008-4309-8980-fb7173df1f02.jpg)

## Radio Setting   

- Receive from radio and send by MQTT   
```
[Arduino] ---> [nRF24L01] ---> [ESP32] ---> [MQTT]
```

![config-radio-1](https://user-images.githubusercontent.com/6020549/166088295-8850b41e-a55c-4338-92ce-43643d298cd0.jpg)

- Receive from MQTT and send by radio   
```
[Arduino] <--- [nRF24L01] <--- [ESP32] <--- [MQTT]
```

![config-radio-2](https://user-images.githubusercontent.com/6020549/166088297-a49f2806-1176-4160-b590-799feb4d4ea9.jpg)

___The payload size and radio channel must match for both transmission and reception.___

## WiFi Setting   
![config-wifi](https://user-images.githubusercontent.com/6020549/124592651-a46e0100-de98-11eb-90eb-2ded527454fe.jpg)

## MQTT Server Setting   
For Receiver, ESP32 publishes this topic.   
For Transmitter, ESP32 subscrives this topic.   
![config-mqtt](https://user-images.githubusercontent.com/6020549/124592695-afc12c80-de98-11eb-9675-814f2ae3d931.jpg)

# Communicate with AtMega/STM32/ESP8266/ESP8285   
I used [this](https://github.com/nopnop2002/Arduino-STM32-nRF24L01) library on Arduino environment.   
Install this library in your Arduino environment.   
After installing this library in your Arduino environment, run a sketch of the Arduino folder in this repository.   

# Receive data from AtMega/STM32/ESP8266/ESP8285   
You can receive MQTT data using mosquitto_sub.   
```mosquitto_sub -h broker.emqx.io -p 1883 -t '/mirf/#' -F %X -d```

![sub-1](https://user-images.githubusercontent.com/6020549/124676650-e5e2c880-def9-11eb-96d7-dd4475cdd2f1.jpg)


You can use Arduino/Emiter/Emitter.ino for transmitter.   
![sub-2](https://user-images.githubusercontent.com/6020549/124676646-e4b19b80-def9-11eb-95f2-d56050d75cce.jpg)

# Transmit data to AtMega/STM32/ESP8266/ESP8285   
You can transmit MQTT data using mosquitto_pub.   
```echo -ne "\x01\x02\x03" | mosquitto_pub -h broker.emqx.io -p 1883 -t '/mirf' -s```

![pub-1](https://user-images.githubusercontent.com/6020549/124676609-d2cff880-def9-11eb-80ab-f49fea19f6d6.jpg)

You can use Arduino/Receive/Receive.ino for receiver.   
![pub-2](https://user-images.githubusercontent.com/6020549/124676606-d2376200-def9-11eb-86ab-c0f17e958e55.jpg)

# Limitation   
The maximum payload size of nRF24L01 is 32 bytes.   
When publishing data exceeding 32 bytes, only 32 bytes are used.   

# Receive MQTT data using python
```
python -m pip install -U paho-mqtt
python mqtt_sub.py
```

![python-sub](https://user-images.githubusercontent.com/6020549/124855492-fc654e80-dfe3-11eb-8a9b-01d746479d88.jpg)

# Send MQTT data using python
```
python -m pip install -U paho-mqtt
python mqtt_pub.py
```

# MQTT client Example
Example code in various languages.   
https://github.com/emqx/MQTT-Client-Examples


# Visualize data from nRF24L01   

## Using python
There is a lot of information on the internet about the Python + visualization library.   
- [matplotlib](https://matplotlib.org/)
- [seaborn](https://seaborn.pydata.org/index.html)
- [bokeh](https://bokeh.org/)
- [plotly](https://plotly.com/python/)

## Using node.js
There is a lot of information on the internet about the node.js + __real time__ visualization library.   
- [epoch](https://epochjs.github.io/epoch/real-time/)
- [plotly](https://plotly.com/javascript/streaming/)
- [chartjs-plugin-streaming](https://nagix.github.io/chartjs-plugin-streaming/1.9.0/)

# Important
When changing the settings of the nRF24L01, it is necessary to power cycle the nRF24L01 before executing.   
Because nRF24L01 remembers the previous setting.   
nRF24L01 does not have Software Reset function.   

# Reference

https://github.com/nopnop2002/esp-idf-mirf

https://github.com/nopnop2002/esp-idf-mqtt-visualize
