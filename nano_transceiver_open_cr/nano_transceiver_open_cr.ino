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
//plug CSN, MO, MI on 
const byte address[6] = "gant0";
//int flexsensorRange[2][3]= {{626,417,369},
//                            {439,363,356}};

int flexsensorRange[2][3]= {{20,20,20},
                            {0,0,0}}; 

float finger_to_move[2][3]{{4,5,6},{0,0,0}};

Servo thumb;
Servo INDEX;
Servo middle;
int angles[3];                              //array for storing servo angles
int val;
String finger_value_str = "";
float finger_value = 0;
int finger_id;
int flexPins[] = {A0,A1,A2};

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
  thumb.attach(2);
  INDEX.attach(3);
  middle.attach(4);
}
void loop() {
  if (radio.available()) {
      char text[50] = "";
      radio.read(&text, sizeof(text));
      //Serial.println(text);
      int j = 0;
      finger_id = 0;
      finger_value_str = "";

      for(j=0;text[j]!=';';j++){
        finger_value_str += text[j];
        if(text[j]==','){
          finger_to_move[1][finger_id] = finger_value_str.toInt();
          //Serial.println(finger_to_move[1][finger_id]);
          finger_id += 1;
          finger_value_str = "";
        }
       }
       finger_to_move[1][finger_id] = finger_value_str.toInt();
       Serial.println(finger_id);
       //Serial.println(finger_to_move[1][finger_id]);
  
      finger_to_move[0][0] = 0;
      finger_to_move[0][1] = 1;
      finger_to_move[0][2] = 2;
      
    }
    
    thumb.write(finger_to_move[1][0]*9);
    INDEX.write(finger_to_move[1][1]*9);
    middle.write(finger_to_move[1][2]*9);

    //motor_control(finger_to_move[0][0],finger_to_move[1][0]);
    //motor_control(finger_to_move[0][1],finger_to_move[1][1]);
    //motor_control(finger_to_move[0][2],finger_to_move[1][2]);
}
  void motor_control(int i, float finger){
                          //repeat process for each of the 5 fingers
    /* 
     * The following if and else if pair of statements are because 2 of the servos are orientated in reverse to the other 3 (see youtube video around 5:11 minute mark)
     * As such depending on which servo is being written to the angle may need to be reversed. 
     * The angles depend case by case basis of how you oreintate the servos and which fingers the fishing line goes to, change the code as needed
     */
    //angles[i]=map(finger, flexsensorRange[0][i], flexsensorRange[1][i], 0, 180);
    //if(i == 0 || i == 2){ //                       
    //angles[i]=map(finger, flexsensorRange[0][i], flexsensorRange[1][i], 180, 0);   //maps the value measured from the flex sensor and outputs an angle for the servo within the range finger motion
    angles[i] = finger*9;
    //}
    //else if(i == 1){
    //  angles[i]=map(finger, flexsensorRange[0][i], flexsensorRange[1][i], 0, 180);
    //}
    
    //angles[i]=constrain(angles[i], 0, 180);       //any values above/below the maximum/minimum calibration value are reset to the highest/lowest value within the acceptable range
    //Serial.print(String(angles[i]) + " " + String(i));
    //Serial.print('\n');
      if(i==0){
        thumb.write(angles[i]);             //move servos to set angles
      }
      if(i==1){
        INDEX.write(angles[i]);
      }
      if(i==2){
        middle.write(angles[i]);
    
    }
    
    //Serial.println();
  }
