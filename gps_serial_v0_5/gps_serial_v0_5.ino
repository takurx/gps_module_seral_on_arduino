/*
 * GPS module serial and Arduino pro micro test V0.5
 * takuzirra@twitter
 * mail: taku3wzr@gmail.com
 * 
 * Arduino pro micro   -  GPS Module
 * GND                 -  GND      (P1)
 * VCC 3.3V            -  VCC 3.3V (P2)
 * GND                 -  GND      (P3)
 * RXD P0              -  TXD      (P4)
 * TXD P1              -  RXD      (P5)
 * 
 * reference:
 * 1. https://www.arduino.cc/en/Tutorial/SoftwareSerialExample
 * 2. http://shokudo-teatime.blogspot.jp/2014/02/arduino.html
 * 3. https://learn.sparkfun.com/tutorials/pro-micro--fio-v3-hookup-guide/example-1-blinkies
 */
 
#include <SoftwareSerial.h>
#include <String.h>

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(115200); //This pipes to the serial monitor
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }


  Serial.println("Goodnight moon!");

  // set the data rate for the SoftwareSerial port
  Serial1.begin(9600); //This is the UART, pipes to sensors attached to board
  // GPS moudule set 9600 bps
}



char character;
  
void loop() { // run over and over
  String content = "";

  /* Take out one line from Serial output */
  while(1) {
      character = Serial1.read();
      if(character == -1)
      {
        continue;
      }
      content = (content + character);
      if(character == '\r')
      {
        break;
      }
  }

  /* Output one line */
  Serial.println(content);
}
