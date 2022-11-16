//L293D
#include <Servo.h>


class Car {


    // Define private variables
    private:
        int IN1;
        int IN2;
        int ENA;
        int IN3;
        int IN4;
        int ENB;
        int SERVO_PIN;
        int ECHO_PIN;
        int TRIG_PIN;
        long duration; // variable for the duration of sound wave travel
        int distance;
        Servo nav_servo;


    public:

        // Assign input variables to private variables
        Car(int in1, int in2, int ena, int in3, int in4, int enb, int servo_pin, int echo_pin, int trig_pin){
            this->IN1 = in1;
            this->IN2 = in2;
            this->IN3 = in3;
            this->IN4 = in4;
            this->ENA = ena;
            this->ENB = enb;
            this->SERVO_PIN = servo_pin;
            this->ECHO_PIN = echo_pin;
            this->TRIG_PIN = trig_pin;

        }
    
    void init(){
                    // Set motor-pins as outputs
            pinMode(IN1, OUTPUT);
            pinMode(IN2, OUTPUT);
            pinMode(IN3, OUTPUT);
            pinMode(IN4, OUTPUT);
            pinMode(ENA, OUTPUT);
            pinMode(ENB, OUTPUT); 
          
            // Servo setup
            nav_servo.attach(SERVO_PIN);

            // Distance sensor setup
            pinMode(TRIG_PIN, OUTPUT); // Sets the trigPin as an OUTPUT
            pinMode(ECHO_PIN, INPUT); // Sets the echoPin as an INPUT

    }
        

    // Drive function which gives speed at both sides
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

        // Set speed
        analogWrite(ENA, abs(speed_left));
        analogWrite(ENB, abs(speed_right));

    }

    // Stop motors
    void stop_drive(){
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, LOW);
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, LOW);
    }

    // Function to rotate servo
    void rotate_servo(int rotation){
        //nav_servo.write(rotation);
        int pulse = map(rotation, 0, 180, 350, 2950);
        nav_servo.writeMicroseconds(pulse);
    }

    int get_distance(){
        // Clears the trigPin condition
        digitalWrite(TRIG_PIN, LOW);
        delayMicroseconds(2);
        // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
        digitalWrite(TRIG_PIN, HIGH);
        delayMicroseconds(10);
        digitalWrite(TRIG_PIN, LOW);
        // Reads the echoPin, returns the sound wave travel time in microseconds
        duration = pulseIn(ECHO_PIN, HIGH);
        // Calculating the distance
        distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
        // Displays the distance on the Serial Monitor
        //  Serial.print("Distance: ");
        //  Serial.print(distance);
        //  Serial.println(" cm");
        return distance;
    }



    void navigate(){
        int vinkel = 0;
        int avstand = 0;
        while(true){
                    if (get_distance() > 30){
            rotate_servo(90);
            drive(255,255);
            }
            else{
            stop_drive();
                rotate_servo(0);

            for(int i=0;i<180; i+=5){
                rotate_servo(i);
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
    }


};


Car myCar(8, 7, 9, 5, 4, 3, 11, 13, 12);

void setup(){
    Serial.begin(9600);
    myCar.init();
    
}

void loop(){
    myCar.navigate();
    // myCar.rotate_servo(90);
    // delay(500);
    
    // myCar.rotate_servo(0);
    // delay(500);
    
    // myCar.rotate_servo(180);
    // delay(500);
}
