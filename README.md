# Project Files

## Part 1: UART Telemetry System with Sensor Simulation
1. **sensor_sim.c**  
   - Simulates onboard sensors: accelerometer, gyroscope, altitude, temperature.

2. **telemetry_tx.c**  
   - Packages sensor data into structured telemetry frames.  
   - Implements checksum validation and fixed-rate transmission (~20 Hz).

3. **uart_rx.py**  
   - Reads telemetry frames from stdin.  
   - Performs frame parsing, checksum verification, console visualization, and CSV logging.

4. **telemetry_log.csv**  
   - Stores all valid telemetry frames for offline analysis.

---

## Part 2: Level-2 Telemetry System with AHRS and Visualization
1. **ahrs_filter.c**  
   - Implements the **Madgwick AHRS algorithm**.  
   - Computes orientation (roll, pitch, yaw) from simulated IMU data.

2. **etelemtry.c**  
   - Simulates IMU sensor data (accelerometer + gyroscope).  
   - Streams enhanced telemetry including AHRS outputs via stdout.

3. **visualizer.py**  
   - Python ground-station visualization script.  
   - Supports time-series plotting of roll, pitch, yaw.  
   - Provides 3D cube visualization for real-time orientation.

4. **run.bat** (optional)  
   - Windows batch file to automate compilation and execution of the C telemetry program and Python visualizer.  
   - Ensures dependencies and execution order are correct.

---
