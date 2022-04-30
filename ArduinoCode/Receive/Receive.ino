//Receiver program

#include "Mirf.h"

Nrf24l Mirf = Nrf24l(10, 9);

byte mydata[32];

void setup()
{
  Serial.begin(115200);
  Mirf.spi = &MirfHardwareSpi;
  Mirf.init();
  Mirf.payload = sizeof(mydata);  // Set the payload size
  Mirf.channel = 90;              //Set the used channel
  Mirf.config();

  //Set your own address (receiver address) using 5 characters
  Mirf.setRADDR((byte *)"FGHIJ");  
  
  Serial.println("Listening...");
}

void loop()
{
  if (Mirf.dataReady()) { //When the program is received, the received data is output from the serial port
    Mirf.getData(mydata);
    Serial.println("Got data from nRF24L01");
    for (int i=0;i<32;i++) {
      Serial.print(mydata[i], HEX);
      Serial.print(" ");
    }
    Serial.println();
  }
}
