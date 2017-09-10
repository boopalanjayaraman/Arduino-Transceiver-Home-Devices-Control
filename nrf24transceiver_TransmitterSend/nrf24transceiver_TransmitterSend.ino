#include <SPI.h>
#include <RadioHead.h>
#include <radio_config_Si4460.h>
#include <RHCRC.h>
#include <RHDatagram.h>
#include <RHGenericDriver.h>
#include <RHGenericSPI.h>
#include <RHHardwareSPI.h>
#include <RHMesh.h>
#include <RHNRFSPIDriver.h>
#include <RHReliableDatagram.h>
#include <RHRouter.h>
#include <RHSoftwareSPI.h>
#include <RHSPIDriver.h>
#include <RHTcpProtocol.h>
#include <RH_ASK.h>
#include <RH_NRF24.h>
#include <RH_NRF51.h>
#include <RH_NRF905.h>
#include <RH_RF22.h>
#include <RH_RF24.h>
#include <RH_RF69.h>
#include <RH_RF95.h>
#include <RH_Serial.h>
#include <RH_TCP.h>

//// This is the server code. This issues the command.

RH_NRF24 nrf24; //// Singleton of nrf24 driver

void setup() {

  Serial.begin(9600);
  while (!Serial)
    ;

  if (!nrf24.init())
  {
    Serial.println("initialize failed");   // Defaults after init are 2.402 GHz (channel 2), 2Mbps, 0dBm
  }

  if (!nrf24.setChannel(1))
  {
    Serial.println("setting channel failed");
  }

  if (!nrf24.setRF(RH_NRF24::DataRate2Mbps, RH_NRF24::TransmitPower0dBm))
  {
    Serial.println("setting RF defaults failed.");
  }

}

void loop() {

  Serial.println("Sending.");

//  if (nrf24.available())
//  {
//    Serial.println("nrf24 available.");
//  }

  if (Serial.available() > 0)
  {
    int val = Serial.parseInt();

    if (val == 1)
    {
      Serial.println("val 1.");
      uint8_t data[] = "1";//"Normal On";
      sendAndReceiveData(data);
    }
    else if (val == 2)
    {
      uint8_t data[] = "2";//"Normal Off";
      sendAndReceiveData(data);
    }
    else if (val == 4)
    {
      uint8_t data[] = "4";//"Cfl On";
      sendAndReceiveData(data);
    }
    else if (val == 5)
    {
      uint8_t data[] = "5";//"Cfl Off";
      sendAndReceiveData(data);
    }
  }

  delay(500);
}

void sendAndReceiveData(uint8_t data[])
{
  Serial.println("data to send:");
  Serial.println((char*)data);
  
  nrf24.send(data, sizeof(data));
  nrf24.waitPacketSent();

  uint8_t response[RH_NRF24_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(response);

  if (nrf24.waitAvailableTimeout(1000))
  {
    if (nrf24.recv(response, &len))
    {
      Serial.println("Action executed.");
      Serial.println((char*)response);
    }
    else
    {
      Serial.println("Receiving failed.");
    }
  }
  else
  {
    Serial.println("Receiving timout passed. No reply.");
  }
}

