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
int prevButton2 = -1;
int prevButton3 = -1;
int prevSensor  = -1;

// Speed calculation
unsigned long lastTriggerTime = 0;

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

  // --- Sensor ---
  int sensorState = digitalRead(sensorPin) == HIGH ? 1 : 0;
  digitalWrite(ledPin, sensorState);

  // --- Motor via button 1 ---
  if (digitalRead(button1Pin) == LOW) stepMotor();
  else stopMotor();

  // --- Button 2 & 3 ---
  int button2State = digitalRead(button2Pin) == LOW ? 1 : 0;
  int button3State = digitalRead(button3Pin) == LOW ? 1 : 0;

  if (button2State != prevButton2 || button3State != prevButton3 || sensorState != prevSensor) {
    Serial.print(button2State);
    Serial.print(",");
    Serial.print(button3State);
    Serial.print(",");
    Serial.println(sensorState);

    prevButton2 = button2State;
    prevButton3 = button3State;
    prevSensor  = sensorState;
  }

  // --- Speed calculation ---
  static int prevSensorState = 0;

  // Detect rising edge
  if (sensorState == 1 && prevSensorState == 0) {
    unsigned long now = micros();
    unsigned long interval = now - lastTriggerTime;
    lastTriggerTime = now;

    // Send to Python
    Serial.print("SPEED:");
    Serial.println(interval);
  }

  prevSensorState = sensorState;
}

// Stepper functions
void stepMotor() {
  for (int i = 0; i < 4; i++) digitalWrite(motorPins[i], steps[stepIndex][i]);
  stepIndex++;
  if (stepIndex >= 4) stepIndex = 0;
  delay(3);
}

void stopMotor() {
  for (int i = 0; i < 4; i++) digitalWrite(motorPins[i], LOW);
}
