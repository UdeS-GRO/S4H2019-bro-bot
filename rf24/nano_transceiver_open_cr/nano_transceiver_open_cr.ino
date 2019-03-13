
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

RF24 radio(7, 8); // CE, CSN
const byte address[6] = "gant0";
int flexsensorRange[2][1]= {{45},
                            {77}};

Servo index;
int angles[1];                              //array for storing servo angles
int val;
String finger_value_str = "";
float finger_value = 0;
float finger[5];
float finger_id;

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
   index.attach(5);
}
void loop() {
  if (radio.available()) {
    char text[50] = "";
    radio.read(&text, sizeof(text));
    int id = int(text[7])-48;
    
    int ii = 9;
    finger_value_str = "";
    while(text[ii] != '\0'){
      finger_value_str += text[ii];
      ii += 1;
    }
    Serial.println(text);
    Serial.println(finger_value_str);
    finger_value = finger_value_str.toFloat();
    Serial.println(finger_value);
    motor_control(finger_value);
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
}
  void motor_control(float finger){
  for(int i=0; i<1; i+=1){                        //repeat process for each of the 5 fingers
    /* 
     * The following if and else if pair of statements are because 2 of the servos are orientated in reverse to the other 3 (see youtube video around 5:11 minute mark)
     * As such depending on which servo is being written to the angle may need to be reversed. 
     * The angles depend case by case basis of how you oreintate the servos and which fingers the fishing line goes to, change the code as needed
     */
    if(i%2==0){ //                       
      angles[i]=map(finger, flexsensorRange[0][i], flexsensorRange[1][i], 0, 180);   //maps the value measured from the flex sensor and outputs an angle for the servo within the range finger motion
    }
    else if(i%2!=0){
      angles[i]=map(finger, flexsensorRange[0][i], flexsensorRange[1][i], 180, 0);
    }
    
    angles[i]=constrain(angles[i], 0, 180);       //any values above/below the maximum/minimum calibration value are reset to the highest/lowest value within the acceptable range
    Serial.print(angles[i]);
    Serial.print('\t');     
    if(i==0){index.write(angles[i]);}             //move servos to set angles
    }
    Serial.println();
    delay(100);
  }
