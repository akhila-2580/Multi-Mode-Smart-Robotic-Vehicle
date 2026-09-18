#include <AFMotor.h>
#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define Echo A0
#define Trig A1
#define motor 10
#define Headlight 2

#define Speed 170
#define FollowSpeed 120
#define spoint 103

char value;
char mode = 'M';

int distance;
int Left;
int Right;
int L = 0;
int R = 0;

Servo servo;

LiquidCrystal_I2C lcd(0x27, 16, 2);

AF_DCMotor M1(1);
AF_DCMotor M2(2);
AF_DCMotor M3(3);
AF_DCMotor M4(4);

void setup() {

  Serial.begin(9600);

  pinMode(Trig, OUTPUT);
  pinMode(Echo, INPUT);

  pinMode(Headlight, OUTPUT);
  digitalWrite(Headlight, LOW);

  servo.attach(motor);
  servo.write(spoint);

  M1.setSpeed(Speed);
  M2.setSpeed(Speed);
  M3.setSpeed(Speed);
  M4.setSpeed(Speed);

  Stop();

  Wire.begin();

  lcd.init();
  lcd.backlight();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("SMART CAR");
  lcd.setCursor(0, 1);
  lcd.print("MANUAL");

  delay(2000);
}

void loop() {

  if (Serial.available() > 0) {

    value = Serial.read();

    if (value == 'M' ||
        value == 'A' ||
        value == 'V' ||
        value == 'G' ||
        value == 'H') {

      mode = value;

      Stop();

      if (mode == 'H') {
        servo.write(spoint);
      }

      if (mode == 'M') {
        showMode("MANUAL MODE");
      }
      else if (mode == 'A') {
        showMode("AUTO MODE");
      }
      else if (mode == 'V') {
        showMode("VOICE MODE");
      }
      else if (mode == 'G') {
        showMode("GESTURE MODE");
      }
      else if (mode == 'H') {
        showMode("HUMAN FOLLOW MODE");
      }
    }

    else if (value == 'K') {
      headlightsOn();
    }

    else if (value == 'k') {
      headlightsOff();
    }

    else if (mode == 'M' ||
             mode == 'V' ||
             mode == 'G') {

      Bluetoothcontrol();
    }
  }

  if (mode == 'A') {
    Obstacle();
  }

  if (mode == 'H') {
    HumanFollow();
  }
}

void showMode(const char *modeName) {

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("SMART CAR");

  lcd.setCursor(0, 1);
  lcd.print(modeName);
}

void Bluetoothcontrol() {

  if (value == 'F') {
    forward();
  }
  else if (value == 'B') {
    backward();
  }
  else if (value == 'L') {
    left();
  }
  else if (value == 'R') {
    right();
  }
  else if (value == 'S') {
    Stop();
  }
}

void Obstacle() {

  distance = ultrasonic();

  if (distance <= 12) {

    Stop();

    delay(100);

    backward();

    delay(300);

    Stop();

    delay(200);

    L = leftsee();

    servo.write(spoint);

    delay(300);

    R = rightsee();

    servo.write(spoint);

    delay(300);

    if (L > R) {

      left();

      delay(500);

      Stop();

      delay(200);

    }
    else {

      right();

      delay(500);

      Stop();

      delay(200);
    }

  }
  else {

    forward();
  }
}

void HumanFollow() {

  servo.write(spoint);

  distance = ultrasonic();

  if (distance > 100) {

    Stop();

    return;
  }

  if (distance < 10) {

    backwardSlow();

    return;
  }

  if (distance >= 10 && distance <= 12) {

    Stop();

    return;
  }

  if (distance > 12 && distance <= 100) {

    forwardSlow();

    return;
  }

  Stop();
}

int ultrasonic() {

  digitalWrite(Trig, LOW);

  delayMicroseconds(4);

  digitalWrite(Trig, HIGH);

  delayMicroseconds(10);

  digitalWrite(Trig, LOW);

  long t = pulseIn(Echo, HIGH, 30000);

  if (t == 0) {
    return 200;
  }

  long cm = t / 29 / 2;

  return cm;
}

int leftsee() {

  servo.write(180);

  delay(600);

  Left = ultrasonic();

  return Left;
}

int rightsee() {

  servo.write(20);

  delay(600);

  Right = ultrasonic();

  return Right;
}

void headlightsOn() {

  digitalWrite(Headlight, HIGH);
}

void headlightsOff() {

  digitalWrite(Headlight, LOW);
}

void forward() {

  M1.setSpeed(Speed);
  M2.setSpeed(Speed);
  M3.setSpeed(Speed);
  M4.setSpeed(Speed);

  M1.run(FORWARD);
  M2.run(FORWARD);
  M3.run(FORWARD);
  M4.run(FORWARD);
}

void backward() {

  M1.setSpeed(Speed);
  M2.setSpeed(Speed);
  M3.setSpeed(Speed);
  M4.setSpeed(Speed);

  M1.run(BACKWARD);
  M2.run(BACKWARD);
  M3.run(BACKWARD);
  M4.run(BACKWARD);
}

void left() {

  M1.run(FORWARD);
  M2.run(FORWARD);

  M3.run(BACKWARD);
  M4.run(BACKWARD);
}

void right() {

  M1.run(BACKWARD);
  M2.run(BACKWARD);

  M3.run(FORWARD);
  M4.run(FORWARD);
}

void Stop() {

  M1.run(RELEASE);
  M2.run(RELEASE);
  M3.run(RELEASE);
  M4.run(RELEASE);
}

void forwardSlow() {

  M1.setSpeed(FollowSpeed);
  M2.setSpeed(FollowSpeed);
  M3.setSpeed(FollowSpeed);
  M4.setSpeed(FollowSpeed);

  M1.run(FORWARD);
  M2.run(FORWARD);
  M3.run(FORWARD);
  M4.run(FORWARD);
}

void backwardSlow() {

  M1.setSpeed(FollowSpeed);
  M2.setSpeed(FollowSpeed);
  M3.setSpeed(FollowSpeed);
  M4.setSpeed(FollowSpeed);

  M1.run(BACKWARD);
  M2.run(BACKWARD);
  M3.run(BACKWARD);
  M4.run(BACKWARD);
}