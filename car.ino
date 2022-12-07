// Use servo library
#include <Servo.h>



class Car {
    
    // Define private variables
    private:
        // Constant variables related to the curcuit board
        int IN1;
        int IN2;
        int ENA;
        int IN3;
        int IN4;
        int ENB;
        // Data pin for servo
        int SERVO_PIN;
        // Echo and trig pin for ultrasonic sensor
        int ECHO_PIN;
        int TRIG_PIN;   
        // Led pins for leds located to the left and right of the car (viewed from the back)
        int LED_RIGHT;
        int LED_LEFT;
        // Create servo instance
        Servo nav_servo;


    public:
        // Assign input variables from instance to private variables
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

            // Turn leds on and off to indicate that the car is working
            digitalWrite(LED_LEFT, HIGH);
            digitalWrite(LED_RIGHT, HIGH);
            delay(1000);
            digitalWrite(LED_LEFT, LOW);
            digitalWrite(LED_RIGHT, LOW);

    }
        

    // Drive function for car. One sets the speed of the motors
    // on the left and right side, in order to make the car drive. Returns nothing
    void drive(int speed_left, int speed_right){

        if (speed_right > 0) { 
            // Turn right motors forwards since speed_right is positive
            digitalWrite(IN3, LOW);
            digitalWrite(IN4, HIGH);
            
        } else {
            // Turn right motors backwards since speed_right is negative
            digitalWrite(IN3, HIGH);
            digitalWrite(IN4, LOW);
        }
        
        if (speed_left > 0) {
            // Turn left motors forwards since speed_left is positive
            digitalWrite(IN1, HIGH);
            digitalWrite(IN2, LOW);
        } else {
            // Turn left motors backwards since speed_left is negative
            digitalWrite(IN1, LOW);
            digitalWrite(IN2, HIGH);
        }

        // Set speed of motors
        analogWrite(ENA, abs(speed_left));
        analogWrite(ENB, abs(speed_right));

    }

    // Stops motors. Returns nothing
    void stop_drive(){
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, LOW);
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, LOW);
    }

    // Function to rotate servo. Returns nothing
    void rotate_servo(int rotation_deg){
        // The function takes a value for servo position between 0-180 (degrees)
        // Using the map function, this is converted to values between 350 and 2950
        // which are the pulse-lengths corresponding to 0 and 180 degrees.
        int pulse = map(rotation_deg, 0, 180, 350, 2950);
        // Move the servo to given position
        nav_servo.writeMicroseconds(pulse);
    }
    
    // Function to measure distance using ultrasonic sensor. Retunrs value in cm. 
    int get_distance(){
        // Variable for the duration of sound wave travel for ultrasonic sensor
        long duration; 
        // Variable used for storing distance measured by ultrasonic sensor
        int measured_distance; 
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

    // Funtion that makes the car avoid obstacles and drive 
    // towards the most open area in its surroundings
    void navigate(){
        // When the servo is rotated from 0-180 deg, max_dist is the longest
        // distance the distance-sensor measures, angel is
        // the angel of the servo when the max_dist was measured
        int max_dist = 0;
        int angel = 0;
        // Variable for saving the momentary distance measured by distane-sensor
        int current_dist = 0;

        // Run navigation loop forever
        while(true){
            // Check if an object is blocking the robot
            // (something closer than 30 cm)
            if (get_distance() > 30){
                // The path of the robot is not blocked
                // Turn the distance-sensor facing forward
                rotate_servo(90);
                // Drive forward
                drive(255,255);

            } else {
                // Something is blocking the car's path
                // Stop driving
                stop_drive();
                // Turn the distance-sensor sideways
                rotate_servo(0);
                // Reset max_dist and angel
                max_dist = 0;
                angel = 0;
                // Rotate the distance-sensor in 5deg increments
                // and find the longest distance (where the path  is not blocked)
                for(int i=0; i<180; i+=5){
                    // Rotate sensor
                    rotate_servo(i);
                    // Give the servo time to rotate
                    delay(100);
                    // Measure distance
                    current_dist = get_distance();
                    // Print data to console (for debug purposes)
                    Serial.print("Angel: ");
                    Serial.print(i); 
                    Serial.print(","); 
                    Serial.print("Distance: ");
                    Serial.println(current_dist);
                    // Check if the current distance is a new
                    // max distance
                    if (current_dist > max_dist){
                        // Save the current distance as the new max distance as well as the servos angle
                        angel = i;
                        max_dist = current_dist;
                        Serial.print("New max-dist: ");
                        Serial.println(max_dist);
                    }
                }
                Serial.print("Final max-dist: ");
                Serial.println(max_dist);
                Serial.print("Angel: ");
                Serial.println(angel);
                
                // Drive a little backwards
                rotate_servo(90);
                drive(-255,-255);
                delay(100);
                stop_drive();

                // Turn based on the angel 
                if (angel < 91){
                    // If the angle is < 91 we want to turn to the right
                    Serial.println("Turning right");
                    drive(-255,255);
                    digitalWrite(LED_RIGHT, HIGH);
                    // The smaller the angle is, the longer we want to turn (since 90 is straight forward
                    // and an angle of 800 would mean that the robot is only 10deg off correct direction)
                    delay((90-angel)*60);
                    // Indicate turn-direction by lighting led
                    digitalWrite(LED_RIGHT, LOW);
                } else {
                    // If the angle is >= 91 we want to turn to the right
                    Serial.println("Turning left");
                    drive(255,-255);
                    digitalWrite(LED_LEFT, HIGH);
                    // The larger the angle is, the longer we want to turn (since 90 is straight forward 
                    // and an angle of 100 would mean that the robot is only 10deg off correct direction)
                    delay((angel-90)*60);
                    // Indicate turn-direction by lighting led
                    digitalWrite(LED_LEFT, LOW);
                }
                // Stop turning
                stop_drive();
            }
        }
    }
};

// Create car instance
Car myCar(8, 7, 9, 5, 4, 3, 11, 13, 12, 6, 10);

void setup(){
    // Begin serial comm.
    Serial.begin(9600);
    // initialize car
    myCar.init();
    // Make the car navigate
    myCar.navigate();
    
}

void loop(){

}
