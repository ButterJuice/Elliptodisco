import serial
import socket

# --- Serial connection ---
ser = serial.Serial('COM4', 9600, timeout=1)  # change COM4 if needed

# --- UDP to Unreal ---
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
unreal_addr = ("127.0.0.1", 5005)

# Flags
FLAG_BUTTONS = 0x04
FLAG_SPEED   = 0x02
#For future use   = 0x03
#For future use   = 0x04
#For future use   = 0x05
#For future use   = 0x06
#For future use   = 0x07
#For future use   = 0x08

while True:
    line = ser.readline().decode().strip()
    if not line:
        continue

    # ---- Buttons + Sensor ----
    if "," in line:
        parts = line.split(",")
        if len(parts) == 3:
            try:
                b2 = int(parts[0])
                b3 = int(parts[1])
                sensor = int(parts[2])

                # Create packet with 1-byte flag + 3 bytes of data
                packet = bytes([FLAG_BUTTONS, b2, b3, sensor])
                sock.sendto(packet, unreal_addr)

                print(f"States sent: {b2}, {b3}, {sensor} (flag={FLAG_BUTTONS})")

            except ValueError:
                pass

    # ---- Speed ----
    elif line.startswith("SPEED:"):
        try:
            interval = int(line.split(":")[1])  # microseconds

            # Convert interval to 4 bytes
            interval_bytes = interval.to_bytes(4, byteorder='little')

            # Prepend 1-byte flag
            packet = bytes([FLAG_SPEED]) + interval_bytes
            sock.sendto(packet, unreal_addr)

            # Optional print in grey
            seconds = interval / 1_000_000.0
            print(f"\033[90mInterval: {interval} µs = {seconds:.6f} s (flag={FLAG_SPEED})\033[0m")

        except ValueError:
            pass
