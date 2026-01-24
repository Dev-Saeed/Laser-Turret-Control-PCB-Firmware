#include "st_driver.h"


#define ST_ID 1
const int ST_BAUDRATE = 1000000;
SMS_STS st;

void init_ST(HardwareSerial &serialPort, uint32_t baud, int rxPin, int txPin) {
  serialPort.begin(baud, SERIAL_8N1, rxPin, txPin);
  st.pSerial = &serialPort;
}

void Tilt_home() {
  st_OP_POSITION();
  delay(200);
  st_go_to(230, 20);
}

void Tilt_Calibrate() {
  st_OP_POSITION();
  st_go_to(50, 50);
  delay(1000);
  st_go_to(20, 15);
  delay(3000);
  st_go_to(120, 15);
  delay(3000);
  st_go_to(20, 5);
}

static inline float fmapf(float x, float in_min, float in_max, float out_min, float out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void st_tilt_speed_control(float rpm, float min_angle, float max_angle) {
    // Clamp rpm
    if (rpm > TILT_SPEED_MAX)  rpm = TILT_SPEED_MAX;
    if (rpm < -TILT_SPEED_MAX) rpm = -TILT_SPEED_MAX;

    // Read current position (0..4095) -> degrees
    float current_pos   = (float)st.ReadPos(ST_ID);
    float current_angle = (current_pos / 4095.0f) * 360.0f;

    // Check limits
    if ((current_angle <= min_angle && rpm < 0) ||
        (current_angle >= max_angle && rpm > 0)) {
        rpm = 0.0f;
    }

    // Convert RPM to digital speed units (0..3400) in float
    float vel_f = fmapf(fabsf(rpm), 0.0f, 50.0f, 0.0f, 3400.0f);

    // Apply sign
    if (rpm < 0) vel_f = -vel_f;

    // Send float directly
    st.WriteSpe(ST_ID, vel_f, TILT_ACCELERATION_MAX);
}

// void st_tilt_speed_control(float rpm, float min_angle, float max_angle) {
//   // Clamp rpm to max allowed
//   if (rpm > TILT_SPEED_MAX) rpm = TILT_SPEED_MAX;
//   if (rpm < -TILT_SPEED_MAX) rpm = -TILT_SPEED_MAX;

//   // Read current position from servo (digital value)
//   float current_pos = st.ReadPos(ST_ID);
//   // Convert digital position to angle (0-4095 <-> 0-360)
//   float current_angle = map(current_pos, 0, 4095, 0, 360);

//   // Check limits
//   if ((current_angle <= min_angle && rpm < 0) || (current_angle >= max_angle && rpm > 0)) {
//     // At limit and trying to go further, stop
//     rpm = 0;
//   }

//   // Map RPM to digital value (0-3400)
//   int velocity = map((int)abs(rpm), 0, 50, 0, 3400);
//   if (rpm < 0) velocity = -velocity;

//   st.WriteSpe(ST_ID, velocity, TILT_ACCELERATION_MAX);
// }

// ----- Getter -----
float tilt_getAngle() {
  int pos = st.ReadPos(ST_ID);
  return (float)pos * 360.0f / 4095.0f; // Convert to degrees
}

float tilt_getVelocity() {
  int speed = st.ReadSpeed(ST_ID);
  return (float)speed * 50.0f / 3400.0f; // Convert to RPM
}

float tilt_getVoltage() {
  int voltage = st.ReadVoltage(ST_ID);
  return (float)voltage / 10.0f; // Convert to volts
}

void st_go_to(float angle_deg, int rpm) {
  if (angle_deg < TILT_ANGLE_MIN) angle_deg = TILT_ANGLE_MIN;
  if (angle_deg > TILT_ANGLE_MAX) angle_deg = TILT_ANGLE_MAX;
  if (rpm > TILT_SPEED_MAX) rpm = TILT_SPEED_MAX;
  if (rpm < -TILT_SPEED_MAX) rpm = -TILT_SPEED_MAX;

  // Map angle to digital value (0-4095)
  int position = map((int)angle_deg, 0, 360, 0, 4095);
  int velocity = map((int)abs(rpm), 0, 50, 0, 3400);

  st.WritePosEx(ST_ID, position, velocity, TILT_ACCELERATION_MAX);
  // st.WritePosEx(ST_ID, 0, 0, 0);
}
void st_speed_to(float rpm) {
  if (rpm > TILT_SPEED_MAX) rpm = TILT_SPEED_MAX; // Max: 50 RPM
  // Map RPM to digital value (0-1023)
  int velocity = map((int)rpm, 0, 50, 0, 3400);

  st.setOperatingMode(ST_ID, OP_VELOCITY);
  st.WriteSpe(ST_ID, velocity, TILT_ACCELERATION_MAX);
  delay(10);
}
void st_OP_POSITION() {
  st.setOperatingMode(ST_ID, OP_POSITION);
}

void st_OP_VELOCITY() {
  st.setOperatingMode(ST_ID, OP_VELOCITY);
}

void st_info() {
  int Pos, Speed, Load, Voltage, Temper, Current;
  DEBUG_SERIAL.println("=== ST Servo Info ===");
  DEBUG_SERIAL.print("ID: ");
  DEBUG_SERIAL.println(ST_ID);
  DEBUG_SERIAL.print("Model Number: ");
  DEBUG_SERIAL.println("N/A"); // Not available for ST
  if (st.FeedBack(ST_ID) != -1) {
    Pos = st.ReadPos(-1) * 360 / 4095; // Convert to degrees
    Speed = st.ReadSpeed(-1) * 50/3400; // Convert to RPM
    Load = st.ReadLoad(-1) / 10;
    Voltage = st.ReadVoltage(-1) / 10; // Convert to volts
    Temper = st.ReadTemper(-1);
    Current = st.ReadCurrent(-1) * 6.5; // Convert to mA

    DEBUG_SERIAL.print("Position (Degree): ");
    DEBUG_SERIAL.print(Pos); DEBUG_SERIAL.println("°");
    DEBUG_SERIAL.print("Speed : ");
    DEBUG_SERIAL.println(Speed); DEBUG_SERIAL.println("RPM");
    DEBUG_SERIAL.print("Load (Duty Cycle): ");
    DEBUG_SERIAL.print(Load); DEBUG_SERIAL.println("%");
    DEBUG_SERIAL.print("Voltage: ");
    DEBUG_SERIAL.print(Voltage); DEBUG_SERIAL.println("V");
    DEBUG_SERIAL.print("Temperature: ");
    DEBUG_SERIAL.print(Temper); DEBUG_SERIAL.println("C°");
    DEBUG_SERIAL.print("Current: ");
    DEBUG_SERIAL.print(Current); DEBUG_SERIAL.println("mA");
  } else {
    DEBUG_SERIAL.println("FeedBack error");
  }
  DEBUG_SERIAL.println("==================");
}

void st_writeWord_eeprom(u8 ID, u8 MemAddr, u16 wDat) {
  st.unLockEprom(ID);//unlock EPROM-SAFE
  st.writeWord(ID, MemAddr, wDat);//ID
  st.LockEprom(ID);//EPROM-SAFE locked
}

void st_writeByte_eeprom(u8 ID, u8 MemAddr, u8 bDat) {
  st.unLockEprom(ID);//unlock EPROM-SAFE
  st.writeByte(ID, MemAddr, bDat);//ID
  st.LockEprom(ID);//EPROM-SAFE locked
}

void st_resetAngle() {
  st.CalibrationOfs(1);
}

// ---- Internal description of registers ----
struct RegDesc { uint8_t addr; uint8_t len; const char* name; const char* mem; };

// Register map extracted from ST3215 memory table V3.7 (compatible with ST3250 in practice)
static const RegDesc regs[] = {
  { 0, 1, "Firmware major version number", "read only" },
  { 1, 1, "Firmware minor version number", "read only" },
  { 3, 1, "Servo major version number",    "read only" },
  { 4, 1, "Servo minor version number",    "read only" },
  { 5, 1, "ID",                             "read/write" },
  { 6, 1, "Baudrate",                       "read/write" },
  { 7, 1, "Return delay",                   "read/write" },
  { 8, 1, "Response status level",          "read/write" },
  { 9, 2, "Minimum angle",                  "read/write" },
  { 11, 2, "Maximum angle",                 "read/write" },
  { 13, 1, "Maximum temperature",           "read/write" },
  { 14, 1, "Maximum input voltage",         "read/write" },
  { 15, 1, "Minimum input voltage",         "read/write" },
  { 16, 2, "Maximum torque",                "read/write" },
  { 18, 1, "Phase",                          "read/write" },
  { 19, 1, "Unloading conditions",           "read/write" },
  { 20, 1, "LED alarm conditions",           "read/write" },
  { 21, 1, "Position loop P coefficient",    "read/write" },
  { 22, 1, "Position loop D coefficient",    "read/write" },
  { 23, 1, "Position loop I coefficient",    "read/write" },
  { 24, 2, "Minimum starting force",         "read/write" },
  { 26, 1, "CW insensitive zone",            "read/write" },
  { 27, 1, "CCW insensitive zone",           "read/write" },
  { 28, 2, "Protection current",             "read/write" },
  { 30, 1, "Angle resolution",               "read/write" },
  { 31, 2, "Position correction",            "read/write" },
  { 33, 1, "Operation mode",                 "read/write" },
  { 34, 1, "Protection torque",              "read/write" },
  { 35, 1, "Protection time",                "read/write" },
  { 36, 1, "Overload torque",                "read/write" },
  { 37, 1, "Speed loop P coefficient",       "read/write" },
  { 38, 1, "Overcurrent protection time",    "read/write" },
  { 39, 1, "Speed loop I coefficient",       "read/write" },
  { 40, 1, "Torque switch",                  "read/write" },
  { 41, 1, "Acceleration",                   "read/write" },
  { 42, 2, "Target location",                "read/write" },
  { 44, 2, "Operation time",                 "read/write" },
  { 46, 2, "Operation speed",                "read/write" },
  { 48, 2, "Torque limit",                   "read/write" },
  { 55, 1, "Lock flag",                      "read/write" },
  { 56, 2, "Current location",               "read only" },
  { 58, 2, "Current speed",                  "read only" },
  { 60, 2, "Current load",                   "read only" },
  { 62, 1, "Current voltage",                "read only" },
  { 63, 1, "Current temperature",            "read only" },
  { 64, 1, "Async write flag",               "read only" },
  { 65, 1, "Servo status",                   "read only" },
  { 66, 1, "Move flag",                      "read only" },
  { 69, 2, "Current current",                "read only" },
};

static inline long readRegValue(uint8_t id, const RegDesc& r) {
  // assuming st.readByte / st.readWord return int; adapt if your API differs
  if (r.len == 1) {
    return (long)st.readByte(id, r.addr);
  } else {
    return (long)st.readWord(id, r.addr);
  }
}

void st_read_eeprom() {
  // Reads ALL registers (EEPROM + SRAM) and prints them
  for (size_t i = 0; i < (sizeof(regs) / sizeof(regs[0])); ++i) {
    const RegDesc& r = regs[i];
    long v = readRegValue(ST_ID, r);
    // Print: addr(dec) addr(hex) name [mem] = value
    DEBUG_SERIAL.printf("%3u (0x%02X)  %-40s  [%s] = %ld\r\n",
                        r.addr, r.addr, r.name, r.mem, v);
    delay(2); // small delay to avoid flooding the bus
  }
}

void test_all_phase_values() {
  for (uint8_t phaseVal = 120; phaseVal <= 135; phaseVal++) {
    st_writeByte_eeprom(1, 18, phaseVal);
    delay(10);
    DEBUG_SERIAL.print("Test Phase value: ");
    DEBUG_SERIAL.println(phaseVal);
    st_go_to(140, 20);
    delay(2000);
    st_go_to(200, 20);
    delay(2000);
  }

  DEBUG_SERIAL.println("Selected Phase value scan complete.");
}  // The right phase value is 128 for the ST3250 servo.

void st_reset() {
  st.reset(ST_ID);
}