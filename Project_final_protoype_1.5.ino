#include <SoftwareSerial.h>
#include <TinyGPS++.h>

#define Trigger 10// pin number for trigger of the ultra sonic sensor
#define Echo 13// pin number for echo of ultra soniuc sensor
#define Buzzer 4// pin for buzzer positive 
#define Vibrator 12// pin for vibrator positive

  static const int GSM_TX = 8, GSM_RX = 9; // pin number for gsm TX & RX
  static const int GPS_TX = 2, GPS_RX = 3; // pin number for gps TX & RX
  static const uint32_t GPSbaud = 4800; // baud number for gps serial

//making serial for gsm module and gps chip
SoftwareSerial GSM_Serial(GSM_RX, GSM_TX);
SoftwareSerial GPS_Serial(GPS_RX, GPS_TX);

//creating object of tinyGPS++
  TinyGPSPlus gps;

//varibales for calculating distance and time taken
long time_taken;
float distance;

//variable to store the message from GSM module
String incoming = "";

//Varibales for storing the latitude and longitude and location message
String Lat = "", Lng = "";
String Location = "";

//void checK_distance();
  
void setup() 
{
//  starting the serial monitor and program
  Serial.begin(9600);

// starting serial for gsm module and gps chip
    GSM_Serial.begin(9600);
    GPS_Serial.begin(GPSbaud);

        GSM_Serial.println("AT");
        updateGsmSerial();

//setting the pinmode of the given attached wires
    pinMode(Trigger, OUTPUT);
    pinMode(Echo, INPUT);
    pinMode(Buzzer, OUTPUT);
    pinMode(Vibrator, OUTPUT);

//setting the buzzer and vibrator pins to low voltage
      digitalWrite(Buzzer, LOW);
      digitalWrite(Vibrator, LOW);
}

void loop() 
{
  updateGsmSerial();
  RecivingCall();

//providing low and high voltage to trigger alternately
  digitalWrite(Trigger, LOW);
  delayMicroseconds(2);
  digitalWrite(Trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(Trigger, LOW);

//calculating distance
    time_taken = pulseIn(Echo, HIGH);
    distance = (time_taken/2)*0.034;
    
//    check_distance();

    Serial.print("Distance : ");
  if(distance > 200 || distance <= 2)
  {
     Serial.println("OUT OF RANGE");
     digitalWrite(Buzzer, LOW);
     digitalWrite(Vibrator, LOW);
     delay(500);
  }else if(distance > 2 && distance <= 80){
    digitalWrite(Buzzer, HIGH);
    digitalWrite(Vibrator, HIGH);
    Serial.print(distance);
    Serial.println(" cm");
    delay(500);
  }else{
    digitalWrite(Buzzer, LOW);
    digitalWrite(Vibrator, LOW);
    Serial.print(distance);
    Serial.println(" cm");
    delay(500);
  }
}


//function to update the gsm module
void updateGsmSerial()
{
  delay(500);
  while(Serial.available())
  {
    GSM_Serial.write(Serial.read());
  }
  while(GSM_Serial.available())
  {
    Serial.println(GSM_Serial.read());
  }
}

//function to track the latitude and longitude through gps chip
String track()
{
  if(gps.location.isValid()){
      Lat = (gps.location.lat(), 6);
      Lng = (gps.location.lng(), 6);

        Location = Lat + "," + Lng;
  }else{
    Location = "no GPS data found";
  }

  return Location;
}


//function to be executed if there is a call to the GSM module
void RecivingCall()
{
  while(GPS_Serial.available() > 0)
  {
    incoming = GSM_Serial.read();
    Serial.println(incoming);

      delay(1000);

        if(incoming == "RING")
        {
          GSM_Serial.println("ATH");  // hanging up the incoming call
          updateGsmSerial();

            GSM_Serial.println("AT+CMGF=1"); // setting the gsm module to be in sms or text mode
            updateGsmSerial();

            GSM_Serial.println("AT+CMGS=\"+9779825140802\""); // getting the number to send message to
            updateGsmSerial();

             String Loaction = "https://google.com/maps/@" + track(); // storing the text message to be sent

           GSM_Serial.println(Location); // giving the location provided by GPS 
           updateGsmSerial();

           GSM_Serial.println((char)26); //Termination of the text message , code for ethernet bata
            
        }else{
          Serial.println("Some Error");
        }
  }
  
}

//function to check the distance and alert when object detected at certain distance
//void checK_distance()
//{
//  Serial.print("Distance : ");
//  if(distance > 200 || distance <= 2)
//  {
//     Serial.println("OUT OF RANGE");
//     digitalWrite(Buzzer, LOW);
//     digitalWrite(Vibrator, LOW);
//     delay(500);
//  }else if(distance > 2 && distance <= 80){
//    digitalWrite(Buzzer, HIGH);
//    digitalWrite(Vibrator, HIGH);
//    Serial.print(distance);
//    Serial.println(" cm");
//    delay(500);
//  }else{
//    digitalWrite(Buzzer, LOW);
//    digitalWrite(Vibrator, LOW);
//    Serial.print(distance);
//    Serial.println(" cm");
//    delay(500);
//  }
//}