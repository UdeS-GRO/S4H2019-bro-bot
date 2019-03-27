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
//int flexsensorRange[2][3]= {{626,417,369},
//                            {439,363,356}};

int flexsensorRange[2][3]= {{20,20,369},
                            {0,0,356}}; 

Servo thumb, index, middle;
int angles[3];                              //array for storing servo angles
int val;
String finger_value_str = "";
float finger_value = 0;
float finger[5];
float finger_id;
int flexPins[] = {A0,A1,A2};

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
  thumb.attach(2);
  index.attach(3);
  middle.attach(4);
}
void loop() {
  //for(int i=0; i<3; i+=1){
    if (radio.available()) {
      char text[50] = "";
      radio.read(&text, sizeof(text));
      int i = int(text[7])-48;
      
      int ii = 9;
      finger_value_str = "";
      while(text[ii] != '\0'){
        finger_value_str += text[ii];
        ii += 1;
      }
      //Serial.println(text);
      //Serial.println('\n');
      //Serial.println(finger_value_str);
      finger_value = finger_value_str.toFloat();
      //Serial.println(finger_value);
      motor_control(finger_value,i);
      //finger_id[id] = finger_value;
      
      //Serial.println("finger value" + finger[0]);
      /*int ii = 0;
      while(text[ii] != "_"){
        Serial.println(text[ii] + "pas de _!");
        ii = ii+1;
        delay(100);
      }
      int jj = ii+2;
      while(text[jj] != '\n'){
        Serial.println("pas de backslash 0!");
        finger_value_str = finger_value_str + text[jj];
        jj = jj+1;
      }
      float finger_value = 0;
      Serial.println("finger value avant conversion" + finger_value_str);
      finger_value = finger_value_str.toFloat();
      Serial.println(finger_value);
      finger[int(text[ii+1])] = finger_value;
      Serial.println(int(text[ii+1]));
      delay(1000);
      */
    }
  //}
  //delay(1000);
}
  void motor_control(float finger, int i){
                          //repeat process for each of the 5 fingers
    /* 
     * The following if and else if pair of statements are because 2 of the servos are orientated in reverse to the other 3 (see youtube video around 5:11 minute mark)
     * As such depending on which servo is being written to the angle may need to be reversed. 
     * The angles depend case by case basis of how you oreintate the servos and which fingers the fishing line goes to, change the code as needed
     */
    //angles[i]=map(finger, flexsensorRange[0][i], flexsensorRange[1][i], 0, 180);
    //if(i == 0 || i == 2){ //                       
    angles[i]=map(finger, flexsensorRange[0][i], flexsensorRange[1][i], 180, 0);   //maps the value measured from the flex sensor and outputs an angle for the servo within the range finger motion
    //}
    //else if(i == 1){
    //  angles[i]=map(finger, flexsensorRange[0][i], flexsensorRange[1][i], 0, 180);
    //}
    
    angles[i]=constrain(angles[i], 0, 180);       //any values above/below the maximum/minimum calibration value are reset to the highest/lowest value within the acceptable range
    Serial.print(String(angles[i]) + " " + String(i));
    Serial.print('\n');

    for(i=0;i<3;i++){
      if(i==0){
        thumb.write(angles[i]);             //move servos to set angles
      }
      if(i==1){
        index.write(angles[i]);
      }
      if(i==2){
        middle.write(angles[i]);
      }
    }
    
    //Serial.println();
  }
