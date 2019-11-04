#include <Servo.h>
int unoMode;
#define period 10000
unsigned long startTime = 0;
//UNO'S MODES:
  //Mode 1: Ball Find Mode
  //Mode 2: Find Goal Mode
  //Mode 3: Power Switch Mode
  //Mode 4: Score Goal Mode
//MOTOR PIN SETUP
  //left motor
  //#define enA 10
  #define in1 5
  #define in2 6
  //right motor
  //#define enB 
  #define in3 10
  #define in4 11
//SENSOR PIN SETUP
#define irSensor A0
#define trigPin 7
#define echoPin 8
//SERVO SETUP
Servo posServo;
//SWITCH PIN SETUP
#define switchPin 12
int switchState;

void setup() {
  //Pin, variable, and sensor declaration
  Serial.begin(9600);
  switchState = LOW;
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  //Set Servo to Pin 9
  posServo.attach(9);
  posServo.write(0);//servo door is open
  delay(1000);
  unoMode = 3; 
  
}

void loop() {
  float sharpDist = sharpSense();
  float wallDist = echoSense();
  //START TIMER, READ SWITCH
    while (unoMode == 3){
    switchState = digitalRead(switchPin);
    Serial.println(switchState);
    if (switchState == HIGH){
      unoMode = 1;
      }
    }

  //FIND BALL MODE CODE HERE
  while (unoMode == 1) {
  float sharpDist = sharpSense();
  float wallDist = echoSense();
  unsigned long currentTime = millis();
  unsigned long timeNow = currentTime - startTime;
  Serial.print(timeNow);
  Serial.println(" milliseconds");
   //Pattern movement for finding ball
   if (timeNow > period){
    getOut();
    startTime = currentTime;
    timeNow = currentTime - startTime; 
   }
   moveForward();
  if (wallDist < 10){
    getOut();
    }
  else if (sharpDist < 4.32){
    posServo.write(30);
    stopUno();
    moveBackwards();
    // delay(500);
    stopUno();
    Serial.println("BALL HAS BEEN CONTAINED");
    startTime = currentTime;
    unoMode = 2;
  }
 }



  //FIND/SCORE GOAL CODE HERE
  while (unoMode == 2){
    unsigned long currentTime = millis();
    unsigned long timeNow = currentTime - startTime;
    float wallDist = echoSense();
    Serial.print(wallDist);
    Serial.println("cm");
    moveForward();
    if (timeNow > period){
      getOut();
      startTime = currentTime;
      timeNow = currentTime - startTime;
      break;
    }
    else if (wallDist < 7){
      stopUno();
      turnRight();
      Serial.println("Step 1");
    }
    else if (wallDist < 90 && wallDist > 88){
      stopUno();
      turnRight();
      unoMode = 4;
      break; 
    }
    else {
      moveForward();
    }
  }



  while(unoMode == 4){
    unsigned long currentTime = millis();
    unsigned long timeNow = currentTime - startTime;
    float wallDist = echoSense();
      moveForward();
      if (timeNow < 7000){
        getOut();
        startTime = currentTime;
        timeNow = currentTime - startTime;
      }
      else if (wallDist < 10){
        stopUno();
        posServo.write(0);
         analogWrite(in1, 0);
         analogWrite(in2, 255);
         analogWrite(in3, 0);
         analogWrite(in4, 255);
        delay(700);
        stopUno();
        getOut();
        unoMode = 1;
        break;
      }
     
  }

}

//IR SENSOR FUNCTION
float sharpSense(){
  float val = analogRead(irSensor);
  float volts = val*0.0048828125;
  float distance = 13*pow(volts, -1);
  return distance;
}


//ECHO SENSOR FUNCTION
float echoSense(){
  digitalWrite(trigPin, LOW);
  delay(2);
  digitalWrite(trigPin, HIGH);
  delay(10);
  digitalWrite(trigPin, LOW);

  float duration = pulseIn(echoPin, HIGH);
  float distance = (duration/2) / 29.1;
  Serial.print("Distance:");
  Serial.print(distance);
  Serial.println("cm");
  return distance;
}


//MOVE FORWARD FUNCTION
void moveForward(){
    //move forwards
  analogWrite(in1, 0);
  analogWrite(in2, 200);
  analogWrite(in3, 0);
  analogWrite(in4, 200);

}


//STOP FUNCTION
void stopUno(){
  Serial.println("STOP!");
  analogWrite(in1, 0);
  analogWrite(in2, 0);
  analogWrite(in3, 0);
  analogWrite(in4, 0);
  delay(1000);
}


//TURN LEFT FUNCTION
void turnLeft(){ //240 degree turn LEFT function
  analogWrite(in1, 255);
  analogWrite(in2, 0);
  analogWrite(in3, 0);
  analogWrite(in4, 255);
  delay(1500);
   Serial.println("STOP!");
  analogWrite(in1, 0);
  analogWrite(in2, 0);
  analogWrite(in3, 0);
  analogWrite(in4, 0);
  delay(1000);
}

//TURN RIGHT FUNCTION
void turnRight(){ //240 turn RIGHT function
 analogWrite(in1, 0);
 analogWrite(in2, 255);
 analogWrite(in3, 255);
 analogWrite(in4, 0);
 delay(1300);

}

//ONE-EIGHTY TURN FUNCTION
void oneEighty(){ //180 degree turn function
  Serial.println("180 degree turn...");
  analogWrite(in1, 0);
  analogWrite(in2, 165);
  analogWrite(in3, 0);
  analogWrite(in4, 165);
  delay(2000);
}

//ACCELERATE AT TOP SPEED
void scoreGoal(){
  Serial.println("SCORE THIS GOAL!");
  analogWrite(in1, 0);
  analogWrite(in2, 255);
  analogWrite(in3, 0);
  analogWrite(in4, 255);
}

//MOVE BACKWARDs
void moveBackwards(){
  Serial.println("Moving backwards");  //move forwards
  analogWrite(in1, 195);
  analogWrite(in2, 0);
  analogWrite(in3, 195);
  analogWrite(in4, 0);

}

//GET OUT OF TROUBLE DEAR
void getOut(){
  unsigned long currentTime = millis();
  unsigned long getoutTime = currentTime - startTime;
     moveBackwards(); 
   delay(1000);
  turnRight();
  
}
