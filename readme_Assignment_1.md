# UART Telemetry System with Sensor Simulation

## Overview

This project implements a real-time UART telemetry pipeline that simulates sensor data, packages it into structured telemetry frames, and receives, validates, and logs the data on a ground-station side.

The goal of this assignment is not only to transmit data, but to design a robust, debuggable telemetry system** similar to those used in embedded aerospace and UAV applications.

---

## Sensor Behaviour

* **Accelerometer & Gyroscope**: Sinusoidal patterns to simulate motion.
* **Altitude**: Increases linearly over time.
* **Temperature**: Minor fluctuations around a nominal value.

---

## Communication Protocol

* UART is assumed **error-free**, except for **simulated checksum verification**.
* Frame size and structure are **fixed** and do not adapt dynamically.
* Frames include a **start marker**, comma-separated payload, and XOR checksum for integrity.

---

## Files

### 1. Sensor Data Generation (`sensor_sim.c`)

Simulates multiple onboard sensors:

* Accelerometer: `ax`, `ay`, `az`
* Gyroscope: `gx`, `gy`, `gz`
* Altitude
* Temperature

**Implementation:** Uses mathematical functions to generate smooth, time-varying sensor data.

---

### 2. Telemetry Framing

* Each telemetry packet follows a structured format:

  ```
  $<FrameType>,<Timestamp>,<Sensor Data>*<Checksum>
  ```
* **Features:**

  * Start marker for frame synchronization
  * Comma-separated payload
  * XOR checksum for data integrity
  * Fixed transmission rate (~20 Hz)

**Inspiration:** Protocols like NMEA and UAV telemetry links.

---

### 3. UART Communication

* Emulated using **standard input/output streams** due to lack of physical UART hardware.
* **Flow:**

  * C telemetry program writes frames to `stdout`.
  * Python receiver reads frames from `stdin`.
  * Shell piping connects both programs.

**Execution Command:**

```bash
./telemetry_tx | python uart_rx.py
```

---

### 4. Ground Station Receiver (`uart_rx.py`)

* Continuous stream reading
* Frame boundary detection
* Checksum verification
* Payload parsing
* Console visualization
* CSV logging (`telemetry_log.csv`)

**Purpose:** Enables offline data analysis compatible with Excel, MATLAB, Python, and other visualization tools.

---

## Issues Encountered & Solutions

| Issue                         | Cause                              | Solution                                        |
| ----------------------------- | ---------------------------------- | ----------------------------------------------- |
| Serial module errors          | Missing/conflicting Python modules | Corrected environment and module usage          |
| COM port not found            | No physical UART device            | Emulated UART using stdin/stdout piping         |
| No output in receiver         | Output buffering on Windows        | Disabled buffering using `setbuf(stdout, NULL)` |
| Receiver waiting indefinitely | Python run without piped input     | Correct execution using shell piping            |

---

## AI Usage

AI-assisted features in **VS Code Insiders** helped with:

* Code formatting suggestions
* Syntax checks
* Debugging assistance

---

## Summary

This project demonstrates:

* Embedded-style telemetry design
* Real-time data streaming
* UART protocol handling
* Checksum-based validation
* Ground-station data processing
* Debugging of stream-based systems

The implementation prioritizes **clarity, reliability, and explainability**, aligned with real-world embedded development practices.

---

## How to Run

```bash
gcc telemetry_tx.c -o telemetry_tx
./telemetry_tx | python uart_rx.py
```
