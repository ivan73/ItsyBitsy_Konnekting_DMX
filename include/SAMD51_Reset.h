/*
 *    This file is part of KONNEKTING Device Library.
 *
 *    The KONNEKTING Device Library is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */



// File : SAMD21G18_Reset.h
// Author: Alexander Christian <info(at)root1.de>
// Description : Code to reset an SAMD21G18 microcontroller, inspired by http://forum.arduino.cc/index.php?topic=366836.0

#ifndef SAMD51_RESET_H
#define SAMD51_RESET_H

#include <Arduino.h>

// keep system alive while wdt is enabled
void resetWDT() {
  // reset the WDT watchdog timer.
  // this must be called before the WDT resets the system
  if (!WDT->SYNCBUSY.bit.CLEAR)                 // Check if the WDT registers are synchronized
  {
	  REG_WDT_CLEAR = WDT_CLEAR_CLEAR_KEY;        // Clear the watchdog timer
  }
}

void systemReset() {
  // use the WDT watchdog timer to force a system reset.
  // WDT MUST be running for this to work
  WDT->CLEAR.reg= 0x00; // system reset via WDT
	while (WDT->SYNCBUSY.reg);
  //while (WDT->STATUS.bit.SYNCBUSY == 1); //Just wait till WDT is free
}

/*
 *initializes WDT period, 
 *                  valid values: 0x0 to 0xB (0-11)
 *                  see 17.18.2 and Table 17-5 in Atmel SAM D21G Datasheet
 */
void setupWDT( uint8_t period) {
  // initialize the WDT watchdog timer
	REG_WDT_CONFIG = WDT_CONFIG_PER_CYC1024;          // Set the WDT reset timeout to 1 second
	REG_WDT_CTRLA = WDT_CTRLA_ENABLE;                 // Enable the WDT in normal mode
	while(WDT->SYNCBUSY.bit.ENABLE);                  // Wait for synchronization
  
}

void disableWDT() {
	WDT->CTRLA.reg = 0; // Disable watchdog for config
}
#endif // SAMD51_RESET_H
