# 🔧 Laser Turret – Embedded Control & PCB Firmware

> **Message Contract First.** All modules talk using **newline-delimited JSON (NDJSON)** over the selected transport (UART / RS485 / CAN with JSON payload). Each JSON object ends with `\n`. Unknown fields are ignored. Missing fields fall back to sane defaults (shown below) inside the firmware via `value | default`.

## 📨 JSON Message Contract (Over Serial/CAN)

### Transport & Framing

* **Physical/Link**: UART (TTL) • RS485 (half-duplex) • CAN (JSON in data field with simple chunking if needed)
* **Encoding**: UTF-8 JSON, **one message per line**, terminated by `\n`
* **Max payload (typ.)**: ≤ 384 bytes for `command`, ≤ 512 bytes for `status` (per current `StaticJsonDocument` sizes)
* **Clocking**: default UART `SERIAL_8N1` (baud per build config)
* **Reliability**: Higher layers may re-send last packet periodically; receivers treat each message as idempotent

---

### ▶️ Command (Control Station ESP32 ➜ Turret ESP32)

**Root shape**

```json
{
  "type": "command",
  "system_enabled": <bool>,
  "motion": {
    "pan_speed_rpm": <float>,
    "pan_min_angle": <float>,
    "pan_max_angle": <float>,
    "tilt_speed_rpm": <float>,
    "tilt_min_angle": <float>,
    "tilt_max_angle": <float>,
    "laser_focus_angle": <float>
  },
  "actuator": {
    "laser1_on": <bool>,
    "laser2_on": <bool>,
    "laser3_on": <bool>,
    "pan_motor_on": <bool>,
    "tilt_motor_on": <bool>,
    "buzzer_on": <bool>
  }
}
```

**Fields**

| Path                       | Type   | Unit    | Default | Range / Notes                           |
| -------------------------- | ------ | ------- | ------- | --------------------------------------- |
| `type`                     | string | –       | –       | Must be `"command"`.                    |
| `system_enabled`           | bool   | –       | `false` | Global soft-enable for turret logic.    |
| `motion.pan_speed_rpm`     | float  | rpm     | `0.0`   | Desired pan speed (sign = direction).   |
| `motion.pan_min_angle`     | float  | degrees | `0.0`   | Software min limit for pan.             |
| `motion.pan_max_angle`     | float  | degrees | `0.0`   | Software max limit for pan.             |
| `motion.tilt_speed_rpm`    | float  | rpm     | `0.0`   | Desired tilt speed (sign = direction).  |
| `motion.tilt_min_angle`    | float  | degrees | `0.0`   | Software min limit for tilt.            |
| `motion.tilt_max_angle`    | float  | degrees | `0.0`   | Software max limit for tilt.            |
| `motion.laser_focus_angle` | float  | degrees | `0.0`   | Target focus angle (if used by optics). |
| `actuator.laser1_on`       | bool   | –       | `false` | Relay R1.                               |
| `actuator.laser2_on`       | bool   | –       | `false` | Relay R2.                               |
| `actuator.laser3_on`       | bool   | –       | `false` | Relay R3 (aiming beam).                 |
| `actuator.pan_motor_on`    | bool   | –       | `false` | Relay R4 (Servo/driver power).          |
| `actuator.tilt_motor_on`   | bool   | –       | `false` | Relay R5 (Servo/driver power).          |
| `actuator.buzzer_on`       | bool   | –       | `false` | Relay R6.                               |

**Minimal example**

```json
{"type":"command","system_enabled":true,"motion":{"pan_speed_rpm":12.0,"pan_min_angle":-90,"pan_max_angle":90,"tilt_speed_rpm":8.0,"tilt_min_angle":-30,"tilt_max_angle":45,"laser_focus_angle":5.0},"actuator":{"laser1_on":true,"laser2_on":false,"laser3_on":true,"pan_motor_on":true,"tilt_motor_on":true,"buzzer_on":false}}
```

---

### 📤 Status (Turret ESP32 ➜ Control Station / GUI)

**Root shape**

```json
{
  "type": "status",
  "motion": {
    "pan_angle_deg": <float>,
    "tilt_angle_deg": <float>,
    "pan_speed_rpm": <float>,
    "tilt_speed_rpm": <float>,
    "pan_voltage": <float>,
    "tilt_voltage": <float>
  },
  "buttons": {
    "laser_blue": <bool>,
    "laser_green": <bool>,
    "stage1": <bool>,
    "stage2": <bool>,
    "stage3": <bool>,
    "manual_mode": <bool>,
    "confirm_target": <bool>
  },
  "joystick": {
    "pot_pan": <float>,
    "pot_tilt": <float>,
    "pot_focus": <float>,
    "press": <bool>
  }
}
```

> **Note:** The code also reads `buttons.arm` when parsing status (sets `lastStatus.buttons.arm`). If you plan to surface an **arm** indicator, include it under `buttons.arm` in the sender.

**Fields**

| Path                     | Type   | Unit    | Default | Range / Notes                                               |
| ------------------------ | ------ | ------- | ------- | ----------------------------------------------------------- |
| `type`                   | string | –       | –       | Must be `"status"`.                                         |
| `motion.pan_angle_deg`   | float  | degrees | `0.0`   | Measured pan angle.                                         |
| `motion.tilt_angle_deg`  | float  | degrees | `0.0`   | Measured tilt angle.                                        |
| `motion.pan_speed_rpm`   | float  | rpm     | `0.0`   | Measured/estimated pan speed.                               |
| `motion.tilt_speed_rpm`  | float  | rpm     | `0.0`   | Measured/estimated tilt speed.                              |
| `motion.pan_voltage`     | float  | volts   | `0.0`   | Driver/sensor reading.                                      |
| `motion.tilt_voltage`    | float  | volts   | `0.0`   | Driver/sensor reading.                                      |
| `buttons.laser_blue`     | bool   | –       | `false` | Panel button state.                                         |
| `buttons.laser_green`    | bool   | –       | `false` | Panel button state.                                         |
| `buttons.stage1`         | bool   | –       | `false` | Mode stage indicator.                                       |
| `buttons.stage2`         | bool   | –       | `false` | Mode stage indicator.                                       |
| `buttons.stage3`         | bool   | –       | `false` | Mode stage indicator.                                       |
| `buttons.manual_mode`    | bool   | –       | `false` | Manual override active.                                     |
| `buttons.confirm_target` | bool   | –       | `false` | Operator confirmation.                                      |
| `buttons.arm`*           | bool   | –       | `false` | (*Optional but parsed by code.) Global arm latch indicator. |
| `joystick.pot_pan`       | float  | 0..1    | `0.0`   | Normalized raw input.                                       |
| `joystick.pot_tilt`      | float  | 0..1    | `0.0`   | Normalized raw input.                                       |
| `joystick.pot_focus`     | float  | 0..1    | `0.0`   | Normalized raw input.                                       |
| `joystick.press`         | bool   | –       | `false` | Joystick switch.                                            |

**Example**

```json
{"type":"status","motion":{"pan_angle_deg":14.2,"tilt_angle_deg":-3.5,"pan_speed_rpm":10.7,"tilt_speed_rpm":7.9,"pan_voltage":11.8,"tilt_voltage":11.7},"buttons":{"arm":true,"laser_blue":false,"laser_green":true,"stage1":true,"stage2":false,"stage3":false,"manual_mode":false,"confirm_target":true},"joystick":{"pot_pan":0.41,"pot_tilt":0.58,"pot_focus":0.12,"press":false}}
```

---

### Parsing & Robustness Rules

* Messages without `type` are **ignored**.
* Unknown fields are **ignored** (for forward compatibility).
* On `type: "command"` and `type: "status"`, missing inner fields use defaults shown above.
* Receiver processes a full message **only at newline**; partial lines are buffered.
* **Validation**: basic deserialization error check; invalid JSON is dropped silently.
* **Versioning** (recommended): add `"schema":"1.0"` at root in future revisions; receivers should ignore if absent.

---

## 📦 Repository Structure

```bash
├── firmware/
│   ├── control_station_esp32/        # Control station (joystick, buttons, PC communication)
│   │   ├── include/                  # Header files
│   │   │   ├── JsonMessenger.h
│   │   │   └── StationConfig.h
│   │   ├── src/                      # Source files
│   │   │   ├── JsonMessenger.cpp
│   │   │   ├── StationConfig.cpp
│   │   │   └── main.cpp
│   │   ├── lib/                      # External libraries
│   │   └── README
│   │
│   ├── turret_esp32/                 # Main turret logic (motors, laser, feedback)
│   │   ├── include/                  # Header files
│   │   │   ├── JsonMessenger.h
│   │   │   ├── STM_Link.h
│   │   │   ├── SerialSpeedInput.h
│   │   │   ├── TurretConfig.h
│   │   │   ├── ddl_servo.h
│   │   │   ├── nema_driver.h
│   │   │   ├── physics.h
│   │   │   ├── st_driver.h
│   │   │   └── stepper_focus_driver.h
│   │   ├── src/                      # Source files
│   │   │   ├── JsonMessenger.cpp
│   │   │   ├── STM_Link.cpp
│   │   │   ├── TurretConfig.cpp
│   │   │   ├── ddl_servo.cpp
│   │   │   ├── nema_driver.cpp
│   │   │   ├── physics.cpp
│   │   │   ├── st_driver.cpp
│   │   │   ├── stepper_focus_driver.cpp
│   │   │   └── main.cpp
│   │   ├── lib/                      # External libraries (Servo, Dynamixel, etc.)
│   │   │   ├── Dynamixel2Arduino/
│   │   │   ├── SCServo/
│   │   │   └── ServoDriverST/
│   │   └── test/
│   │
│   └── turret_stm32/                 # Low-level I/O, relays, limit switches
│       ├── Core/                     # Application core (HAL, main loop)
│       ├── Drivers/                  # STM32 HAL & BSP drivers
│       ├── metadata/                 # Project metadata
│       ├── settings/                 # Build / configuration files
│       └── Debug/
│
├── pcb/
│   └── turret_control_station_pcb_design/   # Shared PCB for turret & control station
│
├── docs/
│   └── readme_assets/                # Diagrams, schematics, images
│
└── README.md

---

## 🧠 System Architecture

Two microcontroller nodes coordinate turret control and safety via serial messaging.

### 1) ESP32 – Control Station Module

**Reads**

* 3-axis joystick + buttons (GPIO)

**Receives**

* Target parameters from laptop (CV module)

**Sends**

* `command` JSON to Turret Unit (UART / Modbus / CAN)

**Inputs from Laptop**

* Pan/Tilt (angle/velocity), laser enable, mode toggles, buzzer

---

### 2) ESP32 – Turret Unit Module

**Receives**

* `command` JSON from Control Station

**Controls**

* **Relays**: laser modules (x3), servo power (x2), buzzer
* **Pan/Tilt servos** over RS485 (half-duplex, vendor protocol)
* **Stepper motor** (driver or direct)

**Communicates**

* UART with **STM32** for relay/limit I/O
* Sends `status` JSON back to Control Station

**Feedback**

* Motor angles/speeds/temperatures, relay states, safety flags

---

### 3) STM32F0 – Turret Relay & Safety Controller

**Functions**

* Drives **6 relays** via GPIO + transistor stages
* Reads **limit switches** (NO type)
* Handles **EMG** switches that cut **12 V** power electrically (out of MCU loop)

**Relay Map**

| Relay | Load                |
| ----: | ------------------- |
|    R1 | Laser Module 1      |
|    R2 | Laser Module 2      |
|    R3 | Laser Aiming Beam   |
|    R4 | Servo Motor 1 Power |
|    R5 | Servo Motor 2 Power |
|    R6 | Buzzer              |

---

## ⚡ Power & Safety

**Input**: 12 V DC
**Regulation**: 12→5 V (buck), 5→3.3 V (LDO)

**Global Power Relays**

| Relay Group | Purpose                         |
| ----------- | ------------------------------- |
| Main        | Master enable for all actuators |
| Motor       | Enables R4–R5 (servo/driver)    |
| Laser       | Enables R1–R3 (lasers)          |

**Emergency Interfaces**

* External EMG/limit contacts in series with relay coils
* **Closed = enabled**, **open = immediate power cut** to the group

---

## 🔁 Links & Protocols

**Control Station ⇄ Turret**: UART / Modbus (RS485) / CAN; payload is the JSON `command`/`status` above.
**Turret ⇄ STM32**: UART, compact custom messages for relay toggles + limit switch feedback (JSON or binary, depending on build).

---

## 🛠️ Feature Highlights

* ✅ One **unified PCB** used in **both modules**
* ✅ Clean module split: **control**, **execution**, **relay/safety**
* ✅ Multi-bus comms: **Serial**, **Modbus**, **CAN**
* ✅ Hardware **EMG** cutoffs for lasers & motors
* ✅ Manual / CV / Hybrid control modes
* ✅ GPIO headroom for future sensors/actuators

---

## 🧪 Getting Started (Soon)

Step-by-step flashing & wiring guides for:

* ESP32 (Control Station)
* ESP32 (Turret Unit)
* STM32 (Relay Controller)

---

## 🧩 Implementation Notes (from `JsonMessenger`)

* `begin()` sets port & reserves ~512 B buffer.
* `sendCommand()` / `sendStatus()` build JSON via ArduinoJson and append `\n`.
* `receive()` buffers until newline, then `processMessage()`.
* `processMessage()` validates `type` and populates `lastCommand` / `lastStatus`.
* Document sizes: `StaticJsonDocument<384>` (command/status send) and `StaticJsonDocument<512>` (receive/parse).

> **Consistent Button Set:** The parser reads `buttons.arm` in `status`; if you want this latched in UI/safety, emit it from the sender.
