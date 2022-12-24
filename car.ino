// Bruk servo-biblioteket
#include <Servo.h>


class Car {
    
    // Definer private variabler (i klassen)
    private:
        // Konstanter for motordriveren
        int IN1;
        int IN2;
        int ENA;
        int IN3;
        int IN4;
        int ENB;
        // PWM for servoen
        int SERVO_PIN;
        // Echo og trig for avstandssensor
        int ECHO_PIN;
        int TRIG_PIN;   
        // Tilkobling for LEDs som sitter foran til venstre og høyre på bilen (sett bakfra)
        int LED_RIGHT;
        int LED_LEFT;
        // Lag en servo-instans
        Servo nav_servo;

    public:
        // Sett de private variablene lik det som blir oppgitt i konstruktøren
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
            // Definer alle tilkoblinger for motor og LEDS som utganger
            pinMode(IN1, OUTPUT);
            pinMode(IN2, OUTPUT);
            pinMode(IN3, OUTPUT);
            pinMode(IN4, OUTPUT);
            pinMode(ENA, OUTPUT);
            pinMode(ENB, OUTPUT); 
            pinMode(LED_RIGHT, OUTPUT);
            pinMode(LED_LEFT, OUTPUT);

            // Koble til servoen
            nav_servo.attach(SERVO_PIN);

            // Oppsett for distansensoren
            pinMode(TRIG_PIN, OUTPUT);
            pinMode(ECHO_PIN, INPUT); 

            // SKru på venstre og høyre led for å vise at ting fungerer som det skal
            digitalWrite(LED_LEFT, HIGH);
            digitalWrite(LED_RIGHT, HIGH);
            delay(1000);
            digitalWrite(LED_LEFT, LOW);
            digitalWrite(LED_RIGHT, LOW);

    }
        

    // Kjøre-funksjon for bilen. Parametrene er hastighet for hjulene på venstre
    // og høyre side, og kan være fra -100 til 100.
    void drive(int speed_left, int speed_right){

        if (speed_right > 0) { 
            // Kjør motorene på høyre side forover ettersom speed_right er positiv
            digitalWrite(IN3, LOW);
            digitalWrite(IN4, HIGH);
            
        } else {
            // Kjør motorene på høyre side bakover ettersom speed_right er negativ
            digitalWrite(IN3, HIGH);
            digitalWrite(IN4, LOW);
        }
        
        if (speed_left > 0) {
            // Kjør motorene på venstre side forover ettersom speed_left er positiv
            digitalWrite(IN1, HIGH);
            digitalWrite(IN2, LOW);
        
        } else {
            // Kjør motorene på venstre side bakover ettersom speed_left er negativt
            digitalWrite(IN1, LOW);
            digitalWrite(IN2, HIGH);
        }

        // Sett hastigheten til motorene
        analogWrite(ENA, abs(speed_left));
        analogWrite(ENB, abs(speed_right));

    }

    // Stopper alle motorene
    void stop_drive(){
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, LOW);
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, LOW);
    }

    // Fukjson for å rotere servoen. rotation_deg fra 0-180
    void rotate_servo(int rotation_deg){
        // map-funksjonen gjør om gradene fra 0-180 til et tall mellom 350
        // og 2950, som tilsvarer endepunktene (0 og 180) på servoen i puls-lengder 
        int pulse = map(rotation_deg, 0, 180, 350, 2950);
        // Flytt servoen til angitt posisjon
        nav_servo.writeMicroseconds(pulse);
    }
    
    // Funksjon for å måle avstand med sensoren. Returnerer avstanden (cm).
    int get_distance(){
        // Tiden det tar for lyden å reise frem og tilbake til sensoren
        long duration; 
        // Avstanden målt av sensoren
        int measured_distance; 
        // Klargjør sensoren
        digitalWrite(TRIG_PIN, LOW);
        delayMicroseconds(2);
        // Mål avstanden
        digitalWrite(TRIG_PIN, HIGH);
        delayMicroseconds(10);
        digitalWrite(TRIG_PIN, LOW);
        duration = pulseIn(ECHO_PIN, HIGH);
        // Regn ut avstanden (tid*farten til lydbølgen/2)
        measured_distance = duration * 0.034 / 2;
        return measured_distance;
    }

    // Funksjon som lar bilen unngå hindringer og kjøre
    // mot der det er mest åpent
    void navigate(){
        // Når servoen roteres fra 0 til 180 grader, er max_dist
        // den lengste avstanden den måler i løpet av rotasjonen.
        // angel er vinkelen når denne avstanden ble målt
        int max_dist = 0;
        int angel = 0;
        // Variable for å lagre den momentane avstanden som blir målt av sensoren
        int current_dist = 0;

        // Kjør for alltid
        while(true){
            // Sjekk om noe er i veien for roboten
            if (get_distance() > 30){
                // Det er fri vei, la avstandssensoren peke rett frem.
                rotate_servo(90);
                // Kjør forover
                drive(255,255);

            } else {
                // Det er noe som blokkerer veien videre
                stop_drive();
                // Snu avstandssensoren til den ene siden
                rotate_servo(0);
                // Resett max_dist og angel
                max_dist = 0;
                angel = 0;
                // Finn området hvor det er mest åpent ved å rotere
                // sensoren i intervaller på 5 grader
                for(int i=0; i<180; i+=5){
                    // Roter sensoren
                    rotate_servo(i);
                    // Gi servoen tid til å rotere
                    delay(100);
                    // Mål avstanden
                    current_dist = get_distance();
                    // Print data i konsollen (for feilsøking)
                    Serial.print("Angel: ");
                    Serial.print(i); 
                    Serial.print(","); 
                    Serial.print("Distance: ");
                    Serial.println(current_dist);
                    // Sjekk om denne målingen (avstanden) er 
                    // større en tidligere målinger
                    if (current_dist > max_dist){
                        // Large denne avstanden som et nytt maksimum sammen med vinkelen til servoen
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
                
                // Kjør litt bakover
                rotate_servo(90);
                drive(-255,-255);
                delay(100);
                stop_drive();

                // Sving basert på vinkelen
                if (angel < 91){
                    // Når vinkelen er mindre enn 91 skal bilen svinge til høyre
                    Serial.println("Turning right");
                   // Begynn å svinge
                    drive(-255,255);
                    digitalWrite(LED_RIGHT, HIGH);
                    // Jo mindre vinkelen er, jo mer skal vi svinge (siden 90 grader er rett frem).
                    delay((90-angel)*60);
                    // Vis at bilen skal svinge til høyre ved å tenne LEDen
                    digitalWrite(LED_RIGHT, LOW);
                } else {
                    // Bilen skal  svinge til venstre
                    Serial.println("Turning left");
                    // Begynn å svinge
                    drive(255,-255);
                    digitalWrite(LED_LEFT, HIGH);
                    // Jo større vinkelen er jo mer skal vi svinge (siden 180 
                    // ville vært rett til siden, og 90 fortsatt er rett frem)
                    // Vi vil fortsatt har korreksjonen på intervallet 0-90
                    delay((angel-90)*60);
                    // Vis at bilen skal svinge til venstre ved å tenne LEDen
                    digitalWrite(LED_LEFT, LOW);
                }
                // Stopp svingingen
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
