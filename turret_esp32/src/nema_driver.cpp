#include "nema_driver.h"
#include "driver/timer.h"

// Internal static variables
static int en_pin_local, dir_pin_local, step_pin_local, com_pin_local, limit_pin_local;
static float steps_per_rev_local = 1600.0f;
static float pan_gear_ratio = 185.0f / 35.0f;
static AccelStepper stepper(AccelStepper::DRIVER, 0, 0);
static float currentAngle = 0.0f;

// Timer variables
hw_timer_t *stepperTimer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

// Timer ISR — optional if you want continuous motion (currently not used)
void IRAM_ATTR onStepperTimer() {
    portENTER_CRITICAL_ISR(&timerMux);
    stepper.runSpeed();
    portEXIT_CRITICAL_ISR(&timerMux);
}

void init_NEMA(int enPin, int dirPin, int stepPin, int comPin, int limitPin, float stepsPerRev) {
    en_pin_local = enPin;
    dir_pin_local = dirPin;
    step_pin_local = stepPin;
    limit_pin_local = limitPin;
    steps_per_rev_local = stepsPerRev;
    com_pin_local = comPin;

    pinMode(en_pin_local, OUTPUT);
    digitalWrite(en_pin_local, LOW); // Enable driver

    pinMode(limit_pin_local, INPUT_PULLUP);
    pinMode(com_pin_local, OUTPUT);
    digitalWrite(com_pin_local, LOW); // Set COM pin low

    stepper = AccelStepper(AccelStepper::DRIVER, step_pin_local, dir_pin_local);
    stepper.setMaxSpeed(3000);
    stepper.setAcceleration(2000);

    // Optional: Enable timer interrupt if needed
    // stepperTimer = timerBegin(0, 80, true);
    // timerAttachInterrupt(stepperTimer, &onStepperTimer, true);
    // timerAlarmWrite(stepperTimer, 2000, true); // 2ms
    // timerAlarmEnable(stepperTimer);
}

// ----- Homing -----
void Pan_home() {
    stepper.setMaxSpeed(500);
    stepper.setSpeed(-500);
    while (digitalRead(limit_pin_local) == LOW) {
        stepper.runSpeed(); // Blocking
        delay(1);
    }
    
    stepper.setCurrentPosition(0);
    currentAngle = 0.0f;

    while (digitalRead(limit_pin_local) == LOW) {
        stepper.runSpeed(); // Blocking
        delay(1);
    }
}

// ----- Move to specific angle -----
void Pan_goTo(float panAngleDeg) {
    float motorAngle = panAngleDeg * pan_gear_ratio;
    long targetSteps = (long)((motorAngle / 360.0f) * steps_per_rev_local);

    stepper.setMaxSpeed(1200);
    stepper.setAcceleration(800);
    stepper.moveTo(targetSteps);
    stepper.runToPosition(); // Blocking
    currentAngle = panAngleDeg;
}

// ----- Calibration routine -----
void Pan_Calibrate() {
    Pan_home();
    Pan_goTo(135.0f);
    delay(10000);
    Pan_goTo(145.0f);
    delay(200);
}

// ----- Speed Control (call from loop) -----
void nema_pan_speed_control(float rpmCommand, float minAngle, float maxAngle) {
    if (rpmCommand > PAN_SPEED_MAX) rpmCommand = PAN_SPEED_MAX;
    if (rpmCommand < -PAN_SPEED_MAX) rpmCommand = -PAN_SPEED_MAX;

    float motorRpm = rpmCommand * pan_gear_ratio;
    float stepsPerSec = (motorRpm * steps_per_rev_local) / 60.0f;
    stepper.setSpeed(stepsPerSec);

    float stepAngle = (stepsPerSec > 0 ? 1 : -1) * (360.0f / steps_per_rev_local);
    float nextAngle = currentAngle + stepAngle;

    float minMotorAngle = minAngle * pan_gear_ratio;
    float maxMotorAngle = maxAngle * pan_gear_ratio;

    if ((stepsPerSec > 0 && nextAngle > maxMotorAngle) ||
        (stepsPerSec < 0 && nextAngle < minMotorAngle)) {
        stepper.setSpeed(0);
    } else {
        // ✅ Update angle only if a step was taken
        if (stepper.runSpeed()) {
            currentAngle = nextAngle;
        }
    }
}

// ----- Getters -----
float pan_getAngle() {
    return currentAngle / pan_gear_ratio;
}

float pan_getVelocity() {
    float motorRpm = stepper.speed() * 60.0f / steps_per_rev_local;
    return motorRpm / pan_gear_ratio;
}
