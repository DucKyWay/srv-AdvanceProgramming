// 0-250 ไฟดวง 1
// 251-500 ไฟดวง 1,2
// 501-750 ไฟดวง 1-3
// 751-1000 ไฟดวง 1-4

#define analog A0
#define redLed 13
#define yellowLed 12
#define greenLed 11
#define blueLed 10

void setup(){
	Serial.begin(9600);
}

void loop(){
	int valueAnalog = analogRead(analog);
 	Serial.println(valueAnalog);
  
  	if(valueAnalog >= 0) {
        digitalWrite (redLed, HIGH);
    } else {
        digitalWrite (redLed, LOW);
    }
    if(valueAnalog > 250) {
        digitalWrite (yellowLed, HIGH);
    } else {
        digitalWrite (yellowLed, LOW);
    }
    if(valueAnalog > 500) {
        digitalWrite (greenLed, HIGH);
    } else {
        digitalWrite (greenLed, LOW);
    }
    if(valueAnalog > 750) {
        digitalWrite (blueLed, HIGH);
    } else {
        digitalWrite (blueLed, LOW);
    }
    if(valueAnalog > 1000){
        digitalWrite (redLed, LOW);
        digitalWrite (yellowLed, LOW);
        digitalWrite (greenLed, LOW);
        digitalWrite (blueLed, LOW);
    }
}