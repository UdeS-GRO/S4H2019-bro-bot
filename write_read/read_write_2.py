from threading import Thread
import serial
import time

ser_write = serial.Serial("/dev/ttyACM0",115200)
ser_read = serial.Serial("/dev/ttyACM0",115200)

def write():
    while 1:        
        cmd = input('Veuillez entrer votre commande (help pour liste de commande): ')
        send(cmd)
            
def read():
    while 1:
        cmd_read = ser_read.readline()
        print(cmd_read.decode('utf-8'))
        
def send(cmd_write):
        ser_write.write(cmd_write.encode())
        print(cmd_write)

  
t1_write = Thread(target = write)
t2_read = Thread(target = read)

t1_write.start()
t2_read.start()

t1_write.join()
t2_read.join()

