//Code for the master recieving data from the 
// MJS_soil_moisture prototype slave device

#include <Wire.h>

//bytes that are expected
const int byte_number = 4;
uint8_t lev=0;
uint8_t levels=6;
uint8_t rec_buf[byte_number];

union {
  uint8_t b[4];
  float f;
} av;

union {
  uint8_t b[4];
  float f;
} st;

union {
  uint8_t b[2];
  uint16_t i;
} T;

//I2C pins
//int Sda = 21; //I2C
//int Scl = 22; //I2C

int c;
int samples=10;

void setup() {
  //Begin serial
  Serial.begin(9600); // start serial for output

  //Start wire connection
  Wire.begin(); // join i2c bus (SDA,SCL)
  Wire.setClock(100000);
}

void loop() {

  send_reg();
  delay(10000);
  
  //measure capacity
  measure_cap();

}

void send_reg(){
  Serial.print("Request measurement of level "+String(lev)+": ");
  Wire.beginTransmission(0x04);
  Wire.write(lev);
  int check=Wire.endTransmission();
  
  lev++;
  if(lev>=levels){
    lev=0;
  }
  
  if(check==0){
    Serial.print("Acknowledged\n");
  }
  else{
    Serial.print("Error\n");    
  }
}


void readSoilMoisture(int lev){

  //bytes that are expected
  const int byte_number = 4;
  uint8_t rec_buf[byte_number];

  union {
    uint8_t b[2];
    uint16_t i;
  } C;

  uint8_t T;

  //start I2C communication
  Wire.beginTransmission(0x04);
  Wire.write(lev);
  int check=Wire.endTransmission();
  
  if(check==0){
    Serial.print(F("Ack"));
    //wait for sensor
    delay(10000);

    //request measurement result
    Wire.requestFrom(0x04, byte_number, true); // request 1 byte from slave device address 4

    int i = 0;
    while (Wire.available()) {
      rec_buf[i]=Wire.read();
      i++;
    }

    //assign bytes to values
    C.b[0]=rec_buf[1];
    C.b[1]=rec_buf[2];
    T=rec_buf[3];

    //checksum if the expected number of bytes are recieved
    if (i = byte_number) {
      Serial.print(F("Suc"));
      l[lev] = C.i;
      t[lev] = T;      
    }
    
    else {
      //invalid number of bytes recievd or no data
      Serial.print(F("Er"));
      l[lev] = 0;
      t[lev] = 0;
    }
  }
  
  else{
    Serial.println("Er");
    l[lev] = 0;
    t[lev] = 0;    
  }

}
