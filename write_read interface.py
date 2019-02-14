"""
 * Copyright 2019 Etienne Villemure
 This Source Code Form is subject to the terms of the Mozilla Public
 License, v. 2.0. If a copy of the MPL was not distributed with this
 file, You can obtain one at http://mozilla.org/MPL/2.0/.
"""

from threading import Thread
import serial
from tkinter import Tk, Button, Label, Entry, W, E, Checkbutton, BooleanVar

ser_write = serial.Serial("/dev/ttyACM0",115200)
ser_read = serial.Serial("/dev/ttyACM0",115200)

angle1 = 0
angle2 = 0
angle3 = 0

listeMoteur1 = [angle1]
listeMoteur2 = [angle2]
listeMoteur3 = [angle3]

def routine():
    send("joint 1 50\n")
    send("joint 2 60\n")

def write():      

    def leftKeyM1(event):
        autoState = automatic()
        
        if autoState == 1:
            moteur1Entry.delete(0,10)
            listeMoteur1[0] -= 10
            moteur1Entry.insert(0,listeMoteur1[0])
            cmd = "joint 1 " + str(listeMoteur1[0]) + "\n"
            print(cmd)
            send(cmd)
        elif autoState == 0:
            moteur1Entry.delete(0,10)
            listeMoteur1[0] -= 10
            moteur1Entry.insert(0,listeMoteur1[0])
            

    def leftKeyM2(event):        
        autoState = automatic()
        
        if autoState == 1:
            moteur2Entry.delete(0,10)
            listeMoteur2[0] -= 10
            moteur2Entry.insert(0,listeMoteur2[0])
            cmd = "joint 2 " + str(listeMoteur2[0]) + "\n"
            print(cmd)
            send(cmd)
        elif autoState == 0:
            moteur2Entry.delete(0,10)
            listeMoteur2[0] -= 10
            moteur2Entry.insert(0,listeMoteur2[0])
                
    def leftKeyM3(event):        
        autoState = automatic()
        
        if autoState == 1:
            moteur3Entry.delete(0,10)
            listeMoteur3[0] -= 10
            moteur3Entry.insert(0,listeMoteur3[0])
            cmd = "joint 3 " + str(listeMoteur3[0]) + "\n"
            print(cmd)
            send(cmd)
        elif autoState == 0:
            moteur3Entry.delete(0,10)
            listeMoteur3[0] -= 10
            moteur3Entry.insert(0,listeMoteur3[0])
            
    def rightKeyM1(event):
        autoState = automatic()
        
        if autoState == 1:
            moteur1Entry.delete(0,10)
            listeMoteur1[0] += 10
            moteur1Entry.insert(0,listeMoteur1[0])
            cmd = "joint 1 " + str(listeMoteur1[0]) + "\n"
            print(cmd)
            send(cmd)
        elif autoState == 0:
            moteur1Entry.delete(0,10)
            listeMoteur1[0] += 10
            moteur1Entry.insert(0,listeMoteur1[0])
        
    def rightKeyM2(event):
        autoState = automatic()
        
        if autoState == 1:
            moteur2Entry.delete(0,10)
            listeMoteur2[0] += 10
            moteur2Entry.insert(0,listeMoteur2[0])
            cmd = "joint 2 " + str(listeMoteur2[0]) + "\n"
            print(cmd)
            send(cmd)
        elif autoState == 0:
            moteur2Entry.delete(0,10)
            listeMoteur2[0] += 10
            moteur2Entry.insert(0,listeMoteur2[0])
        
    def rightKeyM3(event):
        autoState = automatic()
        
        if autoState == 1:
            moteur3Entry.delete(0,10)
            listeMoteur3[0] += 10
            moteur3Entry.insert(0,listeMoteur3[0])
            cmd = "joint 3 " + str(listeMoteur3[0]) + "\n"
            print(cmd)
            send(cmd)
        elif autoState == 0:
            moteur3Entry.delete(0,10)
            listeMoteur3[0] += 10
            moteur3Entry.insert(0,listeMoteur3[0])
    
    def automatic():
        return automaticChoice.get()
    
    def sendAngle(event):
        try:
            valueM1 = int(moteur1Entry.get())
            valueM2 = int(moteur2Entry.get())
            valueM3 = int(moteur3Entry.get())
        except ValueError:
            valueM1 = 0
            valueM2 = 0
            valueM3 = 0
        
        listeMoteur1[0] = valueM1
        listeMoteur2[0] = valueM2
        listeMoteur3[0] = valueM3
        
        cmd1 = "joint 1 " + str(listeMoteur1[0]) + "\n"
        cmd2 = "joint 2 " + str(listeMoteur2[0]) + "\n"
        cmd3 = "joint 3 " + str(listeMoteur3[0]) + "\n"
        
        send(cmd1)
        send(cmd2)
        send(cmd3)
        
        
    def reset(event):
        listeMoteur1[0] = 0
        listeMoteur2[0] = 0
        listeMoteur3[0] = 0
        moteur1Entry.delete(0,10)
        moteur2Entry.delete(0,10)
        moteur3Entry.delete(0,10)
        moteur1Entry.insert(0,0)
        moteur2Entry.insert(0,0)
        moteur3Entry.insert(0,0)

    #-------------moteur 1-------------
    Label(root,text = "moteur 1").grid(row=0,sticky=W, padx=4)
    
    left1Button = Button(root,text="<-")
    right1Button = Button(root,text="->")
    
    left1Button.bind('<Button-1>', leftKeyM1)
    left1Button.grid(row=0, column = 1, sticky = E, padx=4, pady=4)
    
    right1Button.bind('<Button-1>', rightKeyM1)
    right1Button.grid(row=0, column = 2, sticky = E, padx=4,pady=4)
    
    moteur1Entry = Entry(root)
    moteur1Entry.grid(row=0,column=3,sticky=E, padx=4, pady=4)
    
    #-------------moteur 2-------------
    Label(root,text = "moteur 2").grid(row=1,sticky=W, padx=4)
    
    left2Button = Button(root,text="<-")
    right2Button = Button(root,text="->")
    
    left2Button.bind('<Button-1>', leftKeyM2)
    left2Button.grid(row=1, column = 1, sticky = E, padx=4, pady=4)
    
    right2Button.bind('<Button-1>', rightKeyM2)
    right2Button.grid(row=1, column = 2, sticky = E, padx=4,pady=4)
    
    moteur2Entry = Entry(root)
    moteur2Entry.grid(row=1,column=3,sticky=E, padx=4,pady=4)
    
    #-------------moteur 3-------------
    Label(root,text = "moteur 3").grid(row=2,sticky=W, padx=4)
    
    left3Button = Button(root,text="<-")
    right3Button = Button(root,text="->")
    
    left3Button.bind('<Button-1>', leftKeyM3)
    left3Button.grid(row=2, column = 1, sticky = E, padx=4,pady=4)
    
    right3Button.bind('<Button-1>', rightKeyM3)
    right3Button.grid(row=2, column = 2, sticky = E, padx=4,pady=4)
    
    moteur3Entry = Entry(root)
    moteur3Entry.grid(row=2,column=3,sticky=E, padx=4,pady=4)
    
    #-------------Send button-------------
    
    sendButton = Button(root,text="send")
    sendButton.bind("<Button-1>", sendAngle)
    sendButton.grid(row=3,column=3,sticky=W)
    
    #-------------Automatic send----------
    
    automaticChoice = BooleanVar()
    auto = Checkbutton(root,text="automatic send", variable=automaticChoice, command=automatic)
    auto.grid(row=0,column=4, padx=4, pady=4, sticky=W)
    
    #-------------Reset-------------------
    
    resetButton = Button(root,text="reset")
    resetButton.bind("<Button-1>", reset)
    resetButton.grid(row=3, column=1, sticky = E)
    
    #-------------Quit button-------------
    bouton = Button(root, text = "close", command=root.quit)
    bouton.grid(row = 3, column = 0)
        

def read():
    while 1:
        cmd_read = ser_read.readline()
        print(cmd_read.decode('utf-8'))
        
def send(cmd_write):
    ser_write.write(cmd_write.encode())
    print(cmd_write)
        


root = Tk()

t1_write = Thread(target = write)
t2_read = Thread(target = read)

t1_write.start()
t2_read.start()

routine()

root.mainloop()
root.destroy()

t1_write.join()
#t2_read.join()


