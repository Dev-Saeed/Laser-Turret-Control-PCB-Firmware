#include "stepper_focus_driver.h"
#include "STM_Link.h"

// Stepper pins
static int pin1, pin2, pin3, pin4;

// Internal step tracking
static long current_steps = 0;
static float last_target_angle = -9999.0; // invalid init to ensure first move

// Gear ratio (set externally)
float focus_gear_ratio = 1.0;

// Constants
static const float steps_per_rev = 4096.0;
static const int step_delay_us = 900;

// Step sequence
static const int step_seq[8][4] = {
    {1, 0, 0, 0},
    {1, 1, 0, 0},
    {0, 1, 0, 0},
    {0, 1, 1, 0},
    {0, 0, 1, 0},
    {0, 0, 1, 1},
    {0, 0, 0, 1},
    {1, 0, 0, 1}
};

// STM32 link
STM_Link STMLink;
bool STM_input[7];

static void setStep(int a, int b, int c, int d) {
    digitalWrite(pin1, a);
    digitalWrite(pin2, b);
    digitalWrite(pin3, c);
    digitalWrite(pin4, d);
}

void init_BYJ(int s1, int s2, int s3, int s4) {
    pin1 = s1;
    pin2 = s2;
    pin3 = s3;
    pin4 = s4;

    pinMode(pin1, OUTPUT);
    pinMode(pin2, OUTPUT);
    pinMode(pin3, OUTPUT);
    pinMode(pin4, OUTPUT);

    setStep(0, 0, 0, 0);
    current_steps = 0;
    last_target_angle = -9999.0;
}

void Focus_home() {
    STMLink.readLimitStates(STM_input);
    while (STM_input[2]) {
        for (int i = 7; i >= 0; i--) {
            setStep(
                step_seq[i][0],
                step_seq[i][1],
                step_seq[i][2],
                step_seq[i][3]
            );
            delay(3);
        }
        STMLink.readLimitStates(STM_input);
    }

    setStep(0, 0, 0, 0);
    current_steps = 0;
    last_target_angle = 0;
}

void stepper_aim_angle_control(float target_angle, float angle_min, float angle_max) {
    target_angle = constrain(target_angle, angle_min, angle_max);

    // Ignore repeated calls to same angle
    if (abs(target_angle - last_target_angle) < 0.1) return;

    float angle_per_step = 360.0 / (steps_per_rev * focus_gear_ratio);
    long target_steps = round(target_angle / angle_per_step);
    long delta = target_steps - current_steps;

    int dir = (delta > 0) ? 1 : -1;

    for (long i = 0; i < abs(delta); i++) {
        int idx = (dir > 0) ? (i % 8) : (7 - (i % 8));
        setStep(
            step_seq[idx][0],
            step_seq[idx][1],
            step_seq[idx][2],
            step_seq[idx][3]
        );
        delayMicroseconds(step_delay_us);
    }

    setStep(0, 0, 0, 0);
    current_steps = target_steps;
    last_target_angle = target_angle;
}
