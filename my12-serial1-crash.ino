
/*
 * my12-serial1-crash.ino For grabbing the Serial1 crash report generated by the Portenta H7
 *
 * Note: Other Arduino must be a 3Volt arduino!
 * GND Portenta to GND other Arduino 
 * TX Portenta to RX other Arduino
 * RX Portenta to TX other Arduino
 *
 * On Portenta H7 RX on pin D13, TX on Pin D14   
 * On Nano 33 BLE RX on pin P1.10 , TX on Pin P1.03 bottom right
 * On Nano 33 IOT RX on pin PB22, TX on Pin PB23 bottom right 
 *
 *
 * You could just purchase a TTL USB cable with the three connections
 * 
 * 
 * The Portenta sends crash information out on the UART TX and RX pins.
 * This program allows you to load a running serial1 monitor on another 3.3V Arduino board
 * I use the program putty to monitor it (radio button serial, find com port and speed 115200)
 * use DOS or CMD command "mode" to find the correct port
 * 
 */

/*  */


void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  Serial1.begin(115200);
  
  //while(!Serial.available()){} // I don't like this but useful today.
  //Serial.println("Hello from the other Arduino");
  digitalWrite(LED_BUILTIN, HIGH);  // flash on-board LED no matter grounding
  delay(400);
  digitalWrite(LED_BUILTIN, LOW);
  delay(400);
  digitalWrite(LED_BUILTIN, HIGH);
}

void loop() {

  if (Serial1.available()) {     // If anything comes in Serial1 (pins 0 & 1)
    Serial.write(Serial1.read());   // read it and send it out Serial (USB)
  }
  
} 





/*
 * lets crash the Portenta !
 * 
 * long comment the above code and uncomment the below code
 * 
 */


/* 

void setup() {
}

void loop() {
  analogWrite(A5,128);
} 

*/
