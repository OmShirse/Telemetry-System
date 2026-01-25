import sys
import csv

def calculate_checksum(data):
    chk = 0
    for ch in data:
        chk ^= ord(ch)
    return chk

ser = sys.stdin   # NOT callable

csv_file = open("telemetry_log.csv", "w", newline="")
csv_writer = csv.writer(csv_file)

csv_writer.writerow([
    "level", "timestamp",
    "ax", "ay", "az",
    "gx", "gy", "gz",
    "altitude", "temperature"
])

while True:
    line = ser.readline()   # CORRECT
    if not line:
        break

    line = line.strip()

    if not line.startswith('$') or '*' not in line:
        continue

    payload, chk_str = line[1:].split('*')

    try:
        rx_chk = int(chk_str, 16)
    except ValueError:
        continue

    if calculate_checksum(payload) != rx_chk:
        print("Checksum error")
        continue
    fields = payload.split(',')

    print(fields)
    csv_writer.writerow(fields)
    csv_file.flush()

    fields = payload.split(',')
    print(fields)
