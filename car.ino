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
        int LED_RIGHT;
        int LED_LEFT;
        long duration; // variable for the duration of sound wave travel
        int measured_distance;
        Servo nav_servo;


    public:

        // Assign input variables to private variables
        Car(int in1, int in2, int ena, int in3, int in4, int enb, int servo_pin, int echo_pin, int trig_pin, int led_right,int led_left){
            this->IN1 = in1;
            this->IN2 = in2;
            this->IN3 = in3;
            this->IN4 = in4;
            this->ENA = ena;
            this->ENB = enb;
            this->SERVO_PIN = servo_pin;
            this->ECHO_PIN = echo_pin;
            this->TRIG_PIN = trig_pin;
            this->LED_RIGHT = led_right;
            this->LED_LEFT = led_left;

        }
    
    void init(){
                    // Set motor-pins as outputs
            pinMode(IN1, OUTPUT);
            pinMode(IN2, OUTPUT);
            pinMode(IN3, OUTPUT);
            pinMode(IN4, OUTPUT);
            pinMode(ENA, OUTPUT);
            pinMode(ENB, OUTPUT); 
            pinMode(LED_RIGHT, OUTPUT);
            pinMode(LED_LEFT, OUTPUT);

          
            // Servo setup
            nav_servo.attach(SERVO_PIN);

            // Distance sensor setup
            pinMode(TRIG_PIN, OUTPUT); // Sets the trigPin as an OUTPUT
            pinMode(ECHO_PIN, INPUT); // Sets the echoPin as an INPUT

            // Turn leds on and off
            digitalWrite(LED_LEFT, HIGH);
            digitalWrite(LED_RIGHT, HIGH);
            delay(1000);
            digitalWrite(LED_LEFT, LOW);
            digitalWrite(LED_RIGHT, LOW);

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
        measured_distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
        return measured_distance;
    }


    void navigate(){
        // When the servo is rotated, max_dist is the longest
        // distance the distance-sensor measures, angel is
        // the angel of the servo when the max_dist was measured
        int max_dist = 0;
        int angel = 0;
        // Variable for keeping comparing distances measured by distane-sensor
        int current_dist = 0;

        // Run navigation loop forever
        while(true){
            // Check if an object is blocking the robot
            // (something closer than 30 cm)
            if (get_distance() > 30){
                // Turn the distance-sensor forward
                rotate_servo(90);
                // Drive forward
                drive(255,255);
            } else {
                // Something is blocking the car's path
                // Stop driving
                stop_drive();
                // Turn the distance-sensor sideways
                rotate_servo(0);
            
                // Rotate the distance-sensor in 5deg increments
                // and find the longest distance (path that is not blocked)
                for(int i=0; i<180; i+=5){
                    // Rotate sensor
                    rotate_servo(i);
                    // Give the servo time to rotate
                    delay(150);
                    // Measure distance
                    current_dist = get_distance();
                    // Check if the current distance is a new
                    // max distance
                    if (current_dist > max_dist){
                        angel = i;
                        max_dist = current_dist;
                    }
                }
                // Drive a little backwards
                drive(-255,-255);
                delay(100);
                stop_drive();

                // Turn based on the angel 
                if (angel < 91){
                    drive(255,-255);
                    digitalWrite(LED_LEFT, HIGH);
                    delay(sqrt(angel)*60);
                    digitalWrite(LED_LEFT, LOW);
                } else {
                    drive(-255,255);
                    digitalWrite(LED_RIGHT, HIGH);
                    delay(sqrt(angel-90)*60);
                    digitalWrite(LED_RIGHT, LOW);
                }
                stop_drive();
            }
        }
    }
};


Car myCar(8, 7, 9, 5, 4, 3, 11, 13, 12, 6, 10);

void setup(){
    Serial.begin(9600);
    myCar.init();
    
}

void loop(){
    myCar.navigate();
}
