//L293D
//Motor left side
const int IN1  = 13;  
const int IN2  = 12;  
const int ENA  = 11;
//Motor right side
const int IN3  = 8; 
const int IN4  = 9;  
const int ENB  = 10;


//This will run only one time.
void setup(){
 
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


void loop(){
  drive(100, 255);
  delay(2000);
  drive(-255, -100);
  delay(2000);
}
