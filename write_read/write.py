import serial

ser = serial.Serial('/dev/ttyACM0',115200)
command_list = []

while 1:

    def send(cmd_write):
        cmd_write = str(cmd_write)
        ser.write(cmd_write)
        print(cmd_write)

    cmd = input("Veuillez entrer votre commande (help pour liste de commande): ")

    if cmd == "help":
        print(command_list)
    else:
        send(cmd)