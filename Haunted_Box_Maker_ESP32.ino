// Haunted Box for Cytron Maker ESP32
// PIR sensor + Servo + RGB
// Horror sound is played on laptop through Serial

#include <ESP32Servo.h>
#include <Adafruit_NeoPixel.h>

// ==================================================
// PIN SETTINGS
// ==================================================

const int pirPin = 21;
const int servoPin = 18;

const int rgbPin = 17;
const int numLeds = 8;

// ==================================================
// SERVO SETTINGS
// ==================================================

const int originalPosition = 180;
const int servoPosition1 = 130;

// ==================================================
// TIMING SETTINGS
// ==================================================

const unsigned long servoInterval = 120;
const unsigned long blinkInterval = 150;

// Sound duration
const unsigned long soundDuration = 5000;

// ==================================================
// RGB
// ==================================================

Adafruit_NeoPixel rgb(
  numLeds,
  rgbPin,
  NEO_GRB + NEO_KHZ800
);

// ==================================================
// SERVO
// ==================================================

Servo hauntedServo;

// ==================================================
// HAUNTED BOX STATE
// ==================================================

bool hauntedMode = false;

int currentAngle = 180;

bool redState = false;

unsigned long lastServoMove = 0;
unsigned long lastBlink = 0;
unsigned long hauntedStartTime = 0;

// ==================================================
// SETUP
// ==================================================

void setup() {

  Serial.begin(115200);

  Serial.println("================================");
  Serial.println("     HAUNTED BOX STARTING");
  Serial.println("================================");

  pinMode(pirPin, INPUT);

  // Setup servo
  hauntedServo.setPeriodHertz(50);
  hauntedServo.attach(servoPin, 500, 2400);
  hauntedServo.write(originalPosition);

  // Setup RGB
  rgb.begin();
  rgb.setBrightness(80);

  // RGB OFF while waiting
  setColor(0, 0, 0);

  // PIR warm-up
  Serial.println("PIR warming up...");
  delay(10000);

  // Wait for PIR to settle
  Serial.println("Checking PIR sensor...");

  while (digitalRead(pirPin) == HIGH) {

    Serial.println("Waiting for PIR to settle...");
    delay(200);
  }

  Serial.println("PIR READY!");
  Serial.println("Waiting for motion...");
}

// ==================================================
// MAIN LOOP
// ==================================================

void loop() {

  // ==================================================
  // WAIT FOR MOTION
  // ==================================================

  if (!hauntedMode && digitalRead(pirPin) == HIGH) {

    Serial.println();
    Serial.println("!!! MOTION DETECTED !!!");

    hauntedMode = true;

    // Start timer
    hauntedStartTime = millis();

    // Reset timers
    lastServoMove = millis();
    lastBlink = millis();

    // Start horror sound
    Serial.println("PLAY");

    // Start red light
    redState = true;
    setColor(255, 0, 0);

    // Start servo
    currentAngle = 180;
    hauntedServo.write(currentAngle);

    Serial.println("HAUNTED MODE ON");
  }

  // ==================================================
  // HAUNTED MODE
  // ==================================================

  if (hauntedMode) {

    unsigned long currentTime = millis();

    // ==================================================
    // SERVO MOVEMENT
    // 180 <-> 130 continuously
    // ==================================================

    if (currentTime - lastServoMove >= servoInterval) {

      lastServoMove = currentTime;

      if (currentAngle == 180) {

        currentAngle = 130;

      } else {

        currentAngle = 180;
      }

      hauntedServo.write(currentAngle);

      Serial.print("Servo: ");
      Serial.println(currentAngle);
    }

    // ==================================================
    // RED BLINK
    // ==================================================

    if (currentTime - lastBlink >= blinkInterval) {

      lastBlink = currentTime;

      redState = !redState;

      if (redState) {

        setColor(255, 0, 0);

      } else {

        setColor(0, 0, 0);
      }
    }

    // ==================================================
    // SOUND TIMER
    // ==================================================

    if (currentTime - hauntedStartTime >= soundDuration) {

      Serial.println();
      Serial.println("Sound finished.");
      Serial.println("STOP");

      // Stop haunted mode
      hauntedMode = false;

      // Stop servo
      hauntedServo.write(originalPosition);

      // RGB OFF
      setColor(0, 0, 0);

      Serial.println("Box closed.");
      Serial.println("Waiting for next motion...");
    }
  }
}

// ==================================================
// RGB FUNCTION
// ==================================================

void setColor(int red, int green, int blue) {

  for (int i = 0; i < numLeds; i++) {

    rgb.setPixelColor(
      i,
      rgb.Color(red, green, blue)
    );
  }

  rgb.show();
}