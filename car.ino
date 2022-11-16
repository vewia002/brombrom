//L293D
#include <Servo.h>
//Motor left side
const int IN1  = 8;  
const int IN2  = 7;  
const int ENA  = 9;
//Motor right side
const int IN3  = 5; 
const int IN4  = 4;  
const int ENB  = 3;
Servo servo1;
int servoPin = 11;

#define echoPin 13 // attach pin D2 Arduino to pin Echo of HC-SR04
#define trigPin 12 //attach pin D3 Arduino to pin Trig of HC-SR04

// defines variables
long duration; // variable for the duration of sound wave travel
int distance; // variable for the distance measurement


//This will run only one time.
void setup(){
    servo1.attach(servoPin);
    pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
    pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
    Serial.begin(9600); // // Serial Communication is starting with 9600 of baudrate speed
    Serial.println("Ultrasonic Sensor HC-SR04 Test"); // print some text in Serial Monitor
    Serial.println("with Arduino UNO R3");
 
    //Set pins as outputs
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
    pinMode(ENA, OUTPUT);
    pinMode(ENB, OUTPUT);
    
    //Motor Control - Motor A: IN1,IN2 & Motor B: IN3,IN4
    rotate(90);

    delay(1000);
}

int pulse = 0;
//Function to rotate servo
void rotate(int rotation){
  pulse = map(rotation, 0, 180, 350, 2950);
  servo1.write(pulse);
}

//Drive function which gives speed at both sides
void drive(int speed_left, int speed_right){

    //Turn right if speed_right is positive
    if (speed_right > 0) { 
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, HIGH);
        
    } else {
    //Turn left if speed_right is not positive
        digitalWrite(IN3, HIGH);
        digitalWrite(IN4, LOW);
    }
    
    if (speed_left > 0) {
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
    } else {
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, HIGH);
    }


    analogWrite(ENA, abs(speed_left));
    analogWrite(ENB, abs(speed_right));

}

void stop_drive(){
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, LOW);
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, LOW);
}


int get_distance(){
  // Clears the trigPin condition
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  // Displays the distance on the Serial Monitor
//  Serial.print("Distance: ");
//  Serial.print(distance);
//  Serial.println(" cm");
  return distance;
}

int vinkel = 0;
int avstand = 0;


void loop(){
     if (get_distance() > 30){
      rotate(90);
      drive(255,255);
     }
     else{
      stop_drive();
        rotate(0);

       for(int i=0;i<180; i+=5){
         rotate(i);
         delay(150);
         int current_dist = get_distance();
         if (current_dist > avstand){
          vinkel = i;
          avstand = current_dist;
         }
       }
      drive(-255,-255);
      delay(100);
      stop_drive();
      if (vinkel < 91){
        drive(255,-255);
        delay(sqrt(vinkel)*60);
      } else {
        drive(-255,255);
        delay(sqrt(vinkel-90)*60);
      }
      stop_drive();
       
     }
}
  