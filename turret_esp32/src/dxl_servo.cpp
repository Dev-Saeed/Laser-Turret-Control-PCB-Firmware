#include "dxl_servo.h"



// Dynamixel Servo Object (XC430-T150BB-T)
#define DXL_ID 1
const float DXL_PROTOCOL_VERSION = 2.0;
const int DXL_DIR_PIN = -1;
const int DXL_BAUDRATE = 1000000; // 1 Mbps
Dynamixel2Arduino dxl(Servo_SERIAL, DXL_DIR_PIN);
using namespace ControlTableItem;

void initDynamixel() {
  dxl.begin(DXL_BAUDRATE);
  dxl.setPortProtocolVersion(DXL_PROTOCOL_VERSION);
  dxl.torqueOff(DXL_ID);
  dxl.setOperatingMode(DXL_ID, OP_VELOCITY);
  dxl.torqueOn(DXL_ID);
  dxl.setGoalVelocity(DXL_ID, 0); 
}

float get_user_pan_angle() {
  static float last_angle = 0;
  static String input = "";
  while (DEBUG_SERIAL.available()) {
    char c = DEBUG_SERIAL.read();
    if (c == '\n' || c == '\r') {
      if (input.length() > 0) {
        float angle = input.toFloat();
        if (angle < 0) angle = 0;
        if (angle > 360) angle = 360;
        last_angle = angle;
        DEBUG_SERIAL.print("Moving to pan angle: ");
        DEBUG_SERIAL.println(last_angle);
        input = "";
      }
      DEBUG_SERIAL.println("Please enter the pan angle (0-360):");
    } else {
      input += c;
    }
  }
  return last_angle;
}

void dxl_go_to(int angle_deg) {
  // Clamp angle to [0, 360]
  if (angle_deg < 0) angle_deg = 0;
  if (angle_deg > 360) angle_deg = 360;

  // Map angle to digital value (0-4095)
  int position = map((int)angle_deg, 0, 360, 0, 4096);

  dxl.setGoalPosition(DXL_ID, position);
}

void dxl_turn_to(int angle_deg) {
  // Clamp angle to [-92160, 92160]
  if (angle_deg < - 360 * 256) angle_deg = - 360 * 256;
  if (angle_deg > 360 * 256) angle_deg = 360 * 256;

  // Map angle to digital value (0-4095)
  int position = map((int)angle_deg, 0, 92160, 0, 1048575);

  dxl.setGoalPosition(DXL_ID, position);
}

void dxl_speed_to(float rpm) {
  if (rpm > PAN_SPEED_MAX) rpm = PAN_SPEED_MAX; // Max: 234.267 RPM
  // Map RPM to digital value (0-1023)
  int velocity = map((int)rpm, 0, PAN_SPEED_MAX, 0, 1023);

  dxl.setGoalVelocity(DXL_ID, velocity);
}

void dxl_pan_speed_control(float rpm, float min_angle, float max_angle) {
  // Clamp rpm to max allowed
  if (rpm >  PAN_SPEED_MAX) rpm =  PAN_SPEED_MAX;
  if (rpm < -PAN_SPEED_MAX) rpm = -PAN_SPEED_MAX;

  // Read current position from servo (digital value)
  int current_pos = dxl.readControlTableItem(PRESENT_POSITION, DXL_ID);
  // Convert digital position to angle (0-4095 <-> 0-360)
  float current_angle = map(current_pos, 0, 4095, 0, 360);

  // Check limits
  if ((current_angle <= min_angle && rpm < 0) || (current_angle >= max_angle && rpm > 0)) {
    // At limit and trying to go further, stop
    rpm = 0;
  }

  // Map RPM to digital value (0-3400)
  int velocity = map((int)abs(rpm), 0, 0.299*1023, 0, 1023);
  if (rpm < 0) velocity = -velocity;

  dxl.setGoalVelocity(DXL_ID, velocity);
}

void dxl_OP_POSITION() {
  dxl.torqueOff(DXL_ID);
  dxl.setOperatingMode(DXL_ID, OP_POSITION);
  dxl.torqueOn(DXL_ID);
}

void dxl_OP_VELOCITY() {
  dxl.torqueOff(DXL_ID);
  dxl.setOperatingMode(DXL_ID, OP_VELOCITY);
  dxl.torqueOn(DXL_ID);
}

void dxl_OP_EXTENDED() {
  dxl.torqueOff(DXL_ID);
  dxl.setOperatingMode(DXL_ID, OP_EXTENDED_POSITION);
  dxl.torqueOn(DXL_ID);
}

void scan_dynemixel() {
  #define MAX_BAUD  8
  const int32_t buad[MAX_BAUD] = {9600, 57600, 115200, 1000000, 2000000, 3000000, 4000000, 4500000};
  // Scan for Dynamixel servos on the bus
  int8_t index = 0;
  int8_t found_dynamixel = 0;

  DEBUG_SERIAL.begin(115200);
  while(!DEBUG_SERIAL); // Wait until the serial port is opened

  for(int8_t protocol = 1; protocol < 3; protocol++) {
    dxl.setPortProtocolVersion((float)protocol);
    DEBUG_SERIAL.print("SCAN PROTOCOL ");
    DEBUG_SERIAL.println(protocol);
    
    for(index = 0; index < MAX_BAUD; index++) {
      DEBUG_SERIAL.print("SCAN BAUDRATE ");
      DEBUG_SERIAL.println(buad[index]);
      dxl.begin(buad[index]);
      for(int id = 0; id < DXL_BROADCAST_ID; id++) {
        if(dxl.ping(id)) {
          DEBUG_SERIAL.print("ID : ");
          DEBUG_SERIAL.print(id);
          DEBUG_SERIAL.print(", Model Number: ");
          DEBUG_SERIAL.println(dxl.getModelNumber(id));
          found_dynamixel++;
        }
      }
    }
  }
  
  DEBUG_SERIAL.print("Total ");
  DEBUG_SERIAL.print(found_dynamixel);
  DEBUG_SERIAL.println(" DYNAMIXEL(s) found!");
}

void change_baudrate(int32_t new_baudrate) {
  if (dxl.ping(DXL_ID)) {
    DEBUG_SERIAL.print("Changing baudrate to ");
    DEBUG_SERIAL.println(new_baudrate);
    
    dxl.torqueOff(DXL_ID);
    dxl.setBaudrate(DXL_ID, new_baudrate);
    dxl.begin(new_baudrate);
    
    DEBUG_SERIAL.println("Baudrate changed successfully!");
  } else {
    DEBUG_SERIAL.println("Failed to ping DYNAMIXEL. Cannot change baudrate.");
  }
}

void dxl_info() {
  DEBUG_SERIAL.println("=== DXL Servo Info ===");
  DEBUG_SERIAL.print("ID: ");
  DEBUG_SERIAL.println(DXL_ID);
  DEBUG_SERIAL.print("Model Number: ");
  DEBUG_SERIAL.println(dxl.readControlTableItem(MODEL_NUMBER, DXL_ID));
  DEBUG_SERIAL.print("Position: ");
  DEBUG_SERIAL.println(dxl.readControlTableItem(PRESENT_POSITION, DXL_ID) * 360 / 4095);
  DEBUG_SERIAL.print("Speed: ");
  DEBUG_SERIAL.println(dxl.readControlTableItem(PRESENT_VELOCITY, DXL_ID) * 0.299);
  DEBUG_SERIAL.print("Voltage: ");
  DEBUG_SERIAL.println(dxl.readControlTableItem(PRESENT_VOLTAGE, DXL_ID));
  DEBUG_SERIAL.print("Temperature: ");
  DEBUG_SERIAL.println(dxl.readControlTableItem(PRESENT_TEMPERATURE, DXL_ID));
  DEBUG_SERIAL.print("Current: ");
  DEBUG_SERIAL.println(dxl.readControlTableItem(PRESENT_CURRENT, DXL_ID));
  DEBUG_SERIAL.println("==================");
}

void dxl_all_info() {
  DEBUG_SERIAL.println("=== DXL Servo Info ===");
  DEBUG_SERIAL.print("ID: ");
  DEBUG_SERIAL.println(DXL_ID);
  DEBUG_SERIAL.print("Model Number: ");
  DEBUG_SERIAL.println(dxl.readControlTableItem(MODEL_NUMBER, DXL_ID));
  DEBUG_SERIAL.print("Firmware Version: ");
  DEBUG_SERIAL.println(dxl.readControlTableItem(FIRMWARE_VERSION, DXL_ID));
  DEBUG_SERIAL.print("Protocol Version: ");
  DEBUG_SERIAL.println(dxl.readControlTableItem(PROTOCOL_VERSION, DXL_ID));
  DEBUG_SERIAL.print("Baud Rate: ");
  DEBUG_SERIAL.println(dxl.readControlTableItem(BAUD_RATE, DXL_ID));
  DEBUG_SERIAL.print("Operating Mode: ");
  DEBUG_SERIAL.println(dxl.readControlTableItem(OPERATING_MODE, DXL_ID));
  DEBUG_SERIAL.print("CW Angle Limit: ");
  DEBUG_SERIAL.println(dxl.readControlTableItem(CW_ANGLE_LIMIT, DXL_ID));
  DEBUG_SERIAL.print("CCW Angle Limit: ");
  DEBUG_SERIAL.println(dxl.readControlTableItem(CCW_ANGLE_LIMIT, DXL_ID));
  DEBUG_SERIAL.print("Temperature Limit: ");
  DEBUG_SERIAL.println(dxl.readControlTableItem(TEMPERATURE_LIMIT, DXL_ID));
  DEBUG_SERIAL.print("Min Voltage Limit: ");
  DEBUG_SERIAL.println(dxl.readControlTableItem(MIN_VOLTAGE_LIMIT, DXL_ID)/10);
  DEBUG_SERIAL.print("Max Voltage Limit: ");
  DEBUG_SERIAL.println(dxl.readControlTableItem(MAX_VOLTAGE_LIMIT, DXL_ID)/10);
  DEBUG_SERIAL.print("Goal Position (Degree): ");
  DEBUG_SERIAL.println(dxl.readControlTableItem(GOAL_POSITION, DXL_ID) * 360/4095);
  DEBUG_SERIAL.print("Present Position (Degree): ");
  DEBUG_SERIAL.println(dxl.readControlTableItem(PRESENT_POSITION, DXL_ID) * 360/4095);
  DEBUG_SERIAL.print("Goal Velocity (RPM): ");
  DEBUG_SERIAL.println(dxl.readControlTableItem(GOAL_VELOCITY, DXL_ID) * 0.229);
  DEBUG_SERIAL.print("Present Velocity (RPM): ");
  DEBUG_SERIAL.println(dxl.readControlTableItem(PRESENT_VELOCITY, DXL_ID) * 0.229);
  DEBUG_SERIAL.print("Goal Current: ");
  DEBUG_SERIAL.println(dxl.readControlTableItem(GOAL_CURRENT, DXL_ID));
  DEBUG_SERIAL.print("Present Current: ");
  DEBUG_SERIAL.println(dxl.readControlTableItem(PRESENT_CURRENT, DXL_ID));
  DEBUG_SERIAL.print("Present Voltage: ");
  DEBUG_SERIAL.println(dxl.readControlTableItem(PRESENT_VOLTAGE, DXL_ID) / 10);
  DEBUG_SERIAL.print("Present Temperature: ");
  DEBUG_SERIAL.println(dxl.readControlTableItem(PRESENT_TEMPERATURE, DXL_ID));
  DEBUG_SERIAL.print("Moving: ");
  DEBUG_SERIAL.println(dxl.readControlTableItem(MOVING, DXL_ID));
  DEBUG_SERIAL.println("==================");
}