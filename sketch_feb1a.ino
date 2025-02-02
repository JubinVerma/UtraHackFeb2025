  // import newping library
  #include "NewPing.h"
  #include <string.h>
  #include <stdlib.h>


// Calibration Values
// *Get these from Calibration Sketch
int redMin = 269; // Red minimum value
int redMax = 1322; // Red maximum value
int greenMin = 351; // Green minimum value
int greenMax = 1377; // Green maximum value
int blueMin = 298; // Blue minimum value
int blueMax = 1326; // Blue maximum value

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
  const int enR = 10;
  const int in3 = 8;
  const int in4 = 9;

  // Supersonic
  const int trigPin = 12;
  const int echoPin = 11;
  #define MAX_DISTANCE 80

  NewPing sonar(trigPin, echoPin, MAX_DISTANCE);


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




  char *getColor() {
    char *color = (char*)malloc(64 * sizeof(char));
    redPW = getRedPW();
    greenPW = getGreenPW();
    bluePW = getBluePW();

    int redVal = map(redPW, redMin,redMax,255,0);
    int blueVal = map(bluePW, blueMin,blueMax,255,0);
    int greenVal = map(greenPW, greenMin,greenMax,255,0);

    color[0] = '\0'; 
    if (redVal <= 100 && blueVal <= 100 && greenVal <= 100) {
      strncat(color, "BLACK", 63); 
    }
    else if (redVal >= 250 && greenVal >= 270 && blueVal >= 270) {
      strncat(color, "WHITE", 63)
    }
    else if (redVal >= 200) {
      strncat(color, "RED", 63);
    }
    else if (blueVal >= 190) {
      strncat(color, "BLUE", 63);
    }
    else if (greenVal >= 150) {
      strncat(color, "GREEN", 63);
    }

    return color;
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
    *color = getColor();  // fill param

    if (strncmp(color, "BLUE", 63)) {
      turn_left(255); 
    }
    else if (strncmp(color, "GREEN", 63)) {
      turn_right(255); 
    }
    else if (strncmp(color, "RED", 63)) {
      uTurn(255);
    }
    
    
    // Delay 
    //stop(); 

    //bool isWall = false; 
    goForward(255);
      //isWall = detectWall();  
    Serial.print(sonar.ping_cm());
    Serial.print("\n");
    delay(600);
    if(sonar.ping_cm()<20){
      Serial.print("STOP\n");  
      stop(); 
      delay(2000);
    }
    //goBackward(255);

    // free(color);
  }

  bool detectWall() {
    return sonar.ping_cm() <= 10;
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
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
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
    delay(2250);
  }

  void turn_right(int speed) {
    // set speed
    analogWrite(enR, speed);
    analogWrite(enL, speed);
    // set direction and turn on motors
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
    delay(2250); 
  }
  void turn_left(int speed) {
    analogWrite(enR, speed);
    analogWrite(enL, speed);
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
    delay(2250);
  }


  void uTurn(int speed) {
    turn_left(speed); 
    turn_left(speed); 
  }


  struct node {
    char color[64];
    struct node *next; 
    struct node *prev; 
  };

  void insert(struct node *front, char *color) {
    if (front->color == NULL) {
      strncat(front->color, color, 63);
      front->next = (struct node*)malloc(sizeof(struct node*)); 
      (front->next)->prev = front; 
      return; 
    }
    struct node *curr = front; 
    while (curr->color != NULL) {
      
      curr = curr->next; 
    }
    strncat(front->color, color, 63);
    curr->next = (struct node*)malloc(sizeof(struct node*));
    (curr->next)->prev = curr; 
  }

  void freeList(struct node *front) {
    while (front->color != NULL) {

      front = front->next; 
      free(front->prev);
    }
    free(front); 

  }

  void remove(struct node *front, char *color) {
    if (strncmp(front->color, color, 63)) {
      
      front = front->next; 
      free(front->prev);
      front->prev = NULL;
    }
  }

  /*int length(struct node *front) {
    int len = 0; 
    struct *node curr = front; 
    while (curr->color != NULL) {
      curr = curr->next; 
      len++; 
    }
    return len - 1; 
  }

  void captureFlag(char **colors, int size) {

    char *color = getColor(); 
    if (length <= 3) {
      insert(head, color); 
    }

  }*/

  void maze_nav() {
    // 0 for north 
    // 1 for east 
    // 2 for south 
    // 3 for west 
    int dir = 0;  
    // if turn left +1 % 4
    // if turn right -1 % 4 
    // if u turn + 2 % 4
    goForward(255); 
    delay(2000); 
    int ran = random(2);
    if (ran == 0) {
      dir++; 
      
      turn_left(255); 
    }
    else {
      turn_right(255); 
      dir--; 
    }
    dir = dir % 4; 

  }

  struct node *head = (struct node*)malloc(sizeof(struct node*)); 

  void loop() {
    
    char *color = getColor(); 
    if (!detectWall() || !strncmp(color, "BLACK", 63)) {
      maze();
    }
    //maze();

    
  }
