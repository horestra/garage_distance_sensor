#include <EEPROM.h>

#define trigPin 13
#define echoPin 12
#define redled 11
#define greenled 10
#define uppin 7
#define downpin 9
#define setPin 8

int addr = 31;
long refvalue = 20;
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 500;    // the debounce time; increase if the output flickers

unsigned long lastDebounceTimeUP = 0;  // the last time the output pin was toggled
unsigned long debounceDelayUP = 50;    // the debounce time; increase if the output flickers

void setup() {
  Serial.begin (9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(redled, OUTPUT);
  pinMode(greenled, OUTPUT);
  pinMode(setPin, INPUT);
  pinMode(downpin, INPUT);
  pinMode(uppin, INPUT);
  refvalue = EEPROM.read(addr);
}

void loop() {
  long duration, distance, tzero, timepressed;
  //This is where the sensor works  
  digitalWrite(trigPin, LOW);  // Added this line
  delayMicroseconds(2); // Added this line
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); // Added this line
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;

  //This is where the leds work
  if (distance < refvalue) {  // This is where the LED On/Off happens
    lastDebounceTime = millis();
    if ((millis() - lastDebounceTime) > debounceDelay) {
      digitalWrite(redled,HIGH); // When the Red condition is met, the Green LED should turn off
      digitalWrite(greenled,LOW);
      //tone(5,880);  
    }
  }
  else {
    digitalWrite(redled,LOW);
    digitalWrite(greenled,HIGH);
    //noTone(5);
  }
  if (distance >= 300 || distance <= 0){
    Serial.println("Out of range");
  }
  else {
    //Serial.print(duration);
    //Serial.println(" sensor value");
    Serial.print(distance);
    Serial.print(" cm");
    Serial.print("   EEPROM setpoint: ");
    Serial.println(EEPROM.read(addr));
  }

    //This is where the set button works
    tzero=millis();
    while (digitalRead(setPin)== HIGH){
    timepressed = millis()- tzero;
    if (timepressed > 3000){
      //noTone(5);
      for (int i = 1; i < 6; i++){
          digitalWrite(greenled,LOW);
          digitalWrite(redled,LOW);
          delay(250);
          digitalWrite(greenled,HIGH);
          digitalWrite(redled,HIGH);
          delay(250);
          //digitalWrite(greenled,LOW);
          //digitalWrite(redled,LOW);
      }
      while (true){
        Serial.println("Bottom set point pressed ");
        Serial.print("Set Point: ");
        Serial.print(refvalue);
        Serial.print("    EEPROM Value: ");
        Serial.println(EEPROM.read(addr));
        
        if (digitalRead(uppin) == HIGH) {
          refvalue = refvalue + 2;
          digitalWrite(greenled,LOW);
          digitalWrite(redled,LOW);
          delay(125);
          digitalWrite(greenled,HIGH);
          digitalWrite(redled,HIGH);
          delay(125);
        }
        if (digitalRead(downpin) == HIGH) {
          refvalue = refvalue - 2;
          digitalWrite(greenled,LOW);
          digitalWrite(redled,LOW);
          delay(125);
          digitalWrite(greenled,HIGH);
          digitalWrite(redled,HIGH);
          delay(125);
        }
        if (digitalRead(setPin)==HIGH){
          digitalWrite(greenled,LOW);
          digitalWrite(redled,LOW);
          delay(250);
          digitalWrite(greenled,HIGH);
          digitalWrite(redled,LOW);
          delay(250);
          digitalWrite(greenled,LOW);
          digitalWrite(redled,HIGH);
          delay(250);
          digitalWrite(greenled,LOW);
          digitalWrite(redled,LOW);
          if (refvalue != EEPROM.read(addr)) {
            EEPROM.write(addr, refvalue);
          } 
          break;
        }
      delay(250);
      }
    }
  }
  delay(250);
}
