"""
 * Copyright 2019 Etienne Villemure
 This Source Code Form is subject to the terms of the Mozilla Public
 License, v. 2.0. If a copy of the MPL was not distributed with this
 file, You can obtain one at http://mozilla.org/MPL/2.0/.
"""

from threading import Thread
#import serial
from tkinter import Tk, Button, Label, Entry, W, E, Checkbutton, BooleanVar, END, INSERT, Text

#ser_write = serial.Serial("/dev/ttyACM0",115200)
#ser_read = serial.Serial("/dev/ttyACM0",115200)

angle1 = 0
angle2 = 0
angle3 = 0

listeMoteur1 = [angle1]
listeMoteur2 = [angle2]
listeMoteur3 = [angle3]

instructionListe = []
command = 1
execute = ""

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
        
    def up(event):
        
        global command
        global execute
        
        if command == 1:
            pass
        else:
            x = routine.search("x","1.0",stopindex=END)
            command -= 1
            routine.delete(x)
            routine.insert(float(command),"x")
            x = routine.search("x",str(float(command)),stopindex=END)
            execute = instructionListe[int(float(x))-1]
            print(execute)        
        

    def down(event):
        
        global command
        global execute
        
        if command == len(instructionListe):
            pass
        else:
            x = routine.search("x","1.0",stopindex=END)
            command += 1
            routine.delete(x)
            routine.insert(float(command),"x")
            x = routine.search("x",str(float(command)),stopindex=END)
            execute = instructionListe[int(float(x))-1]
            print(execute)
            
    def add(event):
        
        global command
        
        x = routine.search("x","1.0", stopindex=END)
        instructionListe.insert(int(float(x)), " " + str(instructEntry.get()))
        
        routine.delete("1.0",END)
        
        for inst in instructionListe:
            routine.insert(END, inst + "\n")
            
        routine.insert(x,"x")
    
        
    def delete(event):
        
        x = routine.search("x","1.0", stopindex=END)
        instructionListe.pop(int(float(x))-1)
        
        routine.delete("1.0",END)
        
        for inst in instructionListe:
            routine.insert(END, inst + "\n")
            
        routine.insert(x,"x")
        
    def replace(event):
        
        x = routine.search("x", "1.0", stopindex=END)
        
        routine.delete(x,str(float(x)+1))
        
        instructionListe[int(float(x))-1] = str(instructEntry.get())
        routine.insert(x, " " + str(instructEntry.get()) + "\n")
        
        routine.insert(x,"x")

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
    
    #-------------Routine---------------
    
    routine = Text(root, height = 10, width = 40)
    routine.place(x = 550, y = 0)
    
    butUp = Button(root, text="UP")
    butUp.bind("<Button-1>", up)
    butUp.grid(row=0, column = 5, sticky = E, padx = 4, pady = 4)
    
    butDown = Button(root, text="DOWN")
    butDown.bind("<Button-1>", down)
    butDown.grid(row=1, column = 5, sticky = E, padx = 4, pady = 4)
    
    butAdd = Button(root, text = "ADD")
    butAdd.bind("<Button-1>", add)
    butAdd.grid(row=2, column = 5, sticky = E, padx = 4, pady = 4)
    
    butDelete = Button(root, text = "DELETE")
    butDelete.bind("<Button-1>", delete)
    butDelete.grid(row=3, column = 5, sticky = E, padx = 4, pady = 4)
    
    butReplace = Button(root, text = "REPLACE")
    butReplace.bind("<Button-1>", replace)
    butReplace.grid(row=4, column = 5, sticky = E, padx = 4, pady = 4)
    
    instructEntry = Entry(root)
    instructEntry.grid(row=5, column = 5, sticky = E, padx = 4, pady = 4)
    
    routine.insert(INSERT, "x")
    
    for inst in instructionListe:
        routine.insert(END, inst + "\n")
    
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
    pass
#    while 1:
#        cmd_read = ser_read.readline()
#        print(cmd_read.decode('utf-8'))
        
def send(cmd_write):
    pass
#    ser_write.write(cmd_write.encode())
#    print(cmd_write)
        


root = Tk()
root.geometry("700x250")

t1_write = Thread(target = write)
t2_read = Thread(target = read)

t1_write.start()
t2_read.start()

root.mainloop()
root.destroy()

t1_write.join()
#t2_read.join()


