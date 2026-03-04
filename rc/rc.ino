// Motor 1
#define EN1  10
#define IN1  9
#define IN2  8
// Motor 2
#define EN2  6
#define IN3  4
#define IN4  3
// Buttons
#define FWD_BTN  12
#define REV_BTN  11
#define STOP_BTN 13

bool movingForward = false;
bool movingReverse = false;
bool lastFwdState = HIGH;
bool lastRevState = HIGH;
bool lastStopState = HIGH;

void setup() {
  pinMode(EN1, OUTPUT);
  pinMode(EN2, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(FWD_BTN, INPUT_PULLUP);
  pinMode(REV_BTN, INPUT_PULLUP);
  pinMode(STOP_BTN, INPUT_PULLUP);

  analogWrite(EN1, 100);
  analogWrite(EN2, 100);
}

void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  movingForward = false;
  movingReverse = false;
}

void loop() {
  bool fwdReading = digitalRead(FWD_BTN);
  bool revReading = digitalRead(REV_BTN);
  bool stopReading = digitalRead(STOP_BTN);

  if (lastFwdState == HIGH && fwdReading == LOW) {
    if (movingForward) {
      stopMotors();
    } else {
      movingReverse = false;
      movingForward = true;
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
    }
    delay(200);
  }

  if (lastRevState == HIGH && revReading == LOW) {
    if (movingReverse) {
      stopMotors();
    } else {
      movingForward = false;
      movingReverse = true;
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);
    }
    delay(200);
  }

  if (lastStopState == HIGH && stopReading == LOW) {
    stopMotors();
    delay(200);
  }

  lastFwdState = fwdReading;
  lastRevState = revReading;
  lastStopState = stopReading;
}