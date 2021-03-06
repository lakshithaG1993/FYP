const int stepPinX = 8; //clk
const int dirPinX = 9; //CW
const int enablePinX = 10; //en

const int stepPinY = 11;  //clk
const int dirPinY = 12;   //CW
const int enablePinY = 13; //en

const int limitSwitchXFront = 4;
const int limitSwitchXBack = 5;
const int limitSwitchYFront = 6;
const int limitSwitchYBack = 7;

const int stepTime = 500;  // half clock time

long xStepCount = 0; // no of X steps
long yStepCount = 0; // no of X steps

char rx_byte = '0'; // input byte from the serial monitor

boolean xDirection = HIGH; // initial directions of movement
boolean yDirection = HIGH;


int maxStepsX = 14000; //maximum steps towards X direction
int maxStepsY = 9000;  //maximum steps towards Y direction

void setup() {
  Serial.begin(9600);

  pinMode(dirPinX, OUTPUT);
  pinMode(stepPinX, OUTPUT);
  pinMode(enablePinX,OUTPUT);
  pinMode(enablePinY,OUTPUT);
  pinMode(dirPinY, OUTPUT);
  pinMode(stepPinY, OUTPUT);

  pinMode(limitSwitchXFront, INPUT);
  pinMode(limitSwitchXBack, INPUT);
  pinMode(limitSwitchYFront, INPUT);
  pinMode(limitSwitchYBack, INPUT);

  digitalWrite(dirPinX, xDirection);
  digitalWrite(dirPinY, yDirection);
}

void loop() {
  listenForCharacter();         // listen for inputs from serial monitor and update it with rx_byte
  if (rx_byte == '1') {
      moveForward(2000);
    }
  else if (rx_byte == '2') {
      moveBackward(1000);
   }
   else if (rx_byte == '3') {
      moveLeft(2000);
   }else if (rx_byte == '4') {
      moveRight(1000);
   }else if (rx_byte == '5') {
      returnToZero();
   }
   delay(3000);
}

boolean forwardDirection(){return LOW;}
boolean backwardDirection(){return HIGH;}
boolean leftDirection(){return HIGH;}
boolean rightDirection(){return LOW;}


boolean changeMovingDirection(boolean state) {
  state = !state;
  return state;
}

void motion(int finalXLocation, int finalYLocation){
  enableMotor(enablePinX);
  enableMotor(enablePinY);
  boolean reachedX = false;
  boolean reachedY = false;
  if(xStepCount <finalXLocation && yStepCount <finalYLocation){
    digitalWrite(dirPinX, forwardDirection());
    digitalWrite(dirPinY, leftDirection());
    while (reachedLimitSwitch(limitSwitchXBack) == LOW && reachedLimitSwitch(limitSwitchYBack) == LOW && !(reachedX && reachedY)) {
      if(xStepCount < finalXLocation){
          digitalWrite(stepPinX, HIGH);
          xStepCount = xStepCount + 1;
        }else if (xStepCount >= finalXLocation){
          digitalWrite(stepPinX, LOW);
          reachedX = true;
        } 
          
        if(yStepCount < finalYLocation){
          digitalWrite(stepPinY, HIGH);
          yStepCount = yStepCount + 1;
        }else if (yStepCount >= finalYLocation){
          digitalWrite(stepPinY, LOW);
          reachedY = true;
        } 
          delayMicroseconds(stepTime);
          digitalWrite(stepPinX, LOW);
          digitalWrite(stepPinY, LOW);
          delayMicroseconds(stepTime);    
  }
    
    
  }else if (xStepCount >finalXLocation && yStepCount <finalYLocation){}
  else if (xStepCount <finalXLocation && yStepCount >finalYLocation){}
  else if (xStepCount >finalXLocation && yStepCount >finalYLocation){}

   disableMotor(enablePinX);
   disableMotor(enablePinY);
   
}


void moveForward(int steps) {
  int currentStep = 0;
  enableMotor(enablePinX);
  digitalWrite(dirPinX, forwardDirection());
  while (reachedLimitSwitch(limitSwitchXBack) == LOW && currentStep <= steps) {
    digitalWrite(stepPinX, HIGH);
    delayMicroseconds(stepTime);
    digitalWrite(stepPinX, LOW);
    delayMicroseconds(stepTime);    
    currentStep = currentStep + 1;
    //printStepCount(xStepCount);
  }
  disableMotor(enablePinX);
}

void moveBackward(int steps) {
  enableMotor(enablePinX);
  int currentStep = 0;
  digitalWrite(dirPinX, backwardDirection());
  while (reachedLimitSwitch(limitSwitchXFront) == LOW && currentStep <= steps) {
    digitalWrite(stepPinX, HIGH);
    delayMicroseconds(stepTime);
    digitalWrite(stepPinX, LOW);
    delayMicroseconds(stepTime);
    currentStep = currentStep + 1;
    //printStepCount(xStepCount);
  }
  disableMotor(enablePinX);
}

void moveLeft(int steps) {
  enableMotor(enablePinY);
  int currentStep = 0;
  digitalWrite(dirPinY, leftDirection());
  while (reachedLimitSwitch(limitSwitchYBack) == LOW && currentStep <= steps) {
    digitalWrite(stepPinY, HIGH);
    delayMicroseconds(stepTime);
    digitalWrite(stepPinY, LOW);
    delayMicroseconds(stepTime);
    currentStep = currentStep + 1;
    //printStepCount(yStepCount);
  }
  disableMotor(enablePinY);
}

void moveRight(int steps) {
  enableMotor(enablePinY);
  int currentStep = 0;
  digitalWrite(dirPinY, rightDirection());
  while (reachedLimitSwitch(limitSwitchYFront) == LOW && currentStep <= steps) {
    digitalWrite(stepPinY, HIGH);
    delayMicroseconds(stepTime);
    digitalWrite(stepPinY, LOW);
    delayMicroseconds(stepTime);
    currentStep = currentStep + 1;
    //printStepCount(yStepCount);
  }
  disableMotor(enablePinY);
}

void returnToZero() {
    moveBackward(10000);
    moveRight(10000);
    xStepCount=0;
    yStepCount = 0;
}

boolean reachedLimitSwitch(int switchPin) {
  if (digitalRead(switchPin) == LOW) {
    return HIGH;
  }
  else {
    return LOW;
  }
}

void listenForCharacter(){
  if (Serial.available() > 0) {    // is a character available?
    rx_byte = Serial.read();
    Serial.print(xStepCount);
    Serial.print(' ');
    Serial.print(yStepCount);
    Serial.print('\n');
  }
}

void disableMotor(int pinNumber){
  digitalWrite(pinNumber, LOW); 
}

void enableMotor(int pinNumber){
  digitalWrite(pinNumber, HIGH); 
}

void printStepCount(int steps){
  Serial.print(steps);
  Serial.print('\n');
  }
