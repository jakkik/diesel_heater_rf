/*
 * 
 *    ESP32         CC1101
 *    -----         ------
 *    4   <-------> GDO2
 *    18  <-------> SCK
 *    3v3 <-------> VCC
 *    23  <-------> MOSI
 *    19  <-------> MISO
 *    5   <-------> CSn
 *    GND <-------> GND
 * 
 */

#include "DieselHeaterRF.h"

#define HEATER_POLL_INTERVAL  4000

uint32_t heaterAddr = 0x56d24eae; // Heater address is a 32 bit unsigned int. Use the findAddress() to get your heater's address.

DieselHeaterRF heater;
heater_state_t state;

void setup() {

  Serial.begin(115200);

  heater.begin(heaterAddr);

  Serial.println("Started pairing, press and hold the pairing button on the heater's LCD panel...");

  uint32_t address = heater.findAddress(60000UL);

  if (address) {
    heaterAddr = address;
    Serial.print("Got address: ");
    Serial.println(address, HEX);
    // Store the address somewhere, eg. NVS
  } else {
    Serial.println("Failed to find a heater");   
    while(1) {}
  }
  
}

void loop() {

  heater.sendCommand(HEATER_CMD_WAKEUP, heaterAddr, 10);

  if (heater.getState(&state, 1000)) {
    Serial.printf("State: %d, Power: %d, Voltage: %f, Ambient: %d, Case: %d, Setpoint: %d, PumpFreq: %f, Auto: %d, RSSI: %d\n", state.state, state.power, state.voltage, state.ambientTemp, state.caseTemp, state.setpoint, state.pumpFreq, state.autoMode, state.rssi); 
  } else {
    Serial.println("Failed to get the state");
  }
  
  delay(HEATER_POLL_INTERVAL);

}