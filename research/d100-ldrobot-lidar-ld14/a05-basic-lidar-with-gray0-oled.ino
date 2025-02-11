/*
 * RoboPeak RPLIDAR Arduino Example
 * This example shows the easy and common way to fetch data from an RPLIDAR
 * 
 * You may freely add your application code based on this template
 *
 * USAGE:
 * ---------------------------------
 * 1. Download this sketch code to your Arduino board
 * 2. Connect the RPLIDAR's serial port (RX/TX/GND) to your Arduino board (Pin 0 and Pin1)
 * 3. Connect the RPLIDAR's motor ctrl pin to the Arduino board pin 3 
 */
 
/* 
 * Copyright (c) 2014, RoboPeak 
 * All rights reserved.
 * RoboPeak.com
 *
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, 
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, 
 *    this list of conditions and the following disclaimer in the documentation 
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY 
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES 
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR 
 * TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 LDrobot https://www.ldrobot.com/download/en/124
 datasheet at
 https://github.com/ldrobotSensorTeam/DeveloperKit/blob/master/D100Kit.md
 
 D100 red wire to +5V
 Yellow wire to GND
 Green wire to RX1 D13 on Portenta
 black wire strangely to D3 as it is the motor PWM control.
 
 
 
 */
 
// This sketch code is based on the RPLIDAR driver library provided by RoboPeak
#include <RPLidar.h>

// You need to create an driver instance 
RPLidar lidar;

#define RPLIDAR_MOTOR 3 // The PWM pin for control the speed of RPLIDAR's motor.
                        // This pin should connected with the RPLIDAR's MOTOCTRL signal 





#include <Adafruit_SSD1327.h>


// Used for software SPI
#define OLED_CLK D9
#define OLED_MOSI D8

// Used for software or hardware SPI
#define OLED_CS D7
#define OLED_DC D6

// Used for I2C or SPI
#define OLED_RESET -1

// software SPI
//Adafruit_SSD1305 display(128, 64, OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);
// hardware SPI
Adafruit_SSD1327 display(128, 128, &SPI, OLED_DC, OLED_RESET, OLED_CS);

// I2C
//Adafruit_SSD1327 display(128, 128, &Wire, OLED_RESET, 1000000);



                        
void setup() {
  // bind the RPLIDAR driver to the arduino hardware serial1
  lidar.begin(Serial1);
  Serial.begin(115200);
  //while (! Serial) delay(100);  // blocking, messes up beginners
  // set pin modes
  pinMode(RPLIDAR_MOTOR, OUTPUT);

  //Serial.println("SSD1327 OLED test");
  
  if ( ! display.begin(0x3D) ) {
     Serial.println("Unable to initialize OLED");
     while (1) yield();
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextWrap(false);
  display.setTextColor(SSD1327_WHITE);
  display.setCursor(0,0);
  display.display();
}







void loop() {


   // display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Rocksetta D100 Lidar");

  if (IS_OK(lidar.waitPoint())) {
    float distance = lidar.getCurrentPoint().distance; // distance value in mm unit
    float angle    = lidar.getCurrentPoint().angle;    // angle value in degree
    bool  startBit = lidar.getCurrentPoint().startBit; // whether this point is belong to a new scan
    byte  quality  = lidar.getCurrentPoint().quality;  // quality of the current measurement
    
    int myX = distance*cos(angle);
    int myY = distance*sin(angle);

    int   myX1Map = map((int)myX, -3000, 3000, 0, 127);  // should be -8000 to 8000 but I just want the close stuff
    int   myY1Map = map((int)myY, -3000, 3000, 0, 127);

    //perform data processing here... 
  Serial.print(myX1Map);  
  Serial.print(", ");  
  Serial.print(myY1Map);  
  Serial.print(";  ");  

  if (startBit) {
     Serial.println();  
  }




  display.drawPixel(myX1Map, myY1Map, quality);
  display.display();



    
  } else {
    analogWrite(RPLIDAR_MOTOR, 0); //stop the rplidar motor
    
    // try to detect RPLIDAR... 
    rplidar_response_device_info_t info;
    if (IS_OK(lidar.getDeviceInfo(info, 100))) {
       // detected...
       lidar.startScan();
       
       // start motor rotating at max allowed speed
       analogWrite(RPLIDAR_MOTOR, 255);
       delay(1000);
    }
  }
}
