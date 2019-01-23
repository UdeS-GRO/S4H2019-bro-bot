import serial

ser = serial.Serial('/dev/ttyACM0', 115200)

while 1:
    cmd_read = ser.readline()
    print(cmd_read)

