//Transmitter program

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
  //Set your own address (sender address) using 5 characters
  Mirf.setRADDR((byte *)"ABCDE");         //Set your own address (receiver address) using 5 characters
  Mirf.payload = sizeof(mydata);
  Serial.print("Mirf.payload=");
  Serial.println(Mirf.payload);
  Mirf.channel = 90;                      //Set the channel used
  Mirf.config();
  memset(mydata.payload.data, 0, sizeof(mydata.payload.data));
}

void loop()
{
  Mirf.setTADDR((byte *)"FGHIJ");         //Set Destination address
  mydata.payload.now_millis = millis();;  //Milliseconds
  unsigned long now_micros = micros();    //Microseconds
  //Serial.print("sizeof(now_micros)=");
  //Serial.println(sizeof(now_micros));
  memcpy(mydata.payload.data, &now_micros, sizeof(now_micros));
  Mirf.send(mydata.value);                //Send instructions, send random number value
  Serial.print("Wait for sending.....");
  //Test you send successfully
  if (Mirf.isSend()) {
    Serial.print("Send success:");
    Serial.println(mydata.payload.now_millis);
  } else {
    Serial.println("Send fail:");
  }
  delay(1000);
}
