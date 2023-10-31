#include <LiquidCrystal.h>
#include <Stepper.h>

const int stepsPerRevolution = 200;
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);
int stepCount = 0;
const int buttonPin = 6;

int buttonState = LOW;  // Initialize to LOW
int lastButtonState = LOW;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;  // Adjust as needed

bool isClockwise = true;
Stepper myStepper(stepsPerRevolution, 2, 3, 4, 5);

void setup() {
  lcd.begin(16, 2);
  pinMode(13, OUTPUT);
  pinMode(buttonPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  int sensorReading = analogRead(A1);
  int motorSpeed = map(sensorReading, 0, 1023, 0, 10);

  // Set the motor speed and direction
  myStepper.setSpeed(motorSpeed);
  if (motorSpeed > 0) {
    if (isClockwise) {
      myStepper.step(stepsPerRevolution / 100);
    } else {
      myStepper.step(-stepsPerRevolution / 100);
    }
  }

 //motorSpeed = motorSpeed/10;

  // Read the button state with debouncing
  int reading = digitalRead(buttonPin);
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;

      // Toggle the direction flag if the button is pressed
      if (buttonState == HIGH) {
        isClockwise = !isClockwise;
      }
    }
  }

  lastButtonState = reading;

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Direction: ");
  lcd.print(isClockwise ? "CW " : "CCW");

  lcd.setCursor(0, 1);
  lcd.print("RPM: ");
  lcd.print(motorSpeed);
}

