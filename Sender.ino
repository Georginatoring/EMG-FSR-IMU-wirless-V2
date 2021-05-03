#include "Adafruit_LC709203F.h" // Bat % lib
Adafruit_LC709203F lc; // bat %


//BLE set up

#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

// ADC libs
#include <Wire.h>
#include <Adafruit_ADS1X15.h>
Adafruit_ADS1115 adc1;
Adafruit_ADS1115 adc2;
Adafruit_ADS1115 adc3;

// IMU
#include <Adafruit_BNO08x.h>

// For SPI mode, we need a CS pin
//#define BNO08X_CS 10
#define BNO08X_INT 9

#define BNO08X_RESET -1

Adafruit_BNO08x  bno08x(BNO08X_RESET);
sh2_SensorValue_t sensorValue;

// OLED 
#include <Adafruit_GFX.h> // OLED lib
#include <Adafruit_SSD1306.h> // OLED  lib

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int count; 


void setup() {
//BLE set up
count = 0;
  Serial.begin(115200);
  SerialBT.begin("ESP32"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");
  
// IMU set up
bno08x.begin_I2C();
bno08x.enableReport(SH2_GAME_ROTATION_VECTOR);
bno08x.enableReport(SH2_RAW_ACCELEROMETER);
bno08x.enableReport(SH2_RAW_GYROSCOPE);
bno08x.enableReport(SH2_RAW_MAGNETOMETER);
  
// bat % set up
lc.begin();  
lc.setPackSize(LC709203F_APA_500MAH); // Bat capacity
lc.setAlarmVoltage(3.8);
 
display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);  


//Display Welcome msg
welcome();

//Pair Bluetoth
BLE();


}

void loop() {
 // if (Serial.available()) {
 //   SerialBT.write(Serial.read());
 // }
  if (SerialBT.read()== '1') {
    while(1)
    sendingData();
  }
  delay(20);



}


// Welcome message
void welcome(){
display.clearDisplay(); 
display.setTextSize(1);
display.setTextColor(SSD1306_WHITE  );
display.setCursor(0,0);

//display.print(lc.cellPercent(), 1);

display.print("       Welcome!                              EMG & FSR & IMU          Sender!                             Version 2.0                                    By: George.B");
display.display();
delay(10000);

}

void BLE(){
display.clearDisplay();
display.setTextSize(1);
//display.setTextColor(SSD1306_WHITE  );
display.setCursor(0,0);
display.print("  Pairing  Bluetooth                          Please Connect     A Device To Capture       The Data                                                       Battery:  "  "   %");
display.setCursor(89,56);
display.print(lc.cellPercent(), 1);
display.display();
delay(10000);


// while loop until connected
//display.clearDisplay();
//display.setTextSize(2);
//display.setTextColor(SSD1306_WHITE  );
//display.setCursor(0,0);
//display.print("Pairing   Successful");
//display.display();
//
//delay(10000);
display.clearDisplay();

  
}

void sendingData(){

  // IMU values
  if (! bno08x.getSensorEvent(&sensorValue)) {
     display.clearDisplay();
     display.setTextSize(1);
     display.setTextColor(SSD1306_WHITE  );
     display.setCursor(0,0);
     display.print("IMU not working");
  }
 
    float    value [13] ; 

    switch (sensorValue.sensorId) {
    
    case SH2_GAME_ROTATION_VECTOR:
      
     //Game Rotation Vector - r: 
       value[0] = sensorValue.un.gameRotationVector.real;
     // i: 
       value[1] = sensorValue.un.gameRotationVector.i;
      // j: 
       value[2] = sensorValue.un.gameRotationVector.j;
     // k: 
       value[3] = sensorValue.un.gameRotationVector.k;
       break;
      
    case SH2_RAW_ACCELEROMETER:
      // Raw Accelerometer - x:
       value[4] =sensorValue.un.rawAccelerometer.x;
      //y: 
       value[5] =sensorValue.un.rawAccelerometer.y;
      // z: 
       value[6] = sensorValue.un.rawAccelerometer.z;
       break;
    case SH2_RAW_GYROSCOPE:
    //Raw Gyro - x:
       value[7] = sensorValue.un.rawGyroscope.x;
     // y: 
       value[8] =sensorValue.un.rawGyroscope.y;
     // z: 
       value[9] =sensorValue.un.rawGyroscope.z;
       break;
    case SH2_RAW_MAGNETOMETER:
     //Raw Magnetic Field - x: 
       value[10] =sensorValue.un.rawMagnetometer.x;
      // y: 
       value[11] =sensorValue.un.rawMagnetometer.y;
      // z: 
       value[12] =sensorValue.un.rawMagnetometer.z;
       break;
  }

     Serial.println(value[12]);

int16_t EMG2, FMG2, EMG3, FMG3, EMG1, FMG1, EMG4, FMG4, EMG5, FMG5, EMG6, FMG6;


// collect EMG & FMG
   adc1.begin(0x48);
  EMG1 = adc1.readADC_SingleEnded(0);
  FMG1 = adc1.readADC_SingleEnded(1);
  EMG4 = adc1.readADC_SingleEnded(2);
  FMG4 = adc1.readADC_SingleEnded(3);
 
 adc2.begin(0x49);
  EMG2 = adc2.readADC_SingleEnded(0);
  FMG2 = adc2.readADC_SingleEnded(1);
  EMG3 = adc2.readADC_SingleEnded(2);
  FMG3 = adc2.readADC_SingleEnded(3);
//
 adc3.begin(0x4B);
  EMG5 = adc3.readADC_SingleEnded(0);
  FMG5 = adc3.readADC_SingleEnded(1);
  EMG6 = adc3.readADC_SingleEnded(2);
  FMG6 = adc3.readADC_SingleEnded(3);

count = count +1; 

// convert values to string
String Scount = String(count);
String SEMG1 = String(EMG1);
String SEMG2 = String(EMG2);
String SEMG3 = String(EMG3);
String SEMG4 = String(EMG4);
String SEMG5 = String(EMG5);
String SEMG6 = String(EMG6);
String SFMG1 = String(FMG1);
String SFMG2 = String(FMG2);
String SFMG3 = String(FMG3);
String SFMG4 = String(FMG4);
String SFMG5 = String(FMG5);
String SFMG6 = String(FMG6);
String Sr = String(value[0]);
String Si = String(value[1]);
String Sj = String(value[2]);
String Sk = String(value[3]);
String Saccx = String(value[4]);
String Saccy = String(value[5]);
String Saccz = String(value[6]);
String SGYRx = String(value[7]);
String SGYRy = String(value[8]);
String SGYRz = String(value[9]);
String SMAGx = String(value[10]);
String SMAGy = String(value[11]);
String SMAGz = String(value[12]);

String sData = Scount + "," + SEMG1 + "," + SEMG2 + "," + SEMG3 + "," + SEMG4 + "," + SEMG5 + "," + SEMG6 + "," + SFMG1 + "," + SFMG2+ "," + SFMG3+ "," + SFMG4+ "," + SFMG5+ "," + SFMG6 + "," + Sr+ "," + Si + "," + Sj + "," + Sk + "," + Saccx + "," +Saccy + "," + Saccz + "," + SGYRx+ "," + SGYRy+ "," +SGYRz + "," +SMAGx+ "," +SMAGy+ "," +SMAGz+ "," + "$" ;    


  if (SerialBT.available()) {
    if (SerialBT.read() == '0'){
      while(SerialBT.read() != '1');
    }
  }
  
// send data through BLE
byte bytes[sData.length() + 1];
sData.getBytes(bytes, sData.length() + 1);
for (int i = 0; i < sData.length(); i++){

    SerialBT.write(bytes[i]);
  }


  // display on screen
display.clearDisplay();
display.setTextSize(1);
display.setTextColor(SSD1306_WHITE  );
display.setCursor(0,0);
display.print("Sending Data:                                                      EMG-FSR-IMU ");
display.setCursor(43,56);
display.print("Battery:  " "  %");
display.setCursor(89,56);
display.print(lc.cellPercent(), 1);
display.display();


}
