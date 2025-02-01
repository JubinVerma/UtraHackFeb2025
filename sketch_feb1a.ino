// import newping library
#include "NewPing.h"
#include <string.h>
#include <stdlib.h>

// Define color sensor pins
#define S0 0
#define S1 1
#define S2 2
#define S3 3
#define sensorOut 4

// Variables for Color Pulse Width Measurements
int redPW = 0;
int greenPW = 0;
int bluePW = 0;

// Motor A connections
const int enL = 5;
const int in1 = 6;
const int in2 = 7;
// Motor B connections
const int enR = 8;
const int in3 = 9;
const int in4 = 10;

// Supersonic
const int trigPin = 11;
const int echoPin = 12;
#define MAX_DISTANCE 50

NewPing sonar(trig_pin, echo_pin, MAX_DISTANCE);


void setup() {

  // COLOR
  // Set S0 - S3 as outputs
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);

  // Set Pulse Width scaling to 20%
  digitalWrite(S0, HIGH);
  digitalWrite(S1, LOW);

  // Set Sensor output as input
  pinMode(sensorOut, INPUT);

  // Setup Serial Monitor
  Serial.begin(9600);

  // MOTOR
  // Set all the motor control pins to outputs
  pinMode(enL, OUTPUT);
  pinMode(enR, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  // Turn off motors - Initial state
  stop();

  // Supersonic
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

// Variables for Color Pulse Width Measurements
int redPW = 0;
int greenPW = 0;
int bluePW = 0;

void loop() {

  maze();
  char *color = getColor();
  if (strncmp(color, "BLACK", 63)) {
    stop();
  }
}

char *getColor() {
  char *s = malloc(64 * sizeof(char));
  redPW = getRedPW();
  greenPW = getGreenPW();
  bluePW = getBluePW();
  delay(200);
  // if something

  return s;
}

void stop() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}

void maze() {
  // check the color of the current tile
  // runs according to the direction given
  // while it is not wall, run
  // exit while, run backward a couple tile
  // exit the function
  char *color = getColor();  // fill param

  if (strncmp(color, "BLACK", 63)) {
    goForward(255);
  }
  else if (strncmp(color, "BLUE", 63) {
    turnLeft(100); 
  }
  else if (strncmp(color, "GREEN", 63)) {
    turnRight(100); 
  }
  else if (strncmp(color, "RED", 63)) {
    uTurn(100);
  }
 
  // Delay 
  stop(); 

  bool isWall = false; 
  while (!isWall) {
    goForward(255);
    isWall = detectWall(); 
  }

  goBackward(); 
  // delay
  stop(); 
  free(color);
}

bool detectWall() {
  int stop_dist = 20;
  return sonar.ping_cm() <= 20;
}

int getRedPW() {
  // Set sensor to read Red only
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);
  // Define integer to represent Pulse Width
  int PW;
  // Read the output Pulse Width
  PW = pulseIn(sensorOut, LOW);
  // Return the value
  return PW;
}

// Function to read Green Pulse Widths
int getGreenPW() {
  // Set sensor to read Green only
  digitalWrite(S2, HIGH);
  digitalWrite(S3, HIGH);
  // Define integer to represent Pulse Width
  int PW;
  // Read the output Pulse Width
  PW = pulseIn(sensorOut, LOW);
  // Return the value
  return PW;
}

// Function to read Blue Pulse Widths
int getBluePW() {
  // Set sensor to read Blue only
  digitalWrite(S2, LOW);
  digitalWrite(S3, HIGH);
  // Define integer to represent Pulse Width
  int PW;
  // Read the output Pulse Width
  PW = pulseIn(sensorOut, LOW);
  // Return the value
  return PW;
}


// Functions for turning in maze
void goForward(int speed) {
  // Setting the speed to be maximal
  analogWrite(enL, speed);
  analogWrite(enR, speed);

  // Go straight
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void goBackward(int speed) {
  // Setting the speed to be maximal
  analogWrite(enL, speed);
  analogWrite(enR, speed);

  // Go straight
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

void turn_left(int speed) {
  // set speed
  analogWrite(en_a, speed);
  analogWrite(en_b, speed);
  // set direction and turn on motors
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}
void turn_right(int speed) {
  analogWrite(en_a, speed);
  analogWrite(en_b, speed);
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}


void uTurn(int speed) {
  analogWrite(en_a, speed);
  analogWrite(en_b, speed);
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  // Delay Amount
  delay(1000);
}


void captureFlag() {
}