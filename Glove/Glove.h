/*
*  This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0.If a copy of the MPL was not distributed with this
* file, You can obtain one at http ://mozilla.org/MPL/2.0/.
*/
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

RF24 radio(7, 8); // CE, CSN
const byte address[6] = "gant0";
const int NUMBER_OF_FINGERS = 3;

int flexsensorRange[2][3]= {{20,20,20},	//369
                            {0,0,0}}; 	//356

//function declaration
void attach_motor(void);
void read_radio(void);

Servo thumb;
Servo INDEX;
Servo middle;
int angles[3];                              //array for storing servo angles
int val;
String finger_value_str = "";
float finger_value = 0;
float finger_to_move[2][NUMBER_OF_FINGERS]{{4,5,6},{0,0,0}};
int finger_id;
int flexPins[] = {A0,A1,A2};
int test = 0;

void attach_motor(void) {
  /*radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();*/
  thumb.attach(2);
  INDEX.attach(3);
  middle.attach(4);
}

void read_radio(void) {
  char text[80] = "";
  radio.read(&text, sizeof(text));
  //Serial.println(text);
  int i = 0;
  finger_id = 0;
  finger_value_str = "";

  for(i=0;text[i]!=';';i++){
    finger_value_str += text[i];
    if(text[i]==','){
      finger_to_move[1][finger_id] = finger_value_str.toInt();
      Serial.println(finger_to_move[1][finger_id]);
      finger_id += 1;
      finger_value_str = "";
    }
  }
	
	finger_to_move[0][0] = 0;
	finger_to_move[0][1] = 1;
	finger_to_move[0][2] = 2;
}

  void motor_control(int i,float finger){
    //angles[i] = map(finger, flexsensorRange[0][i], flexsensorRange[1][i], 180, 0);   //maps the value measured from the flex sensor and outputs an angle for the servo within the range finger motion
    angles[i] = finger*9;	// 180 divise par 20 = 9 donc le mapping se fait par bon de 9
    angles[i] = constrain(angles[i], 0, 180);       //any values above/below the maximum/minimum calibration value are reset to the highest/lowest value within the acceptable range
	//if(test <50){
	//	Serial.println("moteur " + String(i) + ":" + String(angles[i]));
	//	test += 1;
	//}
    //for(i=0;i<3;i++){
      if(i==0){
        thumb.write(angles[i]);             //move servos to set angles
      }
      if(i==1){
        INDEX.write(angles[i]);
      }
      if(i==2){
        middle.write(angles[i]);
      }
    //}
  }