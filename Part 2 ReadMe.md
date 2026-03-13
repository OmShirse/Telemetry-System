# UART Telemetry System – Level 2

## Overview

This project implements a **Level-2 telemetry system** that:

* Simulates IMU sensor data
* Estimates orientation using an **AHRS algorithm**
* Streams **enhanced telemetry**
* Visualizes attitude on a host system using Python

**Goals:**

* Demonstrate understanding of sensor fusion concepts
* Show telemetry data flow
* Provide real-time ground-station visualization

The focus is on **clarity, correctness, and explainability**, rather than hardware-specific optimizations.

---

## Components

### 1. Sensor Data Simulation (C)

* Accelerometer and gyroscope data are simulated using mathematical functions
* No physical hardware is required, allowing **deterministic testing** and easier debugging

**Why simulation is used:**

* Hardware availability is not assumed
* Focus is on **algorithm and telemetry design**

---

### 2. AHRS Algorithm (C)

* Orientation is estimated using a **Madgwick IMU filter**
* **Inputs:** accelerometer + gyroscope
* **Outputs:** roll, pitch, yaw (in degrees)

**Notes:**

* No magnetometer is used
* Yaw may drift over time (expected and documented)

---

### 3. Enhanced Telemetry (C)

* Sensor values + AHRS outputs are streamed as telemetry
* **Tab-separated format** for easy parsing and debugging
* Data is sent via **stdout**, simulating a UART data stream

**Why stdout instead of hardware UART:**

* Allows **quick integration** with Python visualization tools
* Preserves the **logical telemetry flow** used in embedded systems

---

### 4. Python Ground Station

Two visualization modes:

**a) Time-Series Plots**

* Roll, pitch, and yaw plotted against time
* Used to verify **correctness and stability** of the filter

**b) 3D Orientation Visualization**

* A 3D rigid body (cube) rotates according to roll, pitch, and yaw
* Demonstrates **spatial attitude in real time**

> The Python program **only visualizes data**; it does not perform filtering or estimation.

---

## Execution Workflow

### Compilation (C)

```bash
gcc etelemetry.c -o etelemetry
```

### Run (Pipeline)

```bash
./etelemetry | python visualizer.py
```

### Batch File Usage (Optional)

A Windows batch file is included for convenience:

* Checks required dependencies
* Ensures correct execution order
* Avoids manually running multiple commands each time

> The batch file is optional and does not affect system operation.

---

## Screen Recording

A 30-second screen recording demonstrates:

* Telemetry generation
* Live visualization
* Correct end-to-end data flow

---

## Assumptions & Limitations

* Sensor data is simulated
* No magnetometer; yaw may drift
* Telemetry is text-based for simplicity
* Visualization runs on the host only

> These choices keep the implementation **focused, explainable, and deterministic**.

---

## Tools & Development Environment

* **Languages:** C, Python
* **Visualization:** matplotlib
* **IDE:** VS Code Insiders

**AI-assisted tools** were used for:

* Debugging support
* Visualization exploration
* Productivity improvements

**Madgwick AHRS** implementation references publicly available documentation and open-source code. Integration into the telemetry pipeline was performed manually to ensure understanding and correctness.

---

## Issues Encountered

| Issue                              | Cause                         | Solution                                |
| ---------------------------------- | ----------------------------- | --------------------------------------- |
| `M_PI` undefined                   | Missing macro                 | Defined manually                        |
| `EnhancedTelemetryFrame` undefined | Struct used before definition | Moved struct definition above functions |
| `undefined reference to WinMain`   | MinGW assumed GUI program     | Compile with `-mconsole`                |
| Python stdin blocking              | Direct piping hangs           | CSV output or correct piping used       |
| `matplotlib.pyplot` install error  | Submodule instead of package  | Install `matplotlib` package            |
| NumPy MINGW warning                | Experimental build            | Use stable NumPy version                |
| Heavy 3D redraw                    | `Poly3DCollection` slow       | Switched to lightweight line-based cube |

> The **batch file** helps overcome these issues during execution.

---

## Summary

This project demonstrates a complete **Level-2 telemetry pipeline**:

* Simulated sensors
* AHRS orientation estimation
* Enhanced telemetry streaming
* Real-time ground-station visualization

> Emphasis is placed on **system integration, clarity, and real-world embedded development practices**.

---

