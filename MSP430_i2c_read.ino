#include <Wire.h>
#define OPT3001_ADDRESS 0x44

unsigned int cycle = 0;

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);
  Wire.begin(); // No args -> Master mode.

  Serial.println("======= Initial DATA =======");

  read_register(0x01, "Config : ");
  set_register(0xC810, 0x01, "Reconfig : "); //C = Auto range // 1 00 0 // 0001 // 0 0 00
  read_register(0x02, "Low Limit : ");
  read_register(0x03, "High Limit : ");
  read_register(0x7E, "Manufacter ID : ");
  read_register(0x7F, "Device ID : ");

  // Begin to read results
  Wire.beginTransmission(OPT3001_ADDRESS);
  Wire.write(0x00);
  Wire.endTransmission();
}

void loop() {
  cycle = cycle+1;
  Serial.print("========= Cycle "); Serial.print(cycle); Serial.println(" =========");


  set_register(0xCA10, 0x01, "Reconfig : "); //C = Auto range // 1 01 0 // 0001 // 0 0 00 Single Shot
  unsigned int sensor_data = read_register(0x00, "Light(lux) : ");

  double lux = data_to_value(sensor_data);
    
  Serial.print("Light intensity:");
  Serial.print(lux);
  Serial.println(" ");
  
  delay(1000);
  Serial.println(" ");
}

unsigned int get_word(void){
    Wire.requestFrom(OPT3001_ADDRESS, 2); // Args : address, data lengths(byte)
  // OPT3001 Addr : 1000100(GND), needs 2 bytes reading
  
    unsigned char MSB = Wire.read();
    //Serial.print("MSB : ");
    //Serial.println(MSB,HEX);
    
    unsigned char LSB = Wire.read();
    //Serial.print("LSB : ");
    //Serial.println(LSB,HEX);
    
    unsigned int x = (MSB << 8) | LSB;
    return x;
}

unsigned int read_register(char reg, char* str){
  Wire.beginTransmission(OPT3001_ADDRESS);
  Wire.write(reg);
  Wire.endTransmission();
  
  unsigned int x = get_word();
    
  Serial.print(str);
  Serial.print(x,HEX);
  Serial.println(" ");

  return x;
}

void set_register(int data, char reg, char* str){
  // reg = target register
  
  Wire.beginTransmission(OPT3001_ADDRESS);
  Wire.write(reg);

  unsigned char MSB = (data & 0xFF00) >> 8;
  //Serial.print("MSB :"); Serial.println(MSB,HEX);

  unsigned char LSB = data & 0x00FF;
  //Serial.print("LSB :"); Serial.println(LSB,HEX);
  
  Wire.write(MSB);
  Wire.write(LSB);
  Wire.endTransmission();

  Serial.print(str); Serial.println(get_word(),HEX);
  
}

double data_to_value(unsigned int data){
  unsigned char exponent = (data & 0xF000) >> 12;
  unsigned char frac = data & 0x0FFF;

  double val = weight_from(exponent)*frac;

  return val;
}

double weight_from(unsigned char exponent){
  double weight = 0.01;
  for (;exponent>0;exponent--){
    weight *= 2;
  }
  return weight;
}
