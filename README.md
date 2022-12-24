# Brombrom
Arduino prosjekt IT2

Laget av Martin Ansteensen og Vemund S. Wilhelmsen

## Beskrivelse

I dette prosjektet har en bil blitt laget med en Arduino Uno, en avstandssensor, en servo og noen andre komponenter. Bilen vil kjøre framover til avstandssensoren registrerer en liten distanse. Bilen vil da stoppe, og servoen med sensoren på toppen vil rotere til sensoren finner den vinkelen som korresponderer med den lengste avstanden. Bilen vil deretter kjøre i denne retningen. 

## Bruk
Hvis du lager kretsen akkurat som i bildet og laster opp Car.ino på arduinoen vil bilen kjøre helt av seg selv.

## Liste over filer
* Car.ino
* tinkerCadCircuits.png
* wiring_led_distance_servo.png
* Wiring-L293D-Motor-Driver-IC-with-DC-motors-Arduino.png


## Car klasse
Denne klassen er for navigasjon og kjøring av bilen

### Metoder 

#### drive

drive metoden tar fart på de venstre hjulene og fart på de høyre hjulene som input og får deretter hjulene til å kjøre med de gitte fartene 

#### stop_drive
Får alle hjul til å stoppe

#### rotate_servo
Får servoen til å rotere mellom 0 og 180 grader.

#### get_distance
Måler avstanden til nærmeste objekt 

#### navigate

Roterer servoen, finner deretter vinkelen hvor avstanden er lengst. Bilen vil videre kjøre bakover, og deretter svinge mot den foretrukne vinkelen

## Krets og komponenter
Du vil trenge:
* 4 små DC-motorer
* 2 LEDS
* 1 L293D-motordriverchip
* En del ledninger
* 1 avstandssensor 
* 1 servo (5V)
* Batteripakke rundt 9V
* En plate til å montere bilen på

Følg disse diagramene for å koble opp bilen. 
![Motorkrets](https://github.com/vewia002/brombrom/blob/main/Wiring-L293D-Motor-Driver-IC-with-DC-Motors-Arduino.png?raw=true)
![Leds- og servokrets](https://github.com/vewia002/brombrom/blob/main/wiring_led_distance_servo.png)


