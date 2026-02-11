// Pins
const int sensorPin = 2;       
const int button1Pin = 3;      
const int button2Pin = 4;      
const int button3Pin = 5;      
const int ledPin = 13;         

int motorPins[4] = {8, 9, 10, 11};  

int steps[4][4] = {
  {1, 0, 0, 0},
  {0, 1, 0, 0},
  {0, 0, 1, 0},
  {0, 0, 0, 1}
};

int stepIndex = 0;

// Previous states
int prevButton1 = -1;
int prevButton2 = -1;
int prevButton3 = -1;

void setup() {
  Serial.begin(9600);
  pinMode(sensorPin, INPUT);
  pinMode(button1Pin, INPUT_PULLUP);
  pinMode(button2Pin, INPUT_PULLUP);
  pinMode(button3Pin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  for (int i = 0; i < 4; i++) pinMode(motorPins[i], OUTPUT);
}

void loop() {
  // LED control
  if (digitalRead(sensorPin) == HIGH) digitalWrite(ledPin, HIGH);
  else digitalWrite(ledPin, LOW);

  // Motor control
  if (digitalRead(button1Pin) == LOW) stepMotor();
  else stopMotor();

  // Read buttons
  int button1State = digitalRead(button1Pin) == LOW ? 1 : 0;
  int button2State = digitalRead(button2Pin) == LOW ? 1 : 0;
  int button3State = digitalRead(button3Pin) == LOW ? 1 : 0;

  // Send only if any button changed
  if (button1State != prevButton1 || button2State != prevButton2 || button3State != prevButton3) {
    Serial.print(button1State);
    Serial.print(",");
    Serial.print(button2State);
    Serial.print(",");
    Serial.println(button3State);

    prevButton1 = button1State;
    prevButton2 = button2State;
    prevButton3 = button3State;
  }

}

// Stepper functions
void stepMotor() {
  for (int i = 0; i < 4; i++) digitalWrite(motorPins[i], steps[stepIndex][i]);
  stepIndex++;
  if (stepIndex >= 4) stepIndex = 0;
  delay(3); // MOTOR SPEED KEPT
}

void stopMotor() {
  for (int i = 0; i < 4; i++) digitalWrite(motorPins[i], LOW);
}
