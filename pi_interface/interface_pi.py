"""
 This Source Code Form is subject to the terms of the Mozilla Public
 License, v. 2.0. If a copy of the MPL was not distributed with this
 file, You can obtain one at http://mozilla.org/MPL/2.0/.
"""

from threading import Thread,Condition,RLock

from tkinter import Tk, Button, Label, Entry, W, E, Checkbutton, BooleanVar, END, INSERT, Text
import time
import serial

if __name__ == '__main__':
    ser_write = serial.Serial("/dev/ttyACM1", 9600)
    ser_read = serial.Serial("/dev/ttyACM1", 9600)
    isTEST = False
else:
    print(" Interface is imported: serial function will be overwritten")
    # Should not be called in test
    ser_write = None
    ser_read = None
    isTEST = True

angle1 = 0
angle2 = 0
angle3 = 0

listeMoteur1 = [angle1]
listeMoteur2 = [angle2]
listeMoteur3 = [angle3]

instructionListe = []
command = 1
execute = ""

routine_cv = Condition()
send_lock = RLock()

root = Tk()
root.geometry("750x250")


def leftKeyM1(event):
    '''  '''
    autoState = automatic()

    if autoState == 1:
        moteur1Entry.delete(0,10)
        listeMoteur1[0] -= 10
        moteur1Entry.insert(0,listeMoteur1[0])
        cmd = "moveto 1 " + str(listeMoteur1[0]) + "\n"
        send(cmd)
    elif autoState == 0:
        moteur1Entry.delete(0,10)
        listeMoteur1[0] -= 10
        moteur1Entry.insert(0,listeMoteur1[0])


def leftKeyM2(event):
    '''  '''
    autoState = automatic()

    if autoState == 1:
        moteur2Entry.delete(0,10)
        listeMoteur2[0] -= 10
        moteur2Entry.insert(0,listeMoteur2[0])
        cmd = "moveto 2 " + str(listeMoteur2[0]) + "\n"
        send(cmd)
    elif autoState == 0:
        moteur2Entry.delete(0,10)
        listeMoteur2[0] -= 10
        moteur2Entry.insert(0,listeMoteur2[0])

def leftKeyM3(event):
    '''  '''
    autoState = automatic()

    if autoState == 1:
        moteur3Entry.delete(0,10)
        listeMoteur3[0] -= 10
        moteur3Entry.insert(0,listeMoteur3[0])
        cmd = "moveto 3 " + str(listeMoteur3[0]) + "\n"
        send(cmd)
    elif autoState == 0:
        moteur3Entry.delete(0,10)
        listeMoteur3[0] -= 10
        moteur3Entry.insert(0,listeMoteur3[0])

def rightKeyM1(event):
    '''  '''
    autoState = automatic()

    if autoState == 1:
        moteur1Entry.delete(0,10)
        listeMoteur1[0] += 10
        moteur1Entry.insert(0,listeMoteur1[0])
        cmd = "moveto 1 " + str(listeMoteur1[0]) + "\n"
        send(cmd)
    elif autoState == 0:
        moteur1Entry.delete(0,10)
        listeMoteur1[0] += 10
        moteur1Entry.insert(0,listeMoteur1[0])

def rightKeyM2(event):
    '''  '''
    autoState = automatic()

    if autoState == 1:
        moteur2Entry.delete(0,10)
        listeMoteur2[0] += 10
        moteur2Entry.insert(0,listeMoteur2[0])
        cmd = "moveto 2 " + str(listeMoteur2[0]) + "\n"
        send(cmd)
    elif autoState == 0:
        moteur2Entry.delete(0,10)
        listeMoteur2[0] += 10
        moteur2Entry.insert(0,listeMoteur2[0])

def rightKeyM3(event):
    '''  '''
    autoState = automatic()

    if autoState == 1:
        moteur3Entry.delete(0,10)
        listeMoteur3[0] += 10
        moteur3Entry.insert(0,listeMoteur3[0])
        cmd = "moveto 3 " + str(listeMoteur3[0]) + "\n"
        send(cmd)
    elif autoState == 0:
        moteur3Entry.delete(0,10)
        listeMoteur3[0] += 10
        moteur3Entry.insert(0,listeMoteur3[0])

def automatic():
    '''  '''
    return automaticChoice.get()

def sendAngle(event):
    '''  '''
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

    cmd1 = "moveto 1 " + str(listeMoteur1[0]) + "\n"
    cmd2 = "moveto 2 " + str(listeMoteur2[0]) + "\n"
    cmd3 = "moveto 3 " + str(listeMoteur3[0]) + "\n"

    send(cmd1)
    send(cmd2)
    send(cmd3)

#        instructionListe.append(" " + cmd1)
#        instructionListe.append(" " + cmd2)
#        instructionListe.append(" " + cmd3)

def reset(event):
    '''  '''
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
    '''  '''

    global command
    global execute

    if not instructionListe:
        pass
    else:
        if command == 1:
            pass
        else:
            x = routine.search("x","1.0",stopindex=END)
            command -= 1
            routine.delete(x)
            routine.insert(float(command),"x")
            x = routine.search("x",str(float(command)),stopindex=END)
            execute = instructionListe[int(float(x))-1]


def down(event):
    '''  '''

    global command
    global execute

    if not instructionListe:
        pass
    else:
        if command == len(instructionListe):
            pass
        else:
            x = routine.search("x","1.0",stopindex=END)
            command += 1
            routine.delete(x)
            routine.insert(float(command),"x")
            x = routine.search("x",str(float(command)),stopindex=END)
            execute = instructionListe[int(float(x))-1]

def add(event):
    '''  '''

    global command
    x = routine.search("x","1.0", stopindex=END)
    instructionListe.insert(int(float(x)), " " + str(instructEntry.get()))
    routine.delete("1.0",END)

    for inst in instructionListe:
        routine.insert(END, inst) #+ "\n")

    routine.insert(x,"x")


def delete(event):
    '''  '''

    if not instructionListe:
        pass
    else:
        x = routine.search("x","1.0", stopindex=END)
        instructionListe.pop(int(float(x))-1)

        routine.delete("1.0",END)

        for inst in instructionListe:
            routine.insert(END, inst + "\n")

        routine.insert(x,"x")

def replace(event):
    '''  '''

    if not instructionListe:
        pass
    else:
        x = routine.search("x", "1.0", stopindex=END)

        routine.delete(x,str(float(x)+1))

        instructionListe[int(float(x))-1] = str(instructEntry.get())
        routine.insert(x, " " + str(instructEntry.get()) + "\n")

        routine.insert(x,"x")

def play_thread(event):
    '''  '''

    try :
        play_thread.t_play = Thread(target = play)
    except RuntimeError:
        print("Thread actually running")

    if not play_thread.t_play.is_alive():
        play_thread.t_play.start()
    else:
        print( "There is actually a routine running")

def loop():
    '''  '''
    return loopRoutine.get()

def play():
    '''  '''
    global routine_cv
    x = routine.search("x","1.0", stopindex=END)

    for index, inst in enumerate(instructionListe):
        if index < int(float(x))-1:
            pass
        else:
            execute = inst
            send(execute)
            with routine_cv:
                routine_cv.wait(20)         #Wait the nolidge flag or wait 20 seconds


def execute(event):
    '''  '''
    x = routine.search("x","1.0", stopindex=END)
    print(x)
    print(instructionListe)

    print(instructionListe[int(float(x))-1])
    send(instructionListe[int(float(x))-1])


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

routine = Text(root, height = 10, width = 20)
routine.place(x = 585, y = 4)

butUp = Button(root, text="UP")
butUp.bind("<Button-1>", up)
butUp.place(x = 485, y = 4)

butDown = Button(root, text="DOWN")
butDown.bind("<Button-1>", down)
butDown.place(x = 485, y = 36)

butAdd = Button(root, text = "ADD")
butAdd.bind("<Button-1>", add)
butAdd.place(x = 485, y = 68)

butDelete = Button(root, text = "DELETE")
butDelete.bind("<Button-1>", delete)
butDelete.place(x = 485, y = 100)

butReplace = Button(root, text = "REPLACE")
butReplace.bind("<Button-1>", replace)
butReplace.place(x = 485, y = 132)

butRun = Button(root, text = "PLAY")
butRun.bind("<Button-1>", play_thread)
butRun.place(x = 485, y = 164)

butExecute = Button(root, text = "EXECUTE")
butExecute.bind("<Button-1>", execute)
butExecute.place(x = 485, y = 196)

instructEntry = Entry(root)
instructEntry.place(x = 585, y = 160)


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

#-------------Loop Routine ----------
loopRoutine = BooleanVar()
loopRout = Checkbutton(root, text="Loop", variable =loopRoutine, command=loop)
loopRout.grid(row =1, column = 4, pady= 4, sticky=W)

#-------------Reset-------------------

resetButton = Button(root,text="reset")
resetButton.bind("<Button-1>", reset)
resetButton.grid(row=3, column=1, sticky = E)

#-------------Quit button-------------
bouton = Button(root, text = "close", command=root.quit)
bouton.grid(row = 3, column = 0)
        

def read():
    '''  '''
#    pass
    global routine_cv
    while 1:
        cmd_read = ser_read.readline()
        cmd_read_decoded = cmd_read.decode('utf-8')
        print(cmd_read_decoded)
        print(cmd_read_decoded[:len(cmd_read_decoded)-2])
        
        if cmd_read_decoded[:len(cmd_read_decoded)-2] == 'nolidge':
            try:
                with routine_cv:
                    routine_cv.notify_all()

            except RuntimeError:
                print("No waiting task")


def send(cmd_write):
    '''  '''
#    pass
    if cmd_write[0] == " ":
        cmd_write = cmd_write[1:]
    else:
        pass
    ser_write.write(cmd_write.encode())

    with send_lock:
        print(cmd_write)





if __name__ == '__main__':

    #t1_write = Thread(target = write)
    t2_read = Thread(target = read)
    t2_read.start()
    #t1_write.start()


    root.mainloop()
    root.destroy()

    #t1_write.join()

    t2_read.join()



