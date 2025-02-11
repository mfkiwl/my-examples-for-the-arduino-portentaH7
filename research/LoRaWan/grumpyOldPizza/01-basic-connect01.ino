/* Simple OTAA join for a LoRaWAN or Helium network network
 * Does nothing except connect every hour. No lights no serial print
 * The concept is that you should see some kind of connection on
 * The Helium of TTN network dashboards
 * This example code is in the public domain.
 */

#include "LoRaWAN.h"

const char *devEui = "0101010101010101";
const char *appEui = "0101010101010101";
const char *appKey = "2B7E151628AED2A6ABF7158809CF4F3C";


void setup( void ){

    LoRaWAN.begin(US915);
    LoRaWAN.setSubBand(1);    // 1 for TTN and 2 for Helium I think or is it the other way round ???
    LoRaWAN.setDutyCycle(false);
    LoRaWAN.setAntennaGain(2.0);
    
    LoRaWAN.setSaveSession(true);
    LoRaWAN.joinOTAA(appEui, appKey, devEui);

}

void loop( void ){
  
if (!LoRaWAN.busy()){
        if (!LoRaWAN.linkGateways()){
            
            LoRaWAN.rejoinOTAA();
        }
        
        if (LoRaWAN.joined())  {
               
            LoRaWAN.beginPacket();
            LoRaWAN.write(0xef);
            LoRaWAN.write(0xbe);
            LoRaWAN.write(0xad);
            LoRaWAN.write(0xde);
            LoRaWAN.endPacket();
        }
    }


    delay(30000);  // 1000 = 1 second, then 3660000 = 1 hour
}
