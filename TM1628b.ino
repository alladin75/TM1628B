const int dataPin = 9;   // VDIO
const int clockPin = 8;  // VCLK
const int strobePin = 7; // VSTR

const int intensity = 2; // €ркость 0-7
const word delay_time=150;

unsigned char s_addr[8] ={
  0x03,0x0b,0x0d,0x07,0x05,0x09,0x0d,0x01};
unsigned char s_byte[8] ={
  0x01,0x01,0x01,0x01,0x01,0x01,0x02,0x01};

// числовые символы 000:00:00 - 123:45:67 - дисплей
const byte  Bit_Mask[7]={0x01,0x02,0x80,0x04,0x08,0x10,0x20}; // маска бита дл€ включени€ соответствующего числового символа
const byte  RAM_Addr[7]={0x00, 0x02, 0x04, 0x06, 0x08, 0x0A, 0x0C}; // адреса в RAM диспле€ дл€ сегментов A-G

const byte Number_Font[16]={0b00111111,0b00000110,0b01011011,0b01001111,0b01100110,0b01101101,0b01111101,0b00000111,
                                0b01111111,0b01101111,0b01110111,0b01111100,0b00111001,0b01011110,0b01111001,0b01110001};

byte  SM1628B_RAM[14]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
byte  Display_Number[7]={0,0,0,0,0,0,0};


void t_sendCommand(byte cmd)
{
  digitalWrite(strobePin, LOW);
  t_send(cmd);
  digitalWrite(strobePin, HIGH);
}

void t_sendData(byte address, byte data)
{
  t_sendCommand(0x44); // запись данных в фиксированную €чейку (без автоинкремента)
  digitalWrite(strobePin, LOW);
  t_send(0xC0 | address);
  t_send(data);
  digitalWrite(strobePin, HIGH);
}

void t_send(byte data)
{
  for (int i = 0; i < 8; i++) {
    digitalWrite(clockPin, LOW);
    digitalWrite(dataPin, data & 1 ? HIGH : LOW);
    data >>= 1;
    digitalWrite(clockPin, HIGH);
  }
}

void SM1628_DVD(boolean Status)
{
  if (Status == true)
  {
    SM1628B_RAM[0x01]=SM1628B_RAM[0x01] | 2;
  } else
  {
    SM1628B_RAM[0x01]=SM1628B_RAM[0x01] ^ 2;
  }
}

void SM1628_VCD(boolean Status)
{
  if (Status == true)
  {
    SM1628B_RAM[0x03]=SM1628B_RAM[0x03] | 2;
  } else
  {
    SM1628B_RAM[0x03]=SM1628B_RAM[0x03] ^ 2;
  }
}

void SM1628_MP3(boolean Status)
{
  if (Status == true)
  {
    SM1628B_RAM[0x05]=SM1628B_RAM[0x05] | 2;
  } else
  {
    SM1628B_RAM[0x05]=SM1628B_RAM[0x05] ^ 2;
  }
}

void SM1628_PBC(boolean Status)
{
  if (Status == true)
  {
    SM1628B_RAM[0x07]=SM1628B_RAM[0x07] | 2;
  } else
  {
    SM1628B_RAM[0x07]=SM1628B_RAM[0x07] ^ 2;
  }
}

void SM1628_Play(boolean Status)
{
  if (Status == true)
  {
    SM1628B_RAM[0x09]=SM1628B_RAM[0x09] | 2;
  } else
  {
    SM1628B_RAM[0x09]=SM1628B_RAM[0x09] ^ 2;
  }
}

void SM1628_Pause(boolean Status)
{
  if (Status == true)
  {
    SM1628B_RAM[0x0B]=SM1628B_RAM[0x0B] | 2;
  } else
  {
    SM1628B_RAM[0x0B]=SM1628B_RAM[0x0B] ^ 2;
  }
}

void SM1628_Dolby(boolean Status)
{
  if (Status == true)
  {
    SM1628B_RAM[0x06]=SM1628B_RAM[0x06] | 64;
  } else
  {
    SM1628B_RAM[0x06]=SM1628B_RAM[0x06] ^ 64;
  }
}

void SM1628_DTS(boolean Status)
{
  if (Status == true)
  {
    SM1628B_RAM[0x08]=SM1628B_RAM[0x08] | 64;
  } else
  {
    SM1628B_RAM[0x08]=SM1628B_RAM[0x08] ^ 64;
  }
}

void SM1628_Repeat(boolean Status)
{
  if (Status == true)
  {
    SM1628B_RAM[0x00]=SM1628B_RAM[0x00] | 64;
  } else
  {
    SM1628B_RAM[0x00]=SM1628B_RAM[0x00] ^ 64;
  }
}

void SM1628_Colon1(boolean Status)
{
  if (Status == true)
  {
    SM1628B_RAM[0x02]=SM1628B_RAM[0x02] | 64;
  } else
  {
    SM1628B_RAM[0x02]=SM1628B_RAM[0x02] ^ 64;
  }
}

void SM1628_Colon2(boolean Status)
{
  if (Status == true)
  {
    SM1628B_RAM[0x04]=SM1628B_RAM[0x04] | 64;
  } else
  {
    SM1628B_RAM[0x04]=SM1628B_RAM[0x04] ^ 64;
  }
}

void SM1628_Disk(byte Status)
{
  if (Status == true)
  {
    SM1628B_RAM[0x04]=SM1628B_RAM[0x04] | 64;
  } else
  {
    SM1628B_RAM[0x04]=SM1628B_RAM[0x04] ^ 64;
  }
}


void Display_Write(byte Dummy)
{
  for (byte i = 0; i <= 0x0D; i++)
  {
    t_sendData(i,SM1628B_RAM[i]);
  }
}

void Display_Clear(void)
{
  for (int i = 0; i <= 0x0D; i++)
  {
    SM1628B_RAM[i]=0;
    t_sendData(i,SM1628B_RAM[i]);
  }
}

// числовые символы 000:00:00 - 123:45:67 - дисплей - отображаемые символы от 0x00 до 0x0F
void ShowNumber(byte Position, byte Value)
{
  byte temp;
//  for (byte i = 0; i < 0x07; i++)
//  {
//   SM1628B_RAM[RAM_Addr[i]]=((SM1628B_RAM[RAM_Addr[i]] | Bit_Mask[Position-1])); 
//   SM1628B_RAM[RAM_Addr[i]]=((SM1628B_RAM[RAM_Addr[i]] ^ Bit_Mask[Position-1])); 
//  }
  temp=Number_Font[Value];
  for (byte i=0; i<0x07; i++)
  {
   if ( ((temp & 0B00000001) == 1 ))
     {
       SM1628B_RAM[RAM_Addr[i]]=SM1628B_RAM[RAM_Addr[i]] | Bit_Mask[Position-1];
     }
     else
     {
       SM1628B_RAM[RAM_Addr[i]]=SM1628B_RAM[RAM_Addr[i]] | Bit_Mask[Position-1];
       SM1628B_RAM[RAM_Addr[i]]=SM1628B_RAM[RAM_Addr[i]] ^ Bit_Mask[Position-1];
     }
     temp=temp>>1;
  }
}




void setup()
{
  Serial.begin(9600);
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(strobePin, OUTPUT);

  digitalWrite(strobePin, HIGH);
  digitalWrite(clockPin, HIGH);

  t_sendCommand(0x40); // установка режима записи с автоинкрементом
  t_sendCommand(0x88 | min(7, intensity));

  // очистка пам€ти
  digitalWrite(strobePin, LOW);
  t_send(0xC0);
  for (int i = 0; i < 16; i++)
  {
    t_send(0x00);
  }
  digitalWrite(strobePin, HIGH);
  Display_Clear;
}



void loop()
{
  SM1628_DVD(true);
  Display_Write(0);
  delay(delay_time);
  SM1628_DVD(false);
  Display_Write(0);
  delay(delay_time);

  SM1628_VCD(true);
  Display_Write(0);
  delay(delay_time);
  SM1628_VCD(false);
  Display_Write(0);
  delay(delay_time);

  SM1628_MP3(true);
  Display_Write(0);
  delay(delay_time);
  SM1628_MP3(false);
  Display_Write(0);
  delay(delay_time);

  SM1628_Play(true);
  Display_Write(0);
  delay(delay_time);
  SM1628_Play(false);
  Display_Write(0);
  delay(delay_time);

  SM1628_Pause(true);
  Display_Write(0);
  delay(delay_time);
  SM1628_Pause(false);
  Display_Write(0);
  delay(delay_time);

  SM1628_PBC(true);
  Display_Write(0);
  delay(delay_time);
  SM1628_PBC(false);
  Display_Write(0);
  delay(delay_time);

  SM1628_Repeat(true);
  Display_Write(0);
  delay(delay_time);
  SM1628_Repeat(false);
  Display_Write(0);
  delay(delay_time);

  SM1628_DTS(true);
  Display_Write(0);
  delay(delay_time);
  SM1628_DTS(false);
  Display_Write(0);
  delay(delay_time);

  SM1628_Dolby(true);
  Display_Write(0);
  delay(delay_time);
  SM1628_Dolby(false);
  Display_Write(0);
  delay(delay_time);

  SM1628_Colon1(true);
  Display_Write(0);
  delay(delay_time);
  SM1628_Colon1(false);
  Display_Write(0);
  delay(delay_time);

  SM1628_Colon2(true);
  Display_Write(0);
  delay(delay_time);
  SM1628_Colon2(false);
  Display_Write(0);
  delay(delay_time);

  for (byte i=1; i <=7; i++)
  {
    for (byte j=0; j <=16; j++)
    {
        ShowNumber(i,j);
        Display_Write(0);
        delay(150);     
    }
  }
}?
