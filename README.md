EasyRider
=========

EasyRider REV C is a motorcycle telemetrics project, developed specifically for my Honda C90 classic bike.

The software is written in C using the libopencm3 library for the STM32 446RE Arm Cortex M4 microcontroller.

The PCB's were created with Kicad 6, you'll find all gerbers/schematics/datasheets in the /docs directory.

Functionality
-------------

EasyRider basically does the following things:

- Controls the lighting system: indicators, rear light, brake light, etc.
- Controls the claxon
- Contains an alarm system, based on an IMU
- Reads the motorcycle battery voltage and the power draw of the board
- Contains a buzzer for some startup/alarm sounds
- Reads the RPM of the motor via the CDI, through an opamp and optocoupled circuit
- Logs GPS coordinates and timestamps of events via the external RTC
- Sends all telemetry data over Bluetooth and writes it to onboard Flash.


Overall changes/benefits compared to Rev B
------------------------------------------

- Way more powerful -> Main board: STM32 Nucleo ARM Cortex 4 32bit mcu with Floating Point Unit
- Way easier debugging with SWD (Serial Wire Debug)
- Smaller form-factor size and more modular -> 2 detachable shields -> riser/daughter boards:
  3x stack of 70mmx80mm boards, connected with double 2x19pin headers (76 pins total)
- EasyRelay board: PSU/Relay grid with connectors and 2x 16 port I/O expander IC's
- EasyComm boards: GPS/Bluetooth/IMU/Flash Storage/Sound(buzzer)
   - Adafruit Ultimate GPS Breakout - 66 channel w/10 Hz updates - Version 3
   - Adafruit LSM6DS33 + LIS3MDL - 9 DoF IMU with Accel / Gyro / Mag - STEMMA QT Qwiic
   - Adafruit Bluefruit LE SPI Friend - Bluetooth Low Energy (BLE)
- 4-layer board instead of 2-layer -> less noise, crosstalk
- Lower power -> most IC's at 3.3V, relay coils at 5V
- Lower power -> switching 5V Voltage Regulator instead of Linear
- Radio connectivity: Bluetooth LE low power instead of power-hungry Wifi
- Fast storage for logging/settings/unique ID's/reboots etc. -> 256MBit WinBond SPI flash memory.
- New smaller molex connectors (Mini-Lock)
- More decoupling cap pads at longer traces
- Seperate 12V higher current wire for claxon relay
- Shift registers/IO expander IC before relays/darlington transitor arrays -> less pins needed on Cortex
- Shift registers/IO expander IC before input sense signals -> less pins needed on Cortex

![Board](https://github.com/stamina/easyriderc/raw/main/images/easycomm_topshield.jpg "EasyComm Top Shield")
![Board](https://github.com/stamina/easyriderc/raw/main/images/easyrelay_bottomshield.jpg "EasyRelay Bottom Shield")
![Board](https://github.com/stamina/easyriderc/raw/main/images/easyriderc_fullstack.jpg "EasyRider Rev C Full Stack")

