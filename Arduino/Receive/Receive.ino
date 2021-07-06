//Receiver program

#include "Mirf.h"

Nrf24l Mirf = Nrf24l(10, 9);

typedef struct {
    unsigned long now_millis;
    uint8_t data[28];
} PAYLOAD_t;

union MYDATA_t{
  byte value[32];
  PAYLOAD_t payload;
};

MYDATA_t mydata;

void setup()
{
  Serial.begin(115200);
  Mirf.spi = &MirfHardwareSpi;
  Mirf.init();
  Mirf.setRADDR((byte *)"FGHIJ"); //Set your own address (receiver address) using 5 characters
  Mirf.payload = sizeof(mydata);
  Mirf.channel = 90;             //Set the used channel
  Mirf.config();
  Serial.println("Listening...");  //Start listening to received data
}

void loop()
{
  if (Mirf.dataReady()) { //When the program is received, the received data is output from the serial port
    Mirf.getData(mydata.value);
    Serial.print("Got now_millisl: ");
    Serial.println(mydata.payload.now_millis);
    for (int i=0;i<28;i++) {
      Serial.print(mydata.payload.data[i], HEX);
      Serial.print(" ");
    }
    Serial.println();
  }
}
