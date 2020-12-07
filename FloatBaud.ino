#define RXpin PB3
#define TXpin PB4

uint8_t data[256];
uint32_t bit_time = 580000; // Bit length in microseconds, instead of setting baudrate.
// Max Baud is 38400. Min Baud is limited by overflow micros() and delayMicroseconds().  Baud = 0.1 Tested.
#define  baudRate 1.7543859f
//#define  baudRate 38400.0f
//#define  baudRate 1.0f

// bit_time = 1 / baud_rate
//At 9600, bit time is 104.166666666666 microseconds.
//At 19200, bit time is 52.083333333333 microseconds.
//If you want to calculate word timing, then you first need to know baud rate, data bits, parity and stop bits.
//For example 9600 8 N 1 uses 10 bits per word (1 start bit, 8 data bits, and 1 stop bit). Each word would take 10/9600 = 1041.66666666 microsecs.

uint8_t enter = 0x0d;

void setup() {
  Serial.begin(230400);
  Serial.setTimeout(1);
  pinMode(RXpin, INPUT);   // sets the digital pin as input
  pinMode(TXpin, OUTPUT);  // sets the digital pin as output

  delay(3000); // for sync with command port window on STM32duino core
#ifdef baudRate
  bit_time = (uint32_t)(1000000 / (baudRate)); //calc. a bit time if baudRate defined
#endif
  Serial.print("bit time = "); Serial.print(bit_time);  Serial.println(" microseconds");
}
void loop() {
  uart_recv(data, 1, bit_time); // read 1 byte from Rx pin and then prints it to PC terminal (reading and printing 1 byte at time may not work on high bauds, timeout is needed)
  for ( int i = 0; i < 1; i++ )
    Serial.write(data[i]);
  sendfromserial();             // read massage from serial and write it to Tx pin 
}

void uart_snd(char *data, uint16_t len, uint32_t bit_length) //  lets send some word's (8N1) 
{
  for ( int i = 0; i < len ; i++ ) {
    txFunc(data[i], bit_length);
  }
  txFunc(enter, bit_length);
}

void txFunc(uint8_t wordtosend, uint32_t bit_length)  // lest send some bit's
{
  digitalWrite(TXpin, LOW); delayMicroseconds(bit_length - 1);
  for ( int i = 0; i < 8 ; i++ ) {
    digitalWrite(TXpin, getBit(wordtosend, i));
    delayMicroseconds(bit_length - 1);
  }
  digitalWrite(TXpin, HIGH);
  delayMicroseconds(bit_length - 1);
}

bool getBit(unsigned char byte, int position) // convert word position to bit
{
  return (byte >> position) & 0x1;
  //  getBit(b,7) << getBit(b,6) << getBit(b,5) << getBit(b,4) << getBit(b,3) << getBit(b,2) << getBit(b,1) << getBit(b,0)
}



void sendfromserial() {
  if (Serial.available() > 0)
  {
    String str = "";
    str = Serial.readStringUntil('\n');
    char charArr[1000];
    str.toCharArray(charArr, str.length());      // form a character array from received string
    Serial.println();
    for ( int i = 0; i < strlen(charArr) ; i++ ) // send sentence back to terminal to be sure STM32 get it.
      Serial.print(charArr[i]);
    Serial.println();
    uart_snd( charArr, strlen(charArr), bit_time); // send character array to Tx pin
  }
}

void uart_recv( uint8_t *data, uint32_t len, uint32_t bit_length) // 8n1
{
  for ( int i = 0; i < len; i++ )
  {
    uint8_t b[8] = { 0 };
    uint32_t c = 0;
    uint32_t add = 0;
    uint32_t bit_lengthdiv = (uint32_t) bit_length / 2;
    uint8_t startbit = 1;
    uint32_t t = micros();
    uint32_t t2 = micros();

    // wait for start bit
    while (startbit) {
      if (digitalRead(RXpin))
      {
        //        if (micros() - t2 > bit_length * 2) // an attempt to create a timeout feature
        //          return i;
        t = micros();
      }
      if (micros() - t >= bit_lengthdiv ) { // does not response for higher bauds and any short impulses
        startbit = 0;
      }
    }

    // check for 8 data bits
    for ( int j = 0; j < 8; j++ )
    {
      delayMicroseconds(bit_length - 1);
      b[j] =  digitalRead( RXpin );
    }

    // make some word
    for ( int j = 0; j < 8; j++ )
    {
      c |= (b[j] << j);
    }

    // Write word to array
    data[i] = c;

    // wait for stop bit
    //    t = micros();
    //    bool bitd = 1;
    while ( digitalRead( RXpin) );
    //          {
    //            if (!)
    //              bitd = 0;
    //            if (micros() - t > bit_length * 2)  // an attempt to create a timeout feature
    //              return i;
    //          }
  }
}
