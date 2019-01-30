from threading import Thread
import serial

ser_write = serial.Serial("/dev/ttyACM0",115200)
ser_read = serial.Serial("/dev/ttyACM0",115200)

def write():
    while 1:
        def send(cmd_write):
            cmd_write = str(cmd_write)
            ser_write.write(cmd_write)
            print(cmd_write)

        cmd = input("Veuillez entrer votre commande (help pour liste de commande): ")
        
        cmd = int(cmd)
        if cmd == 1:
            send(cmd)
            break
        else:
            send(cmd)
            
            
            
def read():
    while 1:
        cmd_read = ser_read.readline()
        print(cmd_read)
        if cmd_read == "break":
            break
        
t1_write = Thread(target = write)
t2_read = Thread(target = read)

t1_write.start()
t2_read.start()

t1_write.join()
t2_read.join()
