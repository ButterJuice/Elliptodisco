// Pins
const int sensorPin = 2;   // capteur magnétique
const int buttonPin = 3;   // bouton pour moteur
const int ledPin = 13;     // LED intégrée

int motorPins[4] = {8, 9, 10, 11};

// séquence pas à pas pour moteur
int steps[4][4] = {
  {1, 0, 0, 0},
  {0, 1, 0, 0},
  {0, 0, 1, 0},
  {0, 0, 0, 1}
};

int stepIndex = 0;

void setup() {
  pinMode(sensorPin, INPUT);           // capteur magnétique
  pinMode(buttonPin, INPUT_PULLUP);    // bouton
  pinMode(ledPin, OUTPUT);             // LED
  for (int i = 0; i < 4; i++) {
    pinMode(motorPins[i], OUTPUT);     // moteur
  }
}

void loop() {
  // --- Gérer LED via capteur ---
  if (digitalRead(sensorPin) == HIGH) {  // aimant détecté
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }

  // --- Gérer moteur via bouton ---
  if (digitalRead(buttonPin) == LOW) {   // bouton appuyé
    stepMotor();
  } else {
    stopMotor();
  }
}

// Fonction pour faire tourner le moteur
void stepMotor() {
  for (int i = 0; i < 4; i++) {
    digitalWrite(motorPins[i], steps[stepIndex][i]);
  }
  stepIndex++;
  if (stepIndex >= 4) stepIndex = 0;
  delay(3); // vitesse moteur
}

// Fonction pour arrêter le moteur
void stopMotor() {
  for (int i = 0; i < 4; i++) {
    digitalWrite(motorPins[i], LOW);
  }
}
