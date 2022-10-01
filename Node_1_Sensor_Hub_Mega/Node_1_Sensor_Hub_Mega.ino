// This sketch combines servo turnout control, sensor/ switch inputs 

// Pins 0, 1 and 2 are protected since these are used for communication
// Pin 13 is the Arduino LED pin and should not be used as an input, but can be used as an output for some applications
// Pins 20 and 21 are reserved for PWM servo control

// We will set the Arduino up to behave like a piece of CMRI hardware called a SUSIC with up to 64 slots
// Each slot has either a 24 or 32 bit input/output card
// 64 x 32 bit cards gives up to 2048 input/outputs!
// However, it's best to only set up the SUSIC with the required inputs/outputs to make the process more efficient.
// We will set cards 0 and 1 to be the sensor inputs (up to 64 inputs) 

// Include libraries
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <CMRI.h>
#include <Auto485.h>

// CMRI Settings
#define CMRI_ADDR 1 //CMRI node address in JMRI
#define DE_PIN 2

// The number of servos connected
#define numServos 16

// Define the PCA9685 board addresses
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(); //setup the board address - defaults to 0x40 if not specified

// Setup serial communication
Auto485 bus(DE_PIN); // Arduino pin 2 -> MAX485 DE and RE pins

// Define CMRI connection with 64 inputs and 128 outputs
CMRI cmri(CMRI_ADDR, 64, 128, bus);

// Create tables to hold data about the servo positions
int Status[numServos]; //Create a table to hold the status of each turnout, signal, etc.
int Throw[numServos]; //Create a table to hold the throw value for each servo
int Close[numServos]; //Create a table to hold the close value for each servo
void setup() {

  // SET PINS TO INPUT OR OUTPUT

    for (int i=3; i<20; i++)  {
           pinMode(i, INPUT_PULLUP);       // define sensor shield pins 3 to 19 as inputs
        }
        
    for (int i=22; i<46; i++)  {
           pinMode(i, INPUT_PULLUP);       // define sensor shield pins 22 to 45 as inputs
        }
    
    for (int i=46; i<70; i++)  {
           pinMode(i, INPUT_PULLUP);             // define sensor shield pins 46 to 69 as inputs
        }

  //INITIALISE PCA9685 BOARDS
  
  Serial.begin(19200); //Baud rate of 19200, ensure this matches the baud rate in JMRI, using a faster rate can make processing faster but can also result in incomplete data
  bus.begin(19200);
  pwm.begin();
  pwm.setPWMFreq(50);  // This is the maximum PWM frequency

  //SET THE THROW AND CLOSE VALUES FOR EACH SERVO BASED ON THE CALIBRATION PROCESS
    //Servo connection 0 - point motor
    Throw[0] = 1200;//servo 1 jmri address 1001
    Close[0] = 1600;
    Throw[1] = 1200;//servo 2 jmri address 1002
    Close[1] = 1600;
    Throw[2] = 1350;//servo 3 jmri address 1003
    Close[2] = 1750;
    Throw[3] = 1270;//servo 4 jmri address 1004
    Close[3] = 1670;
    Throw[4] = 1260;//servo 5 jmri address 1005
    Close[4] = 1660;
    Throw[5] = 1325;//servo 6 jmri address 1006
    Close[5] = 1725;
    Throw[6] = 1325;//servo 7 jmri address 1007
    Close[6] = 1725;
    Throw[7] = 1200;//servo 8 jmri address 1008
    Close[7] = 1600;
    Throw[8] = 1350;//servo 9 jmri address 1009
    Close[8] = 1750;
    Throw[9] = 1280;//servo 10 jmri address 1010
    Close[9] = 1680;
    Throw[10] = 1010;//servo 11 jmri address 1011
    Close[10] = 2150;
    Throw[11] = 1200;//servo 12 jmri address 1011
    Close[11] = 1600;
    Throw[12] = 1200;//servo 13 jmri address 1011
    Close[12] = 1600;
    Throw[13] = 1200;//servo 14 jmri address 1011
    Close[13] = 1600;
    Throw[14] = 1200;//servo 15 jmri address 1011
    Close[14] = 1600;
    Throw[15] = 1200;//servo 16 jmri address 1011
    Close[15] = 1600;
}

void loop(){
   cmri.process();

   // PROCESS SERVOS
   // Assume servos start on bit 0 which corresponds to output address 1001
     for (int i = 0; i < numServos; i++) {
         Status[i] = (cmri.get_bit(i));
         if (Status[i] == 1){
             pwm.writeMicroseconds(i, Throw[i]);
             //pwm.writeMicroseconds(i, Midpos[i]);
         }
         else {
             pwm.writeMicroseconds(i, Close[i]);
             //pwm.writeMicroseconds(i, Midpos[i]);
         }
     }

   // PROCESS SENSORS
   // Only include lines that are required. This reduces processing time - delete or comment out lines that are not required
   // Ensure bit address matches pin, i.e. a sensor attached to pin 17 corresponds to bit 13 (because we've skipped pins 0, 1, 2 and 13) which is address 1014 for this CMRI node

      // Do not read 0, 1 or 2 SN EQUALS INFRARED SENSORS
     cmri.set_bit(0, !digitalRead(3));   // S1 = address 1001 in JMRI
     cmri.set_bit(1, !digitalRead(4));   
     cmri.set_bit(2, !digitalRead(5));   
     cmri.set_bit(3, !digitalRead(6));   
     cmri.set_bit(4, !digitalRead(7));   
     cmri.set_bit(5, !digitalRead(8));   
     cmri.set_bit(6, !digitalRead(9));   
     cmri.set_bit(7, !digitalRead(10));  
     cmri.set_bit(8, !digitalRead(11));  
     cmri.set_bit(9, !digitalRead(12));  // S10 = address 1010 in JMRI 
     //DO  NOT READ PIN 13
     cmri.set_bit(10, !digitalRead(14)); // S11 = address 1011 in JMRI 
     cmri.set_bit(11, !digitalRead(15)); 
     cmri.set_bit(12, !digitalRead(16));
     cmri.set_bit(13, !digitalRead(17)); 
     cmri.set_bit(14, !digitalRead(18));
     cmri.set_bit(15, !digitalRead(19)); // S16 = address 1016 in JMRI
     // DO NOT READ PINS 20 21
     cmri.set_bit(16, !digitalRead(22)); // S17 = address 1017 in JMRI
     cmri.set_bit(17, !digitalRead(23));
     cmri.set_bit(18, !digitalRead(24));
     cmri.set_bit(19, !digitalRead(25));
     cmri.set_bit(20, !digitalRead(26));
     cmri.set_bit(21, !digitalRead(27));
     cmri.set_bit(22, !digitalRead(28));
     cmri.set_bit(23, !digitalRead(29));
     cmri.set_bit(24, !digitalRead(30));
     cmri.set_bit(25, !digitalRead(31));
     cmri.set_bit(26, !digitalRead(32));
     cmri.set_bit(27, !digitalRead(33));
     cmri.set_bit(28, !digitalRead(34)); 
     cmri.set_bit(29, !digitalRead(35)); 
     cmri.set_bit(30, !digitalRead(36));
     cmri.set_bit(31, !digitalRead(37)); 
     cmri.set_bit(32, !digitalRead(38));
     cmri.set_bit(33, !digitalRead(39)); 
     cmri.set_bit(34, !digitalRead(40));
     cmri.set_bit(35, !digitalRead(41));
     cmri.set_bit(36, !digitalRead(42)); 
     cmri.set_bit(37, !digitalRead(43));
     cmri.set_bit(38, !digitalRead(44));
     cmri.set_bit(39, !digitalRead(45));
     cmri.set_bit(40, !digitalRead(46));
     cmri.set_bit(41, !digitalRead(47));
     cmri.set_bit(42, !digitalRead(48));
     cmri.set_bit(43, !digitalRead(49));
     cmri.set_bit(44, !digitalRead(50));
     cmri.set_bit(45, !digitalRead(51));
     cmri.set_bit(46, !digitalRead(52));
     cmri.set_bit(47, !digitalRead(53));
     cmri.set_bit(48, !digitalRead(54));
     cmri.set_bit(49, !digitalRead(55));
     cmri.set_bit(50, !digitalRead(56));
     cmri.set_bit(51, !digitalRead(57));
     cmri.set_bit(52, !digitalRead(58));
     cmri.set_bit(53, !digitalRead(59)); 
     cmri.set_bit(54, !digitalRead(60)); 
     cmri.set_bit(55, !digitalRead(61)); 
     cmri.set_bit(56, !digitalRead(62)); 
     cmri.set_bit(57, !digitalRead(63)); 
     cmri.set_bit(58, !digitalRead(64)); 
     cmri.set_bit(59, !digitalRead(65)); 
     cmri.set_bit(60, !digitalRead(66));  
     cmri.set_bit(61, !digitalRead(67)); 
     cmri.set_bit(62, !digitalRead(68)); 
     cmri.set_bit(63, !digitalRead(69)); // S66 = address 1064 in JMRI

      
}
