import serial
import socket

# Arduino
ser = serial.Serial('COM4', 9600, timeout=1)

# UDP → Unreal
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
unreal_addr = ("127.0.0.1", 5005)

while True:
    line = ser.readline().decode().strip()

    if not line:
        continue

    # ----- Button + Sensor states -----
    if "," in line:
        parts = line.split(",")
        if len(parts) == 3:
            try:
                b2 = int(parts[0])
                b3 = int(parts[1])
                sensor = int(parts[2])

                sock.sendto(bytes([b2, b3, sensor]), unreal_addr)
                print(f"States sent: {b2}, {b3}, {sensor}")

            except ValueError:
                pass

    # ----- Speed message -----
    elif line.startswith("SPEED:"):
        try:
            interval = int(line.split(":")[1])

            # Send raw microseconds (4 bytes) to Unreal
            sock.sendto(interval.to_bytes(4, byteorder='little'), unreal_addr)

            # Convert to seconds (NOT sent)
            seconds = interval / 1_000_000.0

            # Grey print (ANSI 90)
            print(f"\033[90mInterval: {interval} µs = {seconds:.6f} s\033[0m")

        except ValueError:
            pass
