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
 * - GNGGA
 * Field          Name                      Description
 * hhmmss.ss      UTC Time                  000000.000~235959.999
 * llll.lll       Latitude
 * A              N/S indicator             'N'orth, 'S'outh
 * yyyyy.yyy      Longitude
 * A              E/W indicator             'E'ast, 'W'est           
 * x              GPS quality indicator     0:position fix unavailable, 1:valid position fix, SPS mode, 2:valid position fix, differential GPS mode
 * uu             Satellites Used           00~24
 * v.v            HDOP                      Horizontal dilution of precision, 00.0~99.9
 * w.w            Altitude                  Mean sea level altitude (-9999.9~17999.9) in meter
 * x.x            Geoidal Separation        In meter
 * zzzzz          DGPS Station ID           Differential reference station ID, 0000~1023, NULL when DGPS not used
 * hh             Checksum
 * 
 * - GNRMC
 * Field          Name                      Description
 * hhmmss.sss     UTC time                  000000.000~235959.999
 * x              Status                    'V'=Navigation receiver warning, 'A'=Data Valid
 * llll.lll       Latitude
 * A              N/S indicator             'N'orth, 'S'outh
 * yyyyy.yyy       Longitude
 * A              E/W indicator             'E'ast, 'W'est
 * x.x            Speed over ground         000.0~999.9 knots
 * u.u            Cource over ground        000.0~359.9 degree
 * xxxxxx         UTC Date
 * v              Mode indicator            'N'=Data not valid, 'A'=Autonomous mode, 'D'=Differential mode, 'E'=Estimated (dead reckoning) mode
 * hh             Checksum
 * 
 * reference:
 * 1. https://www.arduino.cc/en/Tutorial/SoftwareSerialExample
 * 2. http://shokudo-teatime.blogspot.jp/2014/02/arduino.html
 * 3. https://learn.sparkfun.com/tutorials/pro-micro--fio-v3-hookup-guide/example-1-blinkies
 * 4. http://shokudo-teatime.blogspot.jp/2015/10/arduinogps.html
 * 5. http://navspark.mybigcommerce.com/content/NMEA_Format_v0.1.pdf
 */
 
#include <SoftwareSerial.h>
#include <String.h>

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(115200); //This pipes to the serial monitor
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // set the data rate for the SoftwareSerial port
  Serial1.begin(9600); //This is the UART, pipes to sensors attached to board
  // GPS moudule set 9600 bps
  
  // シリアル通信のテスト
  Serial.println("Program Setting");
}




//long int Tido = 35431721;
//long int Tkeido = 139317210;

String inputString = "";

String Data[16];

float deg;
long int ido,keido;
float lat, lon, alt;

char moji;

void loop() { // run over and over
  int i=0;
  
  if(Serial1.available())
  {
    moji = Serial1.read();
    if(moji=='$')
    {
      inputString = "";
      i=0;
      while(1)
      {
        moji = Serial1.read();
        if(moji==-1)
        {
          continue;
        }
        else if(moji==',')
        {
          Data[i]=inputString;
          inputString = "";
          if(Data[0]!="GNRMC" && Data[0]!="GNGGA")
          {
            break;
          }
          
          i++;
        }
        else if(moji=='\r' || moji=='\n')
        {
          if(Data[0] == "GNRMC")
          {
            ido = Data[3].substring(5, 9).toInt() + Data[3].substring(0, 4).toInt()*10000;
            keido = Data[5].substring(6, 10).toInt() + Data[5].substring(0, 5).toInt()*10000;
            deg = Data[8].toFloat();

            
            Serial.print(Data[0]);  Serial.print("\t"); //GNRMC
            Serial.print(Data[1]);  Serial.print("\t"); //Unix time
            Serial.print(Data[2]);  Serial.print("\t"); //Status
            Serial.print(ido);      Serial.print("\t"); //Latitude
            Serial.print(Data[4]);  Serial.print("\t"); //N or S
            Serial.print(keido);    Serial.print("\t"); //Longitude
            Serial.print(Data[6]);  Serial.print("\t"); //E or W
            Serial.print(Data[7]);  Serial.print("\t"); //Speed, knot
            Serial.print(deg);      Serial.print("\t"); //degree
            Serial.print(Data[9]);  Serial.print("\t"); //UTC date
            Serial.println(Data[10]); //Serial.print("\t"); //Mode indicator
            
            Serial.println(deg);      //Serial.print("\t"); //degree
          }
          else if(Data[0] == "GNGGA")
          {
            lat = Data[2].toFloat();
            lon = Data[4].toFloat();
            alt = Data[9].toFloat();

            /*
            Serial.print(Data[0]);  Serial.print("\t"); //GNGGA
            Serial.print(Data[1]);  Serial.print("\t"); //Unix time
            Serial.print(Data[2]);  Serial.print("\t"); //Latitude
            Serial.print(Data[3]);  Serial.print("\t"); //N or S
            Serial.print(Data[4]);  Serial.print("\t"); //Longitude
            Serial.print(Data[5]);  Serial.print("\t"); //E or W
            Serial.print(Data[6]);  Serial.print("\t"); //GNSS quality indicator
            Serial.print(Data[7]);  Serial.print("\t"); //Num. of satellites in use
            Serial.print(Data[8]);  Serial.print("\t"); //HDOP
            Serial.print(Data[9]);  Serial.print("\t"); //Altitude
            Serial.print(Data[10]);  Serial.print("\t"); //Geoidal Separation
            Serial.println(Data[11]);  //Serial.print("\t"); //DGPS Station ID
            */
            Serial.print(lat);      Serial.print("\t"); //Latitude
            Serial.print(lon);      Serial.print("\t"); //Longitude
            Serial.print(alt);      Serial.print("\t"); //degree
          }
          
          break;
        }
        else inputString += moji;
      }
    }
  }
}
