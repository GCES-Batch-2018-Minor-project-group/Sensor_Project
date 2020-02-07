#define Trigger 10 // pin code for trigger 
#define Echo 13 // pin code for echo
#define Buzzer 3 // pin code for buzzer
#define Vibrator 12 // pin code for vibrator

long time_taken; // calculates the time taken by the wave to return
int distance; // claculate the distance between the source and object

void setup() {
  Serial.begin(9600);
  
//  selecting the pin modes
  pinMode(Trigger, OUTPUT);
  pinMode(Echo, INPUT);
  pinMode(Buzzer, OUTPUT);
  pinMode(Vibrator, OUTPUT);
  
//  giving 0v to the buzzer pin
  digitalWrite(Buzzer, LOW);
  digitalWrite(Vibrator, LOW);
}

void loop() {
//  giving 0v and 5v to trigger pin turn-by-turn
  digitalWrite(Trigger, LOW);
  delayMicroseconds(2);
  digitalWrite(Trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(Trigger, LOW);

// calculating the time taken 
    time_taken = pulseIn(Echo, HIGH);

// calculating the diatnce
    distance = (time_taken/2)*0.034;

// runs the code if the distancce is more than 200 cm or less than 2 cm
      if(distance >= 200 || distance <=2){
        Serial.println("Out Of Range");
        digitalWrite(Buzzer, LOW);
        digitalWrite(Vibrator, LOW);
      }
//      runs this code when the distance is in between 2 cm to 80 cm
      else if(distance <= 80 && distance >= 2){
        Serial.print("Distance: " + distance);
        Serial.println("cm");
        digitalWrite(Vibrator, HIGH);
        digitalWrite(Buzzer, HIGH);
        tone(Buzzer, 400, 500);
        delay(500);
      }
//      runs the code when distance is not out of range or in buzzer range
      else{
        Serial.print("Distance: " + distance);
        Serial.println("cm");
        delay(500);
      }
}
