@echo off
echo Starting visualization...
echo.

REM Check if etelemetry.exe exists
if not exist etelemetry.exe (
    echo Error: etelemetry.exe not found
    echo Compile with: gcc -o etelemetry.exe etelemetry.c -lm
    pause
    exit
)

REM Install PyQt5 if needed (better than TkAgg)
python -m pip install --quiet --upgrade PyQt5 matplotlib numpy 2>nul

REM Run
etelemetry.exe | python visualizer.py

pause