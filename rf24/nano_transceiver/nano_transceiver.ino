/*
*  This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0.If a copy of the MPL was not distributed with this
* file, You can obtain one at http ://mozilla.org/MPL/2.0/.
*/
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define THUMB_PIN A5
#define INDEX_PIN A7
#define MIDDLE_PIN A6
#define NB_MESSAGE 3
#define SIZE_MESSAGE_BUFFER 20

RF24 radio(7, 8); // CE, CSN

const byte address[6] = "gant0";

String message[NB_MESSAGE];
void read_sensor(void);
void send_message(void);
void motor_control(void);
float filter_signal(float signal_to_filter);
float old_finger_0 = 0;
float old_finger_1 = 0;
float old_finger_2 = 0;
int fingerPins[3]={THUMB_PIN,INDEX_PIN,MIDDLE_PIN};
int CalibrationVals[2][3];   

void setup() {
  Serial.begin(9600);

  pinMode(THUMB_PIN, INPUT);
  pinMode(INDEX_PIN, INPUT);
  pinMode(MIDDLE_PIN, INPUT);
  CalibrateFlexSensors();
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);  // Lowest power
  radio.stopListening();          // Setting in transmiting mode
  float old_finger_0 = analogRead(THUMB_PIN);
  float old_finger_1 = analogRead(INDEX_PIN);
  float old_finger_2 = analogRead(MIDDLE_PIN);
  
}

void loop() 
{
  /* Read the sensors and build strings to send */
  read_sensor();
   
  /* Send the builded strings */
  send_message();
}
//float old_value=0;      // a enelever

//float filter_signal(int signal_to_filter, float filterFrequency){
  // filters out changes faster than specified freq.
  
  // create a one pole (RC) lowpass filter
//  FilterOnePole lowpassFilter( LOWPASS, filterFrequency );  
// float filtered_signal = lowpassFilter.input(signal_to_filter); 
  //old_value = 0.3*old_value + 0.3*((float)signal_to_filter);
  //float filtered_signal = old_value;
 // return filtered_signal;
  
//}


void CalibrateFlexSensors(){
  int calibrationCount=0;
  while(calibrationCount<=1){
    /* If calibrationCount==0, then find minimum flex value (hand relaxed position)
     * Elif calibrationCount==1, then find maximum flex value (hand in fist position)
     */
     if(calibrationCount==0){
      Serial.println("Please lay your hand flat");
      delay(3000);
      Serial.println("Collecting minimum flex readings");
     }
     else if(calibrationCount==1){
      Serial.println("Please close you hand into a fist");
      digitalWrite(LED_BUILTIN, HIGH);
      delay(3000);
      Serial.println("Collecting maximum flex readings");
     }
     for(int i=0; i<3; i++){
      int readingCount=0;
      int readingSum=0;
      while(readingCount<10){                        //Take 10 unique readings
        readingSum+=analogRead(fingerPins[i]);
        readingCount+=1;
        delay(200);
      }      
      CalibrationVals[calibrationCount][i]=readingSum/10;    //Record average of 10 readings
     }
     calibrationCount+=1;
  }
  Serial.println("Calibration Complete");
  digitalWrite(LED_BUILTIN, LOW);
  Serial.println("In the order from thumb to middle, your offsets are:");
  for(int i=0; i<2; i++){
    if(i==0){Serial.print("Minimum Offsets:\t");}
    else{Serial.print("Maximum Offsets:\t");}
    for(int j=0; j<3; j++){
      Serial.print(CalibrationVals[i][j]);
      Serial.print(", ");
    }  
    Serial.println();
  }
}

void read_sensor(void)
{
    

  //int finger_0 = lowpassFilter.input(analogRead(INDEX_PIN)); //Read voltage of the voltage divider of the index
  float finger_0 = analogRead(THUMB_PIN);
  //Serial.println("THUMB_PIN: " + String(finger_0) + "\n");
  //finger_0 = filter_signal(finger_0, 5000);    //filtrage du signal RC a la freq specifiee 
  finger_0 = map(finger_0, CalibrationVals[0][0], CalibrationVals[1][0], 0, 20);
  
  if(finger_0 < old_finger_0+2 && finger_0 > old_finger_0-2){
    finger_0 = old_finger_0;
  }
 
  else{
    old_finger_0 = finger_0;
  }
    
  float finger_1 = analogRead(INDEX_PIN);     //Read voltage of the voltage divider of the index
  //Serial.println("INDEX_PIN: " + String(finger_1) + "\n");
  //finger_1 = filter_signal(finger_1, 5000);    //filtrage du signal RC a la freq specifiee 
  finger_1 = map(finger_1, CalibrationVals[0][1], CalibrationVals[1][1], 0, 20);
  
  if(finger_1 < old_finger_1+1.5 && finger_1 > old_finger_1-1.5){
    finger_1 = old_finger_1;
  }
  else{
    old_finger_1 = finger_1;
  }
  
  float finger_2 = analogRead(MIDDLE_PIN);     //Read voltage of the voltage divider of the major
  //Serial.println("MIDDLE_PIN: " + String(finger_2) + "\n");
  //finger_2 = filter_signal(finger_2, 5000);    //filtrage du signal RC a la freq specifiee 
  finger_2 = map(finger_2, CalibrationVals[0][2], CalibrationVals[1][2], 0, 20);
  
  if(finger_2 < old_finger_2+1.5 && finger_2 > old_finger_2-1.5){
    finger_2 = old_finger_2;
  }
  else{
    old_finger_2 = finger_2;
  }
  /* Add message here and change the NB_MESSAGE to fit the number of strings below */
  message[0] = String("finger_0 " + String(finger_0)  + "\n");
  message[1] = String("finger_1 " + String(finger_1)  + "\n");
  message[2] = String("finger_2 " + String(finger_2)  + "\n");
  Serial.print(message[0]+" "+message[1]+" " + message[2]);
  //message[3] = String("\n");
  //delay(1000);
}

void send_message(void)
{
  int counter =0;
  char message_buff[SIZE_MESSAGE_BUFFER]="";
  
  for(counter=0; counter < NB_MESSAGE; counter++)
  {
    message[counter].toCharArray(message_buff,message[counter].length());
    radio.write(&message_buff, sizeof(message_buff));     // Send over the radio
    memset(message_buff, 0, sizeof(message_buff));        //Clear the buffer
  }
}
