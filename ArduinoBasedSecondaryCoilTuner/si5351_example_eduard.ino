/*
 * si5351_example.ino - Simple example of using Si5351Arduino library
 *
 * Copyright (C) 2015 - 2016 Jason Milldrum <milldrum@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "si5351.h"
#include "Wire.h"

Si5351 si5351;

#define in0   8 /// Digital (user) input 
#define in1   9 /// Digital (user) input 
#define in2   4 /// Digital (user) input 


void setup()
{
  bool i2c_found;

  // Start serial and initialize the Si5351
  Serial.begin(9600);
  i2c_found = si5351.init(SI5351_CRYSTAL_LOAD_8PF, 0, 0);
  if(!i2c_found)
  {
    Serial.println("Device not found on I2C bus!");
  }

  // Set CLK0 to output 14 MHz
  // si5351.set_freq(1400000000ULL, SI5351_CLK0);
  // si5351.set_freq(4000000ULL, SI5351_CLK0);
  si5351.set_freq(3276800ULL, SI5351_CLK0);

  // Set CLK1 to output 175 MHz
  si5351.set_ms_source(SI5351_CLK1, SI5351_PLLB);
  si5351.set_freq_manual(17500000000ULL, 70000000000ULL, SI5351_CLK1);

  // Query a status update and wait a bit to let the Si5351 populate the
  // status flags correctly.
  si5351.update_status();
  delay(500);

  pinMode(in0, INPUT_PULLUP);    // sets the digital pin 7 as input
  pinMode(in1, INPUT_PULLUP);    // sets the digital pin 7 as input
  pinMode(in2, INPUT_PULLUP);    // sets the digital pin 7 as input

}

void loop()
{

  static int prev_val0 = 0 ;
  static int prev_val1 = 0 ;
  static int prev_val2 = 0 ;
  static uint64_t tenMhz = 0 ;
  static uint64_t oneMhz = 0 ;
  static uint64_t hundrenKiloHz = 0 ;


  int val0 = digitalRead(in0);   // read the input pin
  if (val0 == 1 && prev_val0 == 0) {
    prev_val0 = 1;
    tenMhz += 1;
    tenMhz %= 4;
  } else if (val0 == 0){
    prev_val0 = 0;
  }

   int val1 = digitalRead(in1);   // read the input pin
  if (val1 == 1 && prev_val1 == 0) {
    prev_val1 = 1;
    oneMhz += 1;
    oneMhz %= 10;
  } else if (val1 == 0){
    prev_val1 = 0;
  }

  int val2 = digitalRead(in2);   // read the input pin
  if (val2 == 1 && prev_val2 == 0) {
    prev_val2 = 1;
    hundrenKiloHz += 1;
    hundrenKiloHz %= 20;
  } else if (val2 == 0){
    prev_val2 = 0;
  }
  
  uint64_t freq = (uint64_t)tenMhz * 1000000000  + 
                            oneMhz * 100000000  + 
                     hundrenKiloHz * 5000000 ;


  si5351.set_freq(freq, SI5351_CLK0);

  // AD.setFrequency(MD_AD9833::CHAN_0, tenMhz * 1000000 
  //                                  + oneMhz * 100000 
  //                           + hundrenKiloHz * 10000); 
  
  // AD.setMode(MD_AD9833::MODE_SINE);
}
