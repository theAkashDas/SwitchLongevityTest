#include <Wire.h>
#include <Servo.h>

byte data = 0;
String st, stn, nst = "";
char buff[10];
Servo myservo;
uint32_t count = 0, cycle = 0;
int addr = 0;
int val = 0;
int pb4Status = 0, previousStat = 1;
long stateChange = 0;
bool stopFunction = 0;

void setup()
{
  Wire.begin(); // initialise the connection
  myservo.attach(PB10);
  pinMode(PB3, OUTPUT);
  pinMode(PB4, INPUT_PULLDOWN);
  digitalWrite(PB3, HIGH);
  Serial.begin(115200);
  byte b = i2c_eeprom_read_byte(0x57, 0);
  while (b != 0)
  {
    nst = nst + (char)b;
    Serial.print((char)b); //print content to serial port
    addr++; //increase address
    b = i2c_eeprom_read_byte(0x57, addr); //access an address from the memory
  }
  cycle = nst.toInt();
  count = cycle * 2;
  myservo.write(90);
  delay(100); //add a small delay
}

void loop()
{
  Serial.print("Reading memory i.e the no. of cycles : ");
  int addr = 0; //first address

  // access the first address from the memory
  byte b = i2c_eeprom_read_byte(0x57, 0);

  while (b != 0)
  {
    Serial.print((char)b); //print content to serial port
    addr++; //increase address
    b = i2c_eeprom_read_byte(0x57, addr); //access an address from the memory
  }
  Serial.println(" ");
  Serial.print("PB4 : ");
  pb4Status = digitalRead(PB4);
  Serial.println(pb4Status);
  if (millis() - stateChange > 10000 && !pb4Status)
  {
    stopFunction = 1;
  }
  if (pb4Status)
  {
    stateChange = millis();
  }
  if(!stopFunction)
  {
    {
      myservo.write(0);
      delay(1000);
    }
    {
      myservo.write(90);
      count++;
      Serial.print("Counts : ");
      Serial.print(count);
      if (!(count % 2))
      {
        cycle = count / 2;
        Serial.print("\tON OFF Cycles : ");
        Serial.print(cycle);
      }
      Serial.println();
      stn = String(cycle);
      stn.toCharArray(buff, 10);

      i2c_eeprom_write_page(0x57, 0, (byte*)buff, sizeof(buff));
      
    }
  }
  else
  {
    Serial.println("Switch didnt have continuity for last ten seconds so considering it to be defective");
  }
  delay(1000);
}

void i2c_eeprom_write_byte( int deviceaddress, unsigned int eeaddress, byte data ) {
  int rdata = data;
  Wire.beginTransmission(deviceaddress);
  Wire.write((int)(eeaddress >> 8)); // MSB
  Wire.write((int)(eeaddress & 0xFF)); // LSB
  Wire.write(rdata);
  Wire.endTransmission();
}

// WARNING: address is a page address, 6-bit end will wrap around
// also, data can be maximum of about 30 bytes, because the Wire library has a buffer of 32 bytes
void i2c_eeprom_write_page( int deviceaddress, unsigned int eeaddresspage, byte* data, byte length ) {
  Wire.beginTransmission(deviceaddress);
  Wire.write((int)(eeaddresspage >> 8)); // MSB
  Wire.write((int)(eeaddresspage & 0xFF)); // LSB
  byte c;
  for ( c = 0; c < length; c++)
    Wire.write(data[c]);
  Wire.endTransmission();
}

byte i2c_eeprom_read_byte( int deviceaddress, unsigned int eeaddress ) {
  byte rdata = 0xFF;
  Wire.beginTransmission(deviceaddress);
  Wire.write((int)(eeaddress >> 8)); // MSB
  Wire.write((int)(eeaddress & 0xFF)); // LSB
  Wire.endTransmission();
  Wire.requestFrom(deviceaddress, 1);
  if (Wire.available()) rdata = Wire.read();
  return rdata;
}

// maybe let's not read more than 30 or 32 bytes at a time!
void i2c_eeprom_read_buffer( int deviceaddress, unsigned int eeaddress, byte *buffer, int length ) {
  Wire.beginTransmission(deviceaddress);
  Wire.write((int)(eeaddress >> 8)); // MSB
  Wire.write((int)(eeaddress & 0xFF)); // LSB
  Wire.endTransmission();
  Wire.requestFrom(deviceaddress, length);
  int c = 0;
  for ( c = 0; c < length; c++ )
    if (Wire.available()) buffer[c] = Wire.read();
}
