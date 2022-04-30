//Transmitter program

#include "Mirf.h"

Nrf24l Mirf = Nrf24l(10, 9);

byte mydata[32];

void setup()
{
  Serial.begin(115200);
  Mirf.spi = &MirfHardwareSpi;
  Mirf.init();
  Mirf.payload = sizeof(mydata); // Set the payload size
  Mirf.channel = 90;             //Set the channel used
  Mirf.config();

  //Set your own address (sender address) using 5 characters
  Mirf.setRADDR((byte *)"ABCDE");

  //Set destination address using 5 characters
  Mirf.setTADDR((byte *)"FGHIJ");
  
  memset(mydata, 0, sizeof(mydata));
}

void loop()
{
  static byte byteData = 0x00;
  static int offset = 0;

  //Serial.println(byteData,HEX);
  //Serial.println(offset);
  mydata[offset] = byteData;

  Mirf.send(mydata);                //Send instructions, send random number value
  Serial.print("Wait for sending.....");
  //Test you send successfully
  if (Mirf.isSend()) {
    Serial.print("Send success: byteData=");
    Serial.print(byteData, HEX);
    Serial.print(" offset=");
    Serial.println(offset);
  } else {
    Serial.println("Send fail:");
  }

  if (byteData == 0xff) {
    byteData = 0;
    offset++;
    if (offset == 32) offset = 0;
  } else {
    byteData++;
  }

  delay(1000);
}
