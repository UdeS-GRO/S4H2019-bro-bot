
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define THUMB_PIN A1
#define INDEX_PIN A2
#define MAJOR_PIN A3
#define NB_MESSAGE 4
#define SIZE_MESSAGE_BUFFER 20

RF24 radio(7, 8); // CE, CSN

const byte address[6] = "gant0";

String message[NB_MESSAGE];
void read_sensor(void);
void send_message(void);

void setup() {
  Serial.begin(9600);

  pinMode(THUMB_PIN, INPUT);
  pinMode(INDEX_PIN, INPUT);
  pinMode(MAJOR_PIN, INPUT);

  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);  // Lowest power
  radio.stopListening();          // Setting in transmiting mode
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


void read_sensor(void)
{
  int finger_0 = analogRead(THUMB_PIN);     //Read voltage of the voltage divider of the thumb
  int finger_1 = analogRead(INDEX_PIN);     //Read voltage of the voltage divider of the index
  int finger_2 = analogRead(MAJOR_PIN);     //Read voltage of the voltage divider of the major

  /* Add message here and change the NB_MESSAGE to fit the number of strings below */
  message[0] = String("finger_0 " + String(finger_0)  + "\n");
  message[1] = String("finger_1 " + String(finger_1)  + "\n");
  message[2] = String("finger_2 " + String(finger_2)  + "\n");
  message[3] = String("\n");
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

