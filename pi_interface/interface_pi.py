"""
 This Source Code Form is subject to the terms of the Mozilla Public
 License, v. 2.0. If a copy of the MPL was not distributed with this
 file, You can obtain one at http://mozilla.org/MPL/2.0/.
"""

from threading import Thread,Condition,RLock,Event

from tkinter import Tk, Button, Label, Entry, W, E, Checkbutton, BooleanVar, END, INSERT, Text,Scale, constants, \
    Radiobutton, StringVar
import time
import serial

test_mode = False

if __name__ == '__main__':
    if test_mode:
        __name__ = "__test__"

if __name__ == '__main__':
    ser_write = serial.Serial("/dev/ttyACM1", 9600)
    ser_read = serial.Serial("/dev/ttyACM1", 9600)

else:
    print(" Interface is imported: serial function will be overwritten")
    # Should not be called in test
    ser_write = None
    ser_read = None


rx_line = []
def dummy_send( cmd_write):
    if cmd_write[0] == " ":
        cmd_write = cmd_write[1:]
    else:
        pass
    rx_line.append(cmd_write)
    print(cmd_write)

def send(cmd_write):
    '''  '''
#    pass
    if cmd_write[0] == " ":
        cmd_write = cmd_write[1:]
    else:
        pass

    with send_lock:
        ser_write.write(cmd_write.encode())
        print(cmd_write)


angle1 = 0
angle2 = 0
angle3 = 0

listeMoteur1 = [angle1]
listeMoteur2 = [angle2]
listeMoteur3 = [angle3]

instructionListe = []
command = 1
execute = ""

# Concurrency variable and thread
routine_event_cv = Event()
routine_event_cv.set()
send_lock = RLock()
routine_cv_waiting_for_unlock = False

threads_on = True

root = Tk()
root.geometry("750x300")


FINGERS_MODE_LIST = [("Control with interface", "GUI"),("Control with glove", "GLOVE"),("Lock position", "LOCK"), ("Torque off", "FREE")]

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
    instructionListe.insert(int(float(x)), " " + str(instructEntry.get())+"\n")
    routine.delete("1.0",END)

    for inst in instructionListe:
        routine.insert(END, inst)

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
            routine.insert(END, inst)

        if len(instructionListe) <= 1:
            routine.insert("1.0", "x")
        else:
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
    global routine_event_cv
    x = routine.search("x","1.0", stopindex=END)
    first_loop = True
    while (loop() or first_loop) and threads_on:
        first_loop = False
        for index, inst in enumerate(instructionListe):
            if index < int(float(x))-1:
                pass
            else:
                execute = inst
                send(execute)
                routine_event_cv.clear()
                routine_event_cv.wait(15)         #Wait the nolidge flag or wait 20 seconds

def stop(event):
    stop_func()

def stop_func():
    pass

def execute(event):
    '''  '''
    x = routine.search("x","1.0", stopindex=END)
    #print(x)
    #print(instructionListe)

    #print(instructionListe[int(float(x))-1])
    send(instructionListe[int(float(x))-1])

def finger1_event(value):
    finger_send(1, value)

def finger2_event(value):
    finger_send(2, value)

def finger3_event(value):
    finger_send(3, value)

def finger_send(finger_number,value):

    if finger_control_variable.get() == 'GUI':
        new_msg = "finger_move " + str(finger_number) + " " + str(value) + "\n"
        send(new_msg)

def finger_control_mode(*args):
    new_mode = finger_control_variable.get()

    if new_mode == "LOCK":
        new_msg = "finger_mode LOCK\n"
        send(new_msg)

    elif new_mode == "FREE":
        new_msg = "finger_mode FREE\n"
        send(new_msg)

    elif new_mode == "GLOVE":
        new_msg = "finger_mode GLOVE\n"
        send(new_msg)

    elif new_mode == "GUI":
        new_msg = "finger_mode GUI\n"
        send(new_msg)

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

# butStop = Button(root, height=5, width=15, text = "STOP", bg="red")
# butStop.bind("<Button-1>", stop)
# butStop.place(x = 610, y = 196)

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

#-------------Slider first finger-------------
slider_group_x_offset = 4
slider_group_y_offset = 150

l_Finger1= Label(root,text = "Finger 1")
l_Finger1.place(x = slider_group_x_offset, y = slider_group_y_offset + 20)

sliderFinger1 = Scale(root,  from_=0, to=200, orient=constants.HORIZONTAL, length=150, command=finger1_event)
sliderFinger1.place(x = slider_group_x_offset+66 , y = slider_group_y_offset)

#-------------Slider second finger-------------

l_Finger2= Label(root,text = "Finger 2")
l_Finger2.place(x = slider_group_x_offset, y = slider_group_y_offset + 20+40)

sliderFinger2 = Scale(root,  from_=0, to=200, orient=constants.HORIZONTAL, length=150, command=finger2_event)
sliderFinger2.place(x = slider_group_x_offset+66 , y = slider_group_y_offset+40)

#-------------Slider third finger-------------

l_Finger3= Label(root,text = "Finger 3")
l_Finger3.place(x = slider_group_x_offset, y = slider_group_y_offset + 20+80)

sliderFinger3 = Scale(root,  from_=0, to=200, orient=constants.HORIZONTAL, length=150, command=finger3_event)
sliderFinger3.place(x = slider_group_x_offset+66 , y = slider_group_y_offset+80)

#------------- Finger select control mode -------------
finger_control_variable = StringVar()
finger_control_variable.set(["L"])


radio_butt_offset = 20
radi_butt_index = 0

radioButFingers = [Radiobutton(),Radiobutton(),Radiobutton(),Radiobutton()]


for text, mode in FINGERS_MODE_LIST:
    radioButFingers[radi_butt_index] = Radiobutton(root, text=text, variable=finger_control_variable, value=mode)
    radioButFingers[radi_butt_index].place(x=slider_group_x_offset+240, y=slider_group_y_offset + 15 +
                                                         radio_butt_offset*radi_butt_index)
    radi_butt_index += 1

radioButFingers[2].select()    #Select initialy the lock position
finger_control_variable.trace("w", finger_control_mode)

def read():
    '''  '''
#    pass
    global routine_event_cv
    while threads_on:
        cmd_read = ser_read.readline()
        cmd_read_decoded = cmd_read.decode('utf-8')
        print(cmd_read_decoded)
        print(cmd_read_decoded[:len(cmd_read_decoded)-2])
        
        if cmd_read_decoded[:len(cmd_read_decoded)-2] == 'nolidge':
            try:
                routine_event_cv.set()

            except RuntimeError:
                print("No waiting task")



if __name__ == '__main__':

    t2_read = Thread(target = read)
    t2_read.start()


    root.mainloop()
    root.destroy()

    #t1_write.join()
    threads_on = False;
    t2_read.join()


if __name__ == '__test__':

    send = dummy_send
    root.mainloop()
    root.destroy()

    #t1_write.join()
    threads_on = False;