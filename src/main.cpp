/*
ATiny328 Pinout

                                  +--, ,--+
             RESET--PCINT5---PC6--|1  U 28|--PC5--PCINT13--ADC5--19--A5--SCL
           0---RxD--PCINT16--PD0--|2    27|--PC4--PCINT12--ADC4--18--A4--SDA
           1---TxD--PCINT17--PD1--|3    26|--PC3--PCINT11--ADC3--17--A3
           2--INT0--PCINT18--PD2--|4    25|--PC2--PCINT10--ADC2--16--A2
OC2B--PWM--3--INT1--PCINT19--PD3--|5    24|--PC1--PCINT9---ADC1--15--A1
       T0--4---XCK--PCINT20--PD4--|6    23|--PC0--PCINT8---ADC0--14--A0
                             Vcc--|7    22|--GND
                             GND--|8    21|--AREF
       OSC1--XTAL1--PCINT6---PB6--|9    20|--AVcc
       OSC2--XTAL2--PCINT7---PB7--|10   19|--PB5--PCINT5--SCK---13
OC0B--PWM--5----T1--PCINT21--PD5--|11   18|--PB4--PCINT4--MISO--12
OC0A--PWM--6--AIN0--PCINT22--PD6--|12   17|--PB3--PCINT3--OC2A--11--PWM--MOSI
           7--AIN1--PCINT23--PD7--|13   16|--PB2--PCINT2--OC1B--10--PWM--SS
     ICP1--8--CLK0--PCINT0---PB0--|14   17|--PB1--PCINT1--OC1A---9--PWM
                                  +-------+

*/

#include <Arduino.h>
#include <SPI.h>
//define PINs
const int LCDEnabledPin    = 9;  //pin 17 on Atmega328 chip
const int LCDCommandPin    = 10; //pin 16 on ATmega328 chip (SPI !ChipSelect)
//const int LCDSPIClockPin = 13;  //pin 19 on ATmega328 chip (will be defined by SPI.begin)
//const int LCDSPIDataPin  = 11;  //pin 17 on ATmega328 chip (will be defined by SPI.begin)
const int LCDDotClockPin   = 14;  //pin  on ATmega328 chip
const int LCDHSyncPin      = 15;  //pin  on ATmega328 chip
const int LCDVSyncPin      = 16;  //pin  on ATmega328 chip
const int LCDData0Pin      = 0;  //pin  2 on ATmega328 chip
const int LCDData1Pin      = 1;  //pin  3 on ATmega328 chip
const int LCDData2Pin      = 2;  //pin  4 on ATmega328 chip
const int LCDData3Pin      = 3;  //pin  5 on ATmega328 chip
const int LCDData4Pin      = 4;  //pin  6 on ATmega328 chip
const int LCDData5Pin      = 5;  //pin 11 on ATmega328 chip
const int LCDData6Pin      = 6;  //pin 12 on ATmega328 chip
const int LCDData7Pin      = 7;  //pin 13 on ATmega328 chip

int  PixelX = 0;
int  PixelY = 0;
byte ColorRed = 0;
byte ColorGreen = 0;
byte ColorBlue = 0;



const byte DisplayInit[150]={0x00,0x01,0x21,0x03,0xEF,0x03,0x80,0x00,0x81,0x03,0x01,0x02,0x80,0x01,0x21,0x02,0xE1,0x02,0xA0,0x01,0x17,0x02,0x01,0x02,0x60,0x00,0x01,0x02,0x41,0x03,0x60,0x01,0x01,0x02,0xE1,0x03,0xE0,0x00,0x69,0x03,0x01,0x02,0x10,0x01,0x01,0x02,0x01,0x02,0x90,0x00,0x01,0x02,0x35,0x02,0xD0,0x00,0xF3,0x03,0x63,0x02,0xD0,0x01,0x01,0x02,0x41,0x02,0xF0,0x00,0x01,0x02,0x01,0x02,0x28,0x00,0x01,0x02,0xC1,0x02,0x10,0x00,0x01,0x02,0xF7,0x02,0xA8,0x00,0x01,0x03,0xC3,0x03,0x18,0x00,0x41,0x03,0x81,0x02,0x18,0x01,0x01,0x02,0xC1,0x03,0x98,0x00,0x01,0x02,0x01,0x02,0x98,0x01,0x41,0x02,0x01,0x03,0x58,0x00,0xC1,0x03,0xC1,0x03,0x58,0x01,0x01,0x02,0x81,0x02,0xD8,0x00,0x41,0x03,0xC1,0x03,0xD8,0x01,0x01,0x03,0x41,0x02,0xB8,0x00,0x51,0x03,0x41,0x02,0xB8,0x01,0x01,0x03,0x21,0x02};
const byte DisplaySleep[7]={0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06};   //sequenz currently unknown and not shure that they exist
const byte DisplayOff[6]={0x10, 0x01, 0x01, 0x02, 0x01, 0x03};




void setup() {
  //energy safing options
  ADCSRA &= B01111111; //deactivate ADC with bit 7 in the ADCSRA register = ADEN = ADC Enabled ... to reactivate ADCSRA |= B10000000;
  pinMode(0, OUTPUT);  //set all PINs they are not used as output low
  pinMode(1, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(14, OUTPUT);
  pinMode(15, OUTPUT);
  pinMode(16, OUTPUT);
  pinMode(17, OUTPUT);
  pinMode(18, OUTPUT);
  pinMode(19, OUTPUT);

  delay(100);  //delay for stable power supply

  //set the normal output signals
  digitalWrite(LCDEnabledPin,  LOW);
  digitalWrite(LCDDotClockPin, HIGH);
  digitalWrite(LCDHSyncPin,    HIGH);
  digitalWrite(LCDVSyncPin,    HIGH);
  digitalWrite(LCDCommandPin,  HIGH);
 

  SPI.begin();
  // The LCD display in the Canon printer has ha SPI clock rate from nearly 1,666MHz.
  // I have the command sniffed as LSB first. So we must send this as LSB.
  // The clock signal pulse to positive and the data is set after the falling edge and will read on the rising edge. This is SPI_MODE0
  SPI.beginTransaction(SPISettings(1666000, LSBFIRST, SPI_MODE0));


  //enable Display or ChipSelect
  digitalWrite(LCDEnabledPin, HIGH);


  digitalWrite(LCDCommandPin, LOW);   //prepare to sending commands

  //init the LCD Display
  byte j = 0;
  for (byte i = 0; i < 75; i++) {
    SPI.transfer(DisplayInit[j]);
    j++;
    SPI.transfer(DisplayInit[j]);
    j++;
    delay(1);  //between each word is a delay of 1ms
  }

  digitalWrite(LCDCommandPin, HIGH);   //sending commands finished

}

void loop() {
  
  digitalWrite(LCDDotClockPin, LOW);  
  digitalWrite(LCDDotClockPin, HIGH);

  //sending red


  digitalWrite(LCDDotClockPin, LOW);  
  digitalWrite(LCDDotClockPin, HIGH);


  //sending green


  digitalWrite(LCDDotClockPin, LOW);  
  digitalWrite(LCDDotClockPin, HIGH);


  //sending blue


  
  PixelX++;
 

  if (PixelX==318)
  {
    digitalWrite(LCDHSyncPin, LOW);
  }

  if (PixelX == 319)
  {
    digitalWrite(LCDHSyncPin, HIGH);
    PixelY++;
    PixelX=0;

    if (PixelY == 198)
    {
      digitalWrite(LCDVSyncPin, LOW);
    }

    if (PixelY == 199)
    {
      digitalWrite(LCDVSyncPin, HIGH);
      PixelY=0;
    }


  }

  

  

}

