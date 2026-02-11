import serial
import socket

# Arduino
ser = serial.Serial('COM4', 9600, timeout=1)

# UDP vers Unreal
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
unreal_addr = ("127.0.0.1", 5005)  # Port UDP qui correspond à Unreal

while True:
    line = ser.readline().decode().strip()
    if line in ['0', '1']:
        sock.sendto(bytes([int(line)]), unreal_addr)

        print(f"Arduino sent: {line}")

