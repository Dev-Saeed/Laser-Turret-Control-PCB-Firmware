/*
  Main control code for the Control Station ESP32.
  This code performs the following main tasks in each loop iteration:

  1. Receive and parse incoming JSON status from the Turret (via UART0/USB).
  2. Read local buttons and joystick, fill them into the status.
  3. Send the completed status JSON to the gui (via UART1).

  4. Receive and parse incoming JSON command from the gui (via UART1).
  5. Forward the command JSON directly to the Turret (via UART0/USB).

  The setup() function initializes communications and configures I/O.

  To Run it using PlatformIO:
  pio run -e ControlStation -t clean && pio run -e ControlStation && pio run -e ControlStation -t upload && pio device monitor -e ControlStation

*/
 
#include <Arduino.h>
#include "JsonMessenger.h"
#include "StationConfig.h"

// Two messengers: one for each UART
JsonMessenger turretMessenger;  // for USB/UART0 (Turret)
JsonMessenger guiMessenger;  // for UART1 (gui)

void setup() {
  setupConfig();

  // Initialize UART0 for Turret
  // turretMessenger.begin(turret_SERIAL, turret_BAUDRATE, turret_RX, turret_TX); //Default turretMassenger
  // Initialize UART1 for gui
  guiMessenger.begin(gui_SERIAL, gui_BAUDRATE, gui_RX, gui_TX); //Default guiMassenger
  delay(200);  
}

void loop() {
  // ---------------- (1) Receive status from Turret ----------------
  // turretMessenger.receive(); 

  // if (turretMessenger.lastStatus.valid || 1) {
    // Copy motion info from Turret
    // TurretStatus status = turretMessenger.lastStatus;
    TurretStatus status;

    // ---------------- (2) Read local buttons ----------------
    status.buttons.arm          = !digitalRead(BTN_ARM);
    status.buttons.laser_blue   = !digitalRead(BTN_LASER_BLUE);
    status.buttons.laser_green  = !digitalRead(BTN_LASER_GREEN);

    bool stageA = !digitalRead(BTN_STAGE_A);
    bool stageB = !digitalRead(BTN_STAGE_B);
    status.buttons.stage1 = false;
    status.buttons.stage2 = false;
    status.buttons.stage3 = false;
    if (stageA && !stageB)      status.buttons.stage1 = true;
    else if (!stageA && stageB) status.buttons.stage2 = true;
    else if (stageA && stageB)  status.buttons.stage3 = true;

    status.buttons.manual_mode   = !digitalRead(BTN_MANUAL);
    status.buttons.confirm_target = !digitalRead(BTN_CONFIRM);

    // ---------------- (3) Read joystick ----------------
    status.joystick.pot_pan   = readJoystickAxis(JOY_POT_PAN);
    status.joystick.pot_tilt  = readJoystickAxis(JOY_POT_TILT);
    status.joystick.pot_focus = readJoystickAxis(JOY_POT_FOCUS);
    status.joystick.press     = !digitalRead(JOY_BTN_PRESS);

    // ---------------- (4) Send status to gui ----------------
    guiMessenger.sendStatus(status);
    delay(300);
  // }

  // ---------------- (5) Receive command from gui and forward to Turret ----------------
  // guiMessenger.receive();
  // if (guiMessenger.lastCommand.valid || 1) {
  //   ControlCommand guicmd = guiMessenger.lastCommand;

  //   // ------------StartTest----------
  //   guicmd.actuator.buzzer_on = !digitalRead(BTN_ARM);
  //   guicmd.actuator.laser3_on = !digitalRead(BTN_LASER_GREEN);
  //   guicmd.actuator.laser1_on = !digitalRead(BTN_LASER_BLUE) || guicmd.actuator.laser1_on;

  //   float panRPM  = map(readJoystickAxis(JOY_POT_PAN), -100, 100, -5, 5);
  //   float tiltRPM = map(readJoystickAxis(JOY_POT_TILT), -100, 100, -5, 5);

  //   guicmd.motion.pan_speed_rpm = panRPM;
  //   guicmd.motion.tilt_speed_rpm = tiltRPM;
  //   // ------------EndTest----------

  //   turretMessenger.sendCommand(guicmd);
  //   delay(10);
  // }
}