# Brombrom
Arduino project IT2

MADE BY: Martin Ansteensen and Vemund S. Wilhelmsen

## Description

In this project a car has been made using an Arduino uno, a distance sensor, a servo, wheels and some other components. The car will drive forward until the distance sensor registers a short distance. The car will then stop, and the servo with the sensor on top will rotate and the sensor will find the angle which gives the longest distance. The car will then drive in this direction. 


## Use 
If you make the circuit excactly as in the picture and upload the code, the car will the drive. 

## List of files 
* Car.ino


## Car class
This class is for navigation and driving for the car

### Methods 

#### drive
The drive method takes speed on left wheels and speed on right wheels as input, and then makes the wheels drive with the given speed

#### stop_drive
Makes all wheels stop

#### rotate_servo
Makes the servo rotate between 0 and 180 degrees

#### get_distance
Measures distance to closest object with the distance sensor

#### navigate
Rotates servo, then finds at which angle the distance i longest. The car proceeds to drive backwards, and then turns to the preferred angle 

## Circuit
Follow this diagram to create the circuit that controls the motors
![alt text](https://github.com/vewia002/brombrom/blob/main/Wiring-L293D-Motor-Driver-IC-with-DC-Motors-Arduino.png?raw=true)


