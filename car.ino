//L293D
//Motor left side
const int IN1  = 13;  
const int IN2  = 12;  
const int ENA  = 11;
//Motor right side
const int IN3  = 8; 
const int IN4  = 9;  
const int ENB  = 10;

#define echoPin 3 // attach pin D2 Arduino to pin Echo of HC-SR04
#define trigPin 2 //attach pin D3 Arduino to pin Trig of HC-SR04

// defines variables
long duration; // variable for the duration of sound wave travel
int distance; // variable for the distance measurement


//This will run only one time.
void setup(){
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

}

void drive(int speed_left, int speed_right){


    if (speed_right > 0) {
        
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, HIGH);
        
    } else {
      
        digitalWrite(IN3, HIGH);
        digitalWrite(IN4, LOW);
    }
    
    if (speed_left > 0) {
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, HIGH);
    } else {
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
    }


    analogWrite(ENA, abs(speed_left));
    analogWrite(ENB, abs(speed_right));

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
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  return distance;

}

void loop(){
//  drive(100, 255);
//  delay(2000);
//  drive(-255, -100);
//  delay(2000);
  int a = get_distance();
   if (a <40){
    drive(-150,150);
   } else{
   drive(150,150);
   }
  }