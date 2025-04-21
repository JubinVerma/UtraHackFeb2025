#include "NewPing.h"

#define S0 0
#define S1 1
#define S2 2
#define S3 3
#define sensorOut 4

#define enL 5
#define in1 6
#define in2 7
#define enR 8
#define in3 9
#define in4 10

#define trigPin 11
#define echoPin 12
#define MAX_DISTANCE 50

NewPing sonar(trigPin, echoPin, MAX_DISTANCE);

int redPW, greenPW, bluePW;

struct Node {
  char color[10];
  Node* next;
  Node* prev;
};

Node* head = NULL;
Node* tail = NULL;

// ============================
// === SETUP FUNCTIONS =======
// ============================

void setup() {
  // Color sensor config
  pinMode(S0, OUTPUT); pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT); pinMode(S3, OUTPUT);
  digitalWrite(S0, HIGH); digitalWrite(S1, LOW);
  pinMode(sensorOut, INPUT);

  // Motor config
  pinMode(enL, OUTPUT); pinMode(enR, OUTPUT);
  pinMode(in1, OUTPUT); pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT); pinMode(in4, OUTPUT);
  stop();

  // Serial monitor
  Serial.begin(9600);
  Serial.println("Starting Color Maze Solver...");
}

// ============================
// ====== MAIN LOOP ==========
// ============================

void loop() {
  if (!detectWall()) {
    goForward(200);
    delay(700);
    stop();

    char* color = getColor();
    Serial.print("Detected Color: ");
    Serial.println(color);
    
    addToPath(color);  // Save visited color

    moveByColor(color); // Act on color
    free(color);
  } else {
    stop();
    delay(300);
    goBackward(200);
    delay(700);
    turn_right(200);
    delay(600);
    stop();
  }

  delay(200); // small cooldown
}

// ============================
// ==== COLOR DETECTION ======
// ============================

char* getColor() {
  redPW = readColor(LOW, LOW);
  greenPW = readColor(HIGH, HIGH);
  bluePW = readColor(LOW, HIGH);
  delay(100);

  char* result = (char*)malloc(10);
  if (!result) return NULL;

  if (redPW < greenPW && redPW < bluePW)
    strcpy(result, "RED");
  else if (greenPW < redPW && greenPW < bluePW)
    strcpy(result, "GREEN");
  else if (bluePW < redPW && bluePW < greenPW)
    strcpy(result, "BLUE");
  else
    strcpy(result, "UNKNOWN");

  return result;
}

int readColor(bool s2Val, bool s3Val) {
  digitalWrite(S2, s2Val);
  digitalWrite(S3, s3Val);
  return pulseIn(sensorOut, LOW);
}

// ============================
// ====== MOTOR CONTROL =======
// ============================

void goForward(int speed) {
  analogWrite(enL, speed); analogWrite(enR, speed);
  digitalWrite(in1, HIGH); digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH); digitalWrite(in4, LOW);
}

void goBackward(int speed) {
  analogWrite(enL, speed); analogWrite(enR, speed);
  digitalWrite(in1, LOW); digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW); digitalWrite(in4, HIGH);
}

void stop() {
  digitalWrite(in1, LOW); digitalWrite(in2, LOW);
  digitalWrite(in3, LOW); digitalWrite(in4, LOW);
}

void turn_left(int speed) {
  analogWrite(enL, speed); analogWrite(enR, speed);
  digitalWrite(in1, LOW); digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH); digitalWrite(in4, LOW);
}

void turn_right(int speed) {
  analogWrite(enL, speed); analogWrite(enR, speed);
  digitalWrite(in1, HIGH); digitalWrite(in2, LOW);
  digitalWrite(in3, LOW); digitalWrite(in4, HIGH);
}

void uTurn(int speed) {
  turn_right(speed);
  delay(1000); // Adjust based on robot rotation speed
}

// ============================
// === MAZE COLOR ACTION =====
// ============================

void moveByColor(const char* color) {
  if (strcmp(color, "RED") == 0) {
    Serial.println("U-turn");
    uTurn(200);
  } else if (strcmp(color, "GREEN") == 0) {
    Serial.println("Right turn");
    turn_right(200);
    delay(600);
  } else if (strcmp(color, "BLUE") == 0) {
    Serial.println("Left turn");
    turn_left(200);
    delay(600);
  } else {
    Serial.println("Moving forward");
    goForward(200);
    delay(500);
  }

  stop();
  delay(300);
}

// ============================
// ===== WALL DETECTION =======
// ============================

bool detectWall() {
  int distance = sonar.ping_cm();
  return (distance > 0 && distance < 20);
}

// ============================
// ====== PATH STORAGE ========
// ============================

void addToPath(const char* color) {
  Node* newNode = (Node*)malloc(sizeof(Node));
  if (!newNode) return;

  strncpy(newNode->color, color, sizeof(newNode->color) - 1);
  newNode->color[sizeof(newNode->color) - 1] = '\0';
  newNode->next = NULL;
  newNode->prev = NULL;

  if (!head) {
    head = newNode;
    tail = newNode;
  } else {
    tail->next = newNode;
    newNode->prev = tail;
    tail = newNode;
  }
}

// Optional: replay path for debugging
void printPath() {
  Node* curr = head;
  Serial.println("PATH TRAVERSED:");
  while (curr) {
    Serial.println(curr->color);
    curr = curr->next;
  }
}

// Optional: Free list after complete
void clearPath() {
  Node* curr = head;
  while (curr) {
    Node* next = curr->next;
    free(curr);
    curr = next;
  }
  head = tail = NULL;
}
