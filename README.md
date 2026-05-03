# Microprocessor_Project
# Cross-Coupled 4-Zone HVAC Thermal Control System

## Team Members
* **Hamit İbrahimoğlu** - 230202915
* **Muhammed Hüseyin** - 220202938

## Project Overview
This project is an automated cooling system for 4 interconnected zones. Instead of operating fans independently the system uses a single unified C-code to read temperatures from all zones and adjust fan speeds based on the overall thermal condition. If one room gets too hot, the adjacent rooms react preventively.

## Hardware Components
* 1x Arduino Uno (Microprocessor Trainer Board)
* 4x LM35 Temperature Sensors
* 4x 5V DC Motor Fans
* 4x TIP120 Darlington Transistors
* 4x 1N4007 Diodes 
* External Power Supply
* Push Button (for Emergency Stop)

## System Modules (4 Distinct Modules)
1. **Multi-Channel ADC Module:** Continuously samples analog signals from the 4 LM35 sensors.
2. **Interconnected PWM Logic:** A unified algorithm that calculates the required fan speed (PWM) for each zone based on its own temperature and the thermal state of neighboring zones.
3. **Hardware Interrupt Module:** An external interrupt (ISR) connected to a master push button to instantly halt all motors in case of an emergency.
4. **Multiplexed Display Interface:** Outputs real-time temperature readings and fan status.

## Simulation & Prototyping
Before the physical hardware assembly, we prototyped the system. We documented the progress in three main stages:

1. **Sensor Calibration:** Testing the ADC readings and mapping of the LM35 sensors.
 

2. **Single Zone Test:** Wiring and testing the TIP120 transistor logic with one DC motor.


3. **Full System Integration:** The complete 4-zone setup including the display and the emergency interrupt button.

  
## Challenges & Solutions
* **Simulator Limitations:** We initially attempted to build the circuit in Proteus but faced issues with library files. Wokwi also lacked some specific components we needed. We ultimately solved this by transitioning to Tinkercad, which allowed us to fully simulate the sensor-motor interactions and capture our prototype stages.
* **Code Complexity:** We originally worked with two separate, complex codes for the sensors and the PWM logic. They were causing timing conflicts, so we spent significant time simplifying and merging them into one clean, unified C-code to handle the entire system efficiently.
* **High Current Load:** The microcontroller cannot directly power 4 motors. We solved this by using an external power source and routing the power through TIP120 transistors, ensuring a Common Ground connection for stable signal reading.
* **Inductive Kickback:** To protect the logic board from the reverse voltage generated when the motors stop, we installed 1N4007 diodes across each motor terminal.

## Workload Justification (3-Person Equivalency)
This project requires the workload of 3 students due to the complexities of managing 4 independent thermal zones with a single controller. We divided the primary responsibilities, and distributed the "Third Person" integration tasks based on system requirements.

### 1. Primary Responsibilities
* **Hamit İbrahimoğlu:** Responsible for configuring the 4-channel ADC, signal scaling, and programming the Hardware Interrupt (ISR) logic for the master emergency stop.
* **Muhammed Hüseyin:** Responsible for writing the cross-coupled PWM logic, data interpolation algorithms, and programming the 7-segment display multiplexing.

### 2. The "Third Person" Workload (Systems Integration)
To successfully merge our individual modules into a working physical system, additional integration engineering was required:

* **Core Code Merging:** Resolving timing conflicts between the ADC, PWM, and ISR loops, and simplifying the logic into a single, unified C-code for real-time execution. *(Hamit İbrahimoğlu)*
* **Virtual Prototyping & Simulation:** Designing the complete 4-zone system architecture on Tinkercad to validate the logic and test the merged code before physical assembly. *(Hamit İbrahimoğlu)*
* **High-Current Power Architecture:** Designing and wiring the external power distribution using TIP120 transistors and establishing a Common Ground to safely drive the 4 DC fans. *(Muhammed Hüseyin)*
* **Hardware Safety Measures:** Installing the 1N4007 flyback diodes across the motor terminals to protect the logic board from inductive kickback. *(Muhammed Hüseyin)*
* **Physical Assembly:** Final breadboard routing, sensor placement, and component organization in the physical maquette. *(Completed by: Joint Effort)*
