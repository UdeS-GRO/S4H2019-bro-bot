#define BDPIN_LED_USER_1 22
#define BDPIN_LED_USER_2 23
#define BDPIN_LED_USER_3 24
#define BDPIN_LED_USER_4 25

int led_pin = 13;
char cmd;

void setup() {
  // put your setup code here, to run once:
  pinMode(led_pin, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available()){
    light(Serial.parseInt());
  }
  delay(500);
}

void light(int n){
    if(n == 1){
      digitalWrite(led_pin,HIGH);
      Serial.println("led open");
      delay(1000);
      digitalWrite(led_pin,LOW);
      Serial.println("led closed");
      delay(1000);
    }
}
