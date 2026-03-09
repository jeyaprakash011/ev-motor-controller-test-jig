# ev-motor-controller-test-jig
STM32 based CAN interface module used in an EV motor controller automated testing jig for generating throttle signals and monitoring RPM, voltage, and current.
# EV Motor Controller Automated Test Jig – STM32 CAN Interface

## Overview

This project demonstrates firmware developed for an **EV motor controller automated testing jig** used in industrial validation and quality assurance.

The firmware runs on an **STM32 Blue Pill** microcontroller and performs the following functions:

- Generates **analog throttle signals** using a DAC module
- Reads **motor controller data via CAN Bus**
- Converts raw CAN data into engineering values (RPM, Voltage, Current)
- Sends processed data to an **ESP32 master controller via serial communication**

This module acts as a **signal interface and data acquisition unit** in the automated EV controller test system.

> Note: Certain identifiers such as CAN IDs have been anonymized for privacy.

---

## System Architecture

The testing system consists of two controllers:

### STM32 Module
Handles:

- CAN bus communication with motor controller
- Analog throttle signal generation
- Data conversion
- Serial data transmission

### ESP32 Controller

Handles:

- Test sequence automation
- Mode selection (Eco / Boost / Reverse / Brake)
- Data logging
- User interface

---

## Features

- CAN bus data acquisition
- DAC based throttle simulation
- Serial command interface
- Data conversion for monitoring parameters
- Real-time telemetry transmission
- Industrial test jig integration

---

## Hardware Components

- STM32 Blue Pill (STM32F103C8T6)
- MCP4725 DAC module
- CAN transceiver module
- EV motor controller
- ESP32 main controller
- Proximity sensor (for RPM validation)
- Relay interface
- Signal conditioning circuits

---

## Functional Workflow

1. The ESP32 sends commands to the STM32.
2. STM32 generates a **throttle signal using DAC**.
3. The EV motor controller responds with **CAN bus data**.
4. STM32 reads the CAN frames.
5. Raw values are converted into:

   - Motor RPM
   - Battery Voltage
   - Battery Current

6. Every second the data is sent via **serial communication** to ESP32.


---

## Key Parameters Monitored

| Parameter | Description |
|--------|-------------|
| Motor RPM | Motor rotational speed |
| Battery Voltage | EV battery voltage |
| Battery Current | Current drawn by controller |
| Throttle % | Throttle position |
| Mode | Eco / Boost / Reverse |
| Error Flags | Motor and MCU fault indicators |

---

## DAC Throttle Control

The MCP4725 DAC generates an **analog throttle signal** between:
0V → Idle
4.2V → Full throttle


The voltage increases gradually using a **controlled ramp algorithm** to simulate real throttle behavior.

---

## Serial Commands

Example command to generate throttle signal:
DAC 2.5


This will ramp the DAC output voltage up to **2.5V**.

---

## Example Serial Output  : *1500,48,20,12,1,0,0,0,0,0#

Where:

| Field | Meaning |
|------|--------|
| 1500 | Motor RPM |
| 48 | Battery Voltage |
| 20 | Throttle % |
| 12 | Battery Current |

---

## Applications

This system can be used for:

- EV motor controller validation
- Production testing
- Hardware-in-loop testing
- Motor controller QA automation

---

## Future Improvements

- CAN command injection
- Data logging to SD card
- Ethernet/WiFi monitoring
- Graphical monitoring dashboard

---

## Author

Embedded firmware developed for **industrial EV motor controller testing systems** using STM32 and CAN bus communication.

