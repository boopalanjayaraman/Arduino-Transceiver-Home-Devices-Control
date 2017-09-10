#include<SPI.h>
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

RH_NRF24 nrf24; //// Singleton instance for the radio driver.

int firstDevicePin = 3;
int secondDevicePin = 4;

char* firstDeviceHigh = "1";
char* firstDeviceLow = "2";
char* secondDeviceHigh = "4";
char* secondDeviceLow = "5";

void setup() {

  Serial.begin(9600);
  while (!Serial)
    ;

  if (!nrf24.init())
  {
    Serial.println("nrf24 rec client init failed");
  }
  if (!nrf24.setChannel(1))
  {
    Serial.println("nrf24 rec client set channel failed");
  }
  if (!nrf24.setRF(RH_NRF24::DataRate2Mbps, RH_NRF24::TransmitPower0dBm))
  {
    Serial.println("nrf24 rec client set RF configs failed");
  }

  pinMode(firstDevicePin, OUTPUT);
  pinMode(secondDevicePin, OUTPUT);
}

void loop() {

  Serial.println("receiving.");
  //if (nrf24.available())
  //{
  uint8_t request[RH_NRF24_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(request);

  if (nrf24.recv(request, &len))
  {
    Serial.println("got request:");
    Serial.println((char*)request);

    //// TODO: Execute command here.
    if (strncmp((const char*)request, firstDeviceHigh, 1) == 0)//"Normal On")
    {
      digitalWrite(firstDevicePin, HIGH);
      Serial.println("firstDevicePin high.");
      //delay(1000);
    }
    else if (strncmp((const char*)request, firstDeviceLow, 1) == 0)//"Normal Off")
    {
      digitalWrite(firstDevicePin, LOW);
      Serial.println("firstDevicePin low.");
    }
    else if (strncmp((const char*)request, secondDeviceHigh, 1) == 0)//"Cfl On")
    {
      digitalWrite(secondDevicePin, HIGH);
      Serial.println("secondDevicePin high.");
      //delay(1000);
    }
    else if (strncmp((const char*)request, secondDeviceLow, 1) == 0)//"Cfl Off")
    {
      digitalWrite(secondDevicePin, LOW);
      Serial.println("secondDevicePin low.");
    }

    uint8_t response[] = "From Rec:Command Executed.";
    nrf24.send(response, sizeof(response));
    nrf24.waitPacketSent();

    Serial.println("sent response:");
    Serial.println((char*)response);
  }
  else
  {
    Serial.println("receive failed.");
  }
  //}
  //else
  //{
  //  Serial.println("nrf24 unavailable.");
  //}
  delay(300);
}
