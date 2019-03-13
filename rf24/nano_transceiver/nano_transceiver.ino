
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Filters.h>

#define THUMB_PIN A1
#define INDEX_PIN A7
#define MAJOR_PIN A3
#define NB_MESSAGE 1
#define SIZE_MESSAGE_BUFFER 20

RF24 radio(7, 8); // CE, CSN

const byte address[6] = "gant0";

String message[NB_MESSAGE];
void read_sensor(void);
void send_message(void);
float filter_signal(float signal_to_filter);
float old_finger_0 = 0;

void setup() {
  Serial.begin(9600);

  pinMode(THUMB_PIN, INPUT);
  pinMode(INDEX_PIN, INPUT);
  pinMode(MAJOR_PIN, INPUT);

  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);  // Lowest power
  radio.stopListening();          // Setting in transmiting mode
  float old_finger_0 = analogRead(INDEX_PIN);

}
void loop() 
{
  /* Read the sensors and build strings to send */
  read_sensor();
  Serial.println(message[0]);
  
  /* Send the builded strings */
  send_message();
  
  delay(100);
}
float old_value=0;      // a enelever

float filter_signal(int signal_to_filter, float filterFrequency){
  // filters out changes faster than specified freq.
  
  // create a one pole (RC) lowpass filter
  FilterOnePole lowpassFilter( LOWPASS, filterFrequency );  
  float filtered_signal = lowpassFilter.input(signal_to_filter); 
  //old_value = 0.3*old_value + 0.3*((float)signal_to_filter);
  //float filtered_signal = old_value;
  return filtered_signal;
  
}

void read_sensor(void)
{
    

  //int finger_0 = lowpassFilter.input(analogRead(INDEX_PIN)); //Read voltage of the voltage divider of the index
  float finger_0 = analogRead(INDEX_PIN);
  finger_0 = filter_signal(finger_0, 5000);    //filtrage du signal RC a la freq specifiee 
  //finger_0 = map(finger_0, 75, 50, 20, 0);
  
  if(finger_0 < old_finger_0+1.5 && finger_0 > old_finger_0-1.5){
    finger_0 = old_finger_0;
  }
  else{
    old_finger_0 = finger_0;
  }
  
  
  
  
  int finger_1 = analogRead(INDEX_PIN);     //Read voltage of the voltage divider of the index
  int finger_2 = analogRead(MAJOR_PIN);     //Read voltage of the voltage divider of the major

  /* Add message here and change the NB_MESSAGE to fit the number of strings below */
  message[0] = String("finger_0 " + String(finger_0)  + "\n");
  //message[1] = String("finger_1 " + String(finger_1)  + "\n");
  //message[2] = String("finger_2 " + String(finger_2)  + "\n");
  //message[3] = String("\n");
  //radio.write(&mess, sizeof(mess));


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
