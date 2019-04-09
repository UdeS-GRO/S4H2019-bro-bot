"""
 This Source Code Form is subject to the terms of the Mozilla Public
 License, v. 2.0. If a copy of the MPL was not distributed with this
 file, You can obtain one at http://mozilla.org/MPL/2.0/.
"""

from threading import Thread, Condition, RLock, Event

from tkinter import Tk, Button, Label, Entry, W, E, Checkbutton, BooleanVar, END, INSERT, Text,Scale, constants, \
    Radiobutton, StringVar, OptionMenu
import time
import serial

# Set this variable to used the print instead of serial. Good to change the GUI and test
test_mode = True
if __name__ == '__main__':
    if test_mode:
        __name__ = "__test__"

# When the code is imported by the test_interface it should not try to communicate with serial
if __name__ == '__main__':
    ser_write = serial.Serial("/dev/myOpenCR", 9600)
    ser_read = serial.Serial("/dev/myOpenCR", 9600)
else:
    print(" Interface is imported: serial function will be overwritten")
    # Should not be called in test
    ser_write = None
    ser_read = None

# Buffer for the dummy serial in test mode
rx_line = []

def dummy_send(cmd_write):
    """ Send the input string argument to a buffer and print it on the console"""
    if cmd_write[0] == " ":
        cmd_write = cmd_write[1:]
    else:
        pass
    rx_line.append(cmd_write)
    print(cmd_write)

def send(cmd_write):
    """ Send the input string argument to the serial port """
    # Remove a blank character if it's the first character
    if cmd_write[0] == " ":
        cmd_write = cmd_write[1:]
    else:
        pass

    with send_lock_:
        ser_write.write(cmd_write.encode())
        print(cmd_write)


# ====== Definition of global variables  ======
NUMBER_OF_FINGERS = 3
NUMBER_OF_JOINTS = 3

initial_angle_ = 0

listeMoteur1_ = [initial_angle_]
listeMoteur2_ = [initial_angle_]
listeMoteur3_ = [initial_angle_]

instructionListe_ = []
command_ = 1
execute = ""

# ====== Concurrency variable and thread =======
routine_event_cv_ = Event()
routine_event_cv_.set()
send_lock_ = RLock()
routine_cv_waiting_for_unlock_ = False

is_play_thread_alive_ = False
stop_flag_lock_ = RLock()
stop_flag_ = False

threads_on_ = True


# Record thread
class Record:

    lock = RLock()
    arm_flag = False
    hand_flag = False
    event = Event()
    hand_event = Event()
    arm_event = Event()

    event.clear()
    hand_event.clear()
    arm_event.clear()

    recorded_hand_value = [0, 0, 0, 0]
    recorded_arm_value = [0, 0, 0, 0]
    rx_hand_cnt = 0
    rx_arm_cnt = 0

# Instance
record = Record



root = Tk()
root.geometry("750x300")

# dropdown list variable
list_choiceHoming = StringVar(root)

FINGERS_MODE_LIST = [("Control with interface", "GUI"),("Control with glove", "GLOVE"),("Lock position", "LOCK"), ("Torque off", "FREE")]

def leftKeyM1(event):
    """ Send command from s GUI left arrow to serial for motor 1 """
    autoState = automatic()

    if autoState == 1:
        moteur1Entry.delete(0,10)
        listeMoteur1_[0] -= 10
        moteur1Entry.insert(0, listeMoteur1_[0])
        cmd = "moveto 1 " + str(listeMoteur1_[0]) + "\n"
        send(cmd)
    elif autoState == 0:
        moteur1Entry.delete(0,10)
        listeMoteur1_[0] -= 10
        moteur1Entry.insert(0, listeMoteur1_[0])


def leftKeyM2(event):
    """ Send command from s GUI left arrow to serial for motor 2 """
    autoState = automatic()

    if autoState == 1:
        moteur2Entry.delete(0,10)
        listeMoteur2_[0] -= 10
        moteur2Entry.insert(0, listeMoteur2_[0])
        cmd = "moveto 2 " + str(listeMoteur2_[0]) + "\n"
        send(cmd)
    elif autoState == 0:
        moteur2Entry.delete(0,10)
        listeMoteur2_[0] -= 10
        moteur2Entry.insert(0, listeMoteur2_[0])


def leftKeyM3(event):
    """ Send command from s GUI left arrow to serial for motor 3 """
    autoState = automatic()

    if autoState == 1:
        moteur3Entry.delete(0,10)
        listeMoteur3_[0] -= 10
        moteur3Entry.insert(0, listeMoteur3_[0])
        cmd = "moveto 3 " + str(listeMoteur3_[0]) + "\n"
        send(cmd)
    elif autoState == 0:
        moteur3Entry.delete(0,10)
        listeMoteur3_[0] -= 10
        moteur3Entry.insert(0, listeMoteur3_[0])


def rightKeyM1(event):
    """ Send command from s GUI right arrow to serial for motor 1 """
    autoState = automatic()

    if autoState == 1:
        moteur1Entry.delete(0,10)
        listeMoteur1_[0] += 10
        moteur1Entry.insert(0, listeMoteur1_[0])
        cmd = "moveto 1 " + str(listeMoteur1_[0]) + "\n"
        send(cmd)
    elif autoState == 0:
        moteur1Entry.delete(0,10)
        listeMoteur1_[0] += 10
        moteur1Entry.insert(0, listeMoteur1_[0])


def rightKeyM2(event):
    """ Send command from s GUI right arrow to serial for motor 2 """
    autoState = automatic()

    if autoState == 1:
        moteur2Entry.delete(0,10)
        listeMoteur2_[0] += 10
        moteur2Entry.insert(0, listeMoteur2_[0])
        cmd = "moveto 2 " + str(listeMoteur2_[0]) + "\n"
        send(cmd)
    elif autoState == 0:
        moteur2Entry.delete(0,10)
        listeMoteur2_[0] += 10
        moteur2Entry.insert(0, listeMoteur2_[0])


def rightKeyM3(event):
    """ Send command from s GUI right arrow to serial for motor 3 """
    autoState = automatic()

    if autoState == 1:
        moteur3Entry.delete(0,10)
        listeMoteur3_[0] += 10
        moteur3Entry.insert(0, listeMoteur3_[0])
        cmd = "moveto 3 " + str(listeMoteur3_[0]) + "\n"
        send(cmd)
    elif autoState == 0:
        moteur3Entry.delete(0,10)
        listeMoteur3_[0] += 10
        moteur3Entry.insert(0, listeMoteur3_[0])


def automatic():
    """ Return the state of the automatic checkbutton """
    return automaticChoice.get()


def sendAngle(event):
    """ Read the angle entry and send the three result over serial """
    try:
        valueM1 = int(moteur1Entry.get())
        valueM2 = int(moteur2Entry.get())
        valueM3 = int(moteur3Entry.get())
    except ValueError:
        valueM1 = 0
        valueM2 = 0
        valueM3 = 0

    listeMoteur1_[0] = valueM1
    listeMoteur2_[0] = valueM2
    listeMoteur3_[0] = valueM3

    cmd1 = "moveto 1 " + str(listeMoteur1_[0]) + "\n"
    cmd2 = "moveto 2 " + str(listeMoteur2_[0]) + "\n"
    cmd3 = "moveto 3 " + str(listeMoteur3_[0]) + "\n"

    send(cmd1)
    send(cmd2)
    send(cmd3)


def reset(event):
    """  Reset the entry values """
    listeMoteur1_[0] = 0
    listeMoteur2_[0] = 0
    listeMoteur3_[0] = 0
    moteur1Entry.delete(0,10)
    moteur2Entry.delete(0,10)
    moteur3Entry.delete(0,10)
    moteur1Entry.insert(0,0)
    moteur2Entry.insert(0,0)
    moteur3Entry.insert(0,0)


def up(event):
    """ Change the x indicator in the routine menu upward """

    global command_
    global execute

    if not instructionListe_:
        pass
    else:
        # If already at the top
        if command_ == 1:
            pass
        else:
            x = routine.search("x","1.0",stopindex=END)
            command_ -= 1
            routine.delete(x)
            routine.insert(float(command_), "x")
            x = routine.search("x", str(float(command_)), stopindex=END)
            execute = instructionListe_[int(float(x)) - 1]


def down(event):
    """ Change the x indicator in the routine menu downward """

    global command_
    global execute

    if not instructionListe_:
        pass
    else:
        # If already at the last command
        if command_ == len(instructionListe_):
            pass
        else:
            x = routine.search("x","1.0",stopindex=END)
            command_ += 1
            routine.delete(x)
            routine.insert(float(command_), "x")
            x = routine.search("x", str(float(command_)), stopindex=END)
            execute = instructionListe_[int(float(x)) - 1]


def add(event):
    """ Add an instruction at the line after the one with the x indicator int the routine text """

    global command_
    x = routine.search("x","1.0", stopindex=END)
    instructionListe_.insert(int(float(x)), " " + str(instructEntry.get()) + "\n")
    routine.delete("1.0",END)

    # Rebuilt the text
    for inst in instructionListe_:
        routine.insert(END, inst)

    routine.insert(x,"x")


def delete(event):
    """ Delete the instruction at the x indicator and move upward all elements below """

    if not instructionListe_:
        pass
    else:
        x = routine.search("x","1.0", stopindex=END)
        instructionListe_.pop(int(float(x)) - 1)

        routine.delete("1.0",END)

        for inst in instructionListe_:
            routine.insert(END, inst)

        if len(instructionListe_) <= 1:
            routine.insert("1.0", "x")
        else:
            routine.insert(x,"x")


def replace(event):
    """ Replace the x indicated line with the new entry command """

    if not instructionListe_:
        pass
    else:
        x = routine.search("x", "1.0", stopindex=END)

        routine.delete(x,str(float(x)+1))

        instructionListe_[int(float(x)) - 1] = str(instructEntry.get())
        routine.insert(x, " " + str(instructEntry.get()) + "\n")

        routine.insert(x,"x")


def play_thread(event):
    """ Call the routine thread if not already alive """
    global is_play_thread_alive_

    try :
        play_thread.t_play = Thread(target = play)
    except RuntimeError:
        print("Thread actually running")

    if not play_thread.t_play.is_alive():
        is_play_thread_alive_ = True
        play_thread.t_play.start()
    else:
        print( "There is actually a routine running")


def loop():
    """ Returns the value of the loop checkbox """
    return loopRoutine.get()


def play():
    """ Sends all command from the x indicated line to the last over serial. Repeats if the loop mode is activated """
    global routine_event_cv_
    global stop_flag_
    global is_play_thread_alive_
    cmd_bloc = []
    is_bloc_encounter = False
    
#    x = routine.search("x","1.0", stopindex=END)
    first_loop = True
    check_parenthesis = check_bloc()
    
    while (loop() or first_loop) and threads_on_ and not stop_flag_ and check_parenthesis:
        first_loop = False
        for index, inst in enumerate(instructionListe_):

            if inst == ' (\n':
                is_bloc_encounter = True
                
            elif inst == ' )\n':
                is_bloc_encounter = False
                send_bloc(cmd_bloc)
                cmd_bloc.clear()
                
            elif is_bloc_encounter:
                cmd_bloc.append(inst)
                
            else:
                    execute = inst
                    send(execute)
                    # Wait for the "nolidge" flag or timeout before continuing to the next command
                    routine_event_cv_.clear()
                    routine_event_cv_.wait(15)         #Wait the nolidge flag or wait 20 seconds
    
            with stop_flag_lock_:
                if stop_flag_:
                    break
    with stop_flag_lock_:
        stop_flag_ = False
        is_play_thread_alive_ = False
    
        
def check_bloc():
    """ Returns true if the routine contains the same number of openning and closing parenthesis  and false otherwise"""
    compteur_left  = 0
    compteur_right = 0
    
    for inst in instructionListe_:
        if inst == ' (\n':
            compteur_left += 1
        elif inst == ' )\n':
            compteur_right += 1
            
    if compteur_left == compteur_right:
        check = True
    else:
        check = False
        print('Missing parenthesis')
    
    return check


def send_bloc(cmd_list):
    """ Sends the input list over serial and waits for all nolidge """
    cnt_nolidge =0
    
    for inst in cmd_list:
        cmd_buffer = inst.split()
        if cmd_buffer[0] == 'moveto':
            cnt_nolidge += 1
        send(inst)

    # Wait for the same number of moveto and nolidge flag
    for counter in range(0, cnt_nolidge):

        routine_event_cv_.clear()
        routine_event_cv_.wait(10)  # Wait the nolidge flag or wait 20 seconds


def Homing(event):
    """ Send the zero comand to the motor specified by the OptionMenu of index when the homing button is pressed"""
    if list_choiceHoming.get() == 'all':
        for indexHoming in range(0, NUMBER_OF_JOINTS):
            cmdHoming = "zero " + str(indexHoming + 1) + "\n"
            send(cmdHoming)
    else:
        cmdHoming = "zero " + str(list_choiceHoming.get()) + "\n"
        send(cmdHoming)


def record_hand(event):
    """ Call the record_hand function when the RECORD_HAND button is pressed"""
    record_hand_func()


def record_hand_func():
    """ Send a message to request a recording of actual hand position and notify the record thread to wait for results"""
    send("record_hand\n")
    with record.lock:
        record.rx_hand_cnt = 0
        record.hand_flag = True
        record.event.set()


def record_arm(event):
    """ Call the record_arm function when the RECORD_ARM button is pressed"""
    record_arm_func()


def record_arm_func():
    """ Send a message to request a recording of actual arm position and notify the record thread to wait for results"""
    send("record_arm\n")
    with record.lock:
        record.rx_arm_cnt = 0
        record.arm_flag = True
        record.event.set()


def record_thread():
    """ When notified, wait for the request position and insert it in the instruction list and routine """
    while threads_on_:

        record.event.wait()        # Wait until notified
        record.event.clear()       # Reset the wait

        # Look which record function woke up the thread
        with record.lock:
            actual_arm_record_status = record.arm_flag
            actual_hand_record_status = record.hand_flag

        if actual_arm_record_status:
            if record.arm_event.wait(5.0): # Wait for all recorded position flag or wait 5 seconds
                add_records_to_routine()
            else:
                print("Timeout error: Record")
            record.arm_event.clear()
            record.arm_flag = False

        if actual_hand_record_status:
            if record.hand_event.wait(5.0): # Wait for all recorded position flag or wait 5 seconds
                add_records_to_routine()
            else:
                print("Timeout error: Record")
            record.hand_event.clear()
            record.hand_flag = False


def add_records_to_routine():
    """ Adds the values in a value list from the class Record to the InstructionListe and routine Entry"""
    global command_
    first_element = False

    if len(instructionListe_) == 0:
        first_element = True

    if record.arm_flag:
        x = routine.search("x", "1.0", stopindex=END)
        if first_element:
            instructionListe_.insert(int(float(x)), " (" + "\n")
            instructionListe_.insert(int(float(x)), " )" + "\n")
        else:
            instructionListe_.insert(int(float(x)), " )" + "\n")

        for index in range(1,NUMBER_OF_JOINTS+1):
            instructionListe_.insert(int(float(x))," " + "moveto " +str(index) + " " + str(record.recorded_arm_value[index]) + "\n")

        if not first_element:
            instructionListe_.insert(int(float(x)), " (" + "\n")

        # Rebuilt the text
        routine.delete("1.0", END)
        for inst in instructionListe_:
            routine.insert(END, inst)

        routine.insert(x, "x")

    if record.hand_flag:
        x = routine.search("x", "1.0", stopindex=END)

        if first_element:
            instructionListe_.insert(int(float(x)), " (" + "\n")
            instructionListe_.insert(int(float(x)), " )" + "\n")
        else:
            instructionListe_.insert(int(float(x)), " )" + "\n")

        for index in range(1,NUMBER_OF_FINGERS+1):
            instructionListe_.insert(int(float(x))," " + "finger_move " +str(index) + " " + str(record.recorded_hand_value[index]) + "\n")

        if not first_element:
            instructionListe_.insert(int(float(x)), " (" + "\n")

        # Rebuilt the text
        routine.delete("1.0", END)
        for inst in instructionListe_:
            routine.insert(END, inst)

        routine.insert(x, "x")


def stop(event):
    """ Call the stop function when stop button is pressed"""
    stop_func()


def stop_func():
    """ Send the stop command over serial and set a stop flag for the routine"""
    global stop_flag_

    # Sets only the stop flag if the routine is alive
    if is_play_thread_alive_:
        with stop_flag_lock_:
            stop_flag_ = True

    send("stop\n")


def execute(event):
    """ Send the x-selected routine command """
    x = routine.search("x","1.0", stopindex=END)
    send(instructionListe_[int(float(x)) - 1])


def finger1_event(value):
    """ Sends a the value for the first finger by serial """
    finger_send(1, value)


def finger2_event(value):
    """ Sends a the value for the second finger by serial """
    finger_send(2, value)


def finger3_event(value):
    """ Sends a the value for the third finger by serial """
    finger_send(3, value)


def finger_send(finger_number, value):
    """ Builds a message to send by serial destined for the finger_number at the value by serial  """
    if finger_control_variable.get() == 'GUI':
        new_msg = "finger_move " + str(finger_number) + " " + str(value) + "\n"
        send(new_msg)


def finger_control_mode(*args):
    """ Reads the state of the finger mode radio buttons and send that state over serial"""
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

butRecord = Button(root, text = "RECORD HAND")
butRecord.bind("<Button-1>", record_hand)
butRecord.place(x = 485, y = 228)

butRecord = Button(root, text = "RECORD ARM")
butRecord.bind("<Button-1>", record_arm)
butRecord.place(x = 485, y = 260)

instructEntry = Entry(root)
instructEntry.place(x = 585, y = 180)

butStop = Button(root, height=4, width=10, text = "STOP", bg="red")
butStop.bind("<Button-1>", stop)
butStop.place(x = 655, y = 215)

routine.insert(INSERT, "x")

for inst in instructionListe_:
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
loopRout.grid(row =0, column = 8, padx=4, pady= 4, sticky=W)

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

sliderFinger1 = Scale(root,  from_=0, to=180, orient=constants.HORIZONTAL, length=150, command=finger1_event)
sliderFinger1.place(x = slider_group_x_offset+66 , y = slider_group_y_offset)

#-------------Slider second finger-------------

l_Finger2= Label(root,text = "Finger 2")
l_Finger2.place(x = slider_group_x_offset, y = slider_group_y_offset + 20+40)

sliderFinger2 = Scale(root,  from_=0, to=180, orient=constants.HORIZONTAL, length=150, command=finger2_event)
sliderFinger2.place(x = slider_group_x_offset+66 , y = slider_group_y_offset+40)

#-------------Slider third finger-------------

l_Finger3= Label(root,text = "Finger 3")
l_Finger3.place(x = slider_group_x_offset, y = slider_group_y_offset + 20+80)

sliderFinger3 = Scale(root,  from_=0, to=180, orient=constants.HORIZONTAL, length=150, command=finger3_event)
sliderFinger3.place(x = slider_group_x_offset+66 , y = slider_group_y_offset+80)

#------------- Finger select control mode -------------
finger_control_variable = StringVar()
finger_control_variable.set(["L"])


radio_butt_offset = 20
radi_butt_index = 0

# Initialize the radioButFingers array
radioButFingers = [Radiobutton(), Radiobutton(), Radiobutton(), Radiobutton()]

# Set the radio buttons
for text, mode in FINGERS_MODE_LIST:
    radioButFingers[radi_butt_index] = Radiobutton(root, text=text, variable=finger_control_variable, value=mode)
    radioButFingers[radi_butt_index].place(x=slider_group_x_offset+240, y=slider_group_y_offset + 15 +
                                                         radio_butt_offset*radi_butt_index)
    radi_butt_index += 1

radioButFingers[2].select()    #Select initialy the lock position

# Call the finger_control_mode function when the radio buttons change state
finger_control_variable.trace("w", finger_control_mode)

# ------------- Homing ---------------

homingButton = Button(root, text="Homing")
homingButton.bind("<Button-1>", Homing)
homingButton.grid(row=3, column=4, sticky=W)

# ------------ DropList ---------------

choices = ['all', '1', '2', '3']
popupList = OptionMenu(root, list_choiceHoming, *choices)
# Label(root, text = "Choose the axe").grid( row = 2, column = 4, sticky = W)
popupList.grid(row=3, column=4, sticky=E)
list_choiceHoming.set('all')


def read():
    """ Reads the serial port and prints the result to the console. If the message "nolidg" is received, wake up routine"""
    global routine_event_cv_

    while threads_on_:
        cmd_read = ser_read.readline()
        cmd_read_decoded = cmd_read.decode('utf-8')
        #print(cmd_read_decoded)
        print(cmd_read_decoded[:len(cmd_read_decoded)-2])
        
        if cmd_read_decoded[:len(cmd_read_decoded)-2] == 'nolidge':
            try:
                routine_event_cv_.set()
                
            except RuntimeError:
                print("No waiting task")

        read_record()   # Verify if the message is for the record thread


def read_record(rx_string):
    """ Verify if the received string is for the record thread and add the position value to the corresponding
    recorded_hand or recorded_arm value list"""

    if rx_string[0] == "recorded_hand" and not record.hand_event.isSet():
        if rx_string[1] in range(0, NUMBER_OF_FINGERS):
            record.recorded_hand_value[rx_string[1]] = rx_string[2]
            record.rx_hand_cnt += 1

    if rx_string[0] == "recorded_arm" and not record.arm_event.isSet():
        if rx_string[1] in range(0, NUMBER_OF_JOINTS):
            record.recorded_arm_value[rx_string[1]] = rx_string[2]
            record.rx_arm_cnt += 1

    if not record.hand_event.isSet() and record.rx_arm_cnt >= NUMBER_OF_JOINTS:
        record.hand_event.set()  # All record position are received

    if not record.arm_event.isSet() and record.rx_hand_cnt >= NUMBER_OF_FINGERS:
        record.arm_event.set()     # All record position are received



# If the code is not run in test mode and it's not imported
if __name__ == '__main__':
    t2_read = Thread(target = read)
    t2_read.start()

    t_record = Thread(target = record_thread)
    t_record.start()

    root.mainloop()

    #t1_write.join()
    threads_on_ = False
    t2_read.join()
    record.event.set()
    t_record.join()

# If the code is run with test flag to true. Set the send function to a dummy send function and don't call the serial thread
if __name__ == '__test__':

    t_record = Thread(target = record_thread)
    t_record.start()

    send = dummy_send
    root.mainloop()

    #t1_write.join()
    threads_on_ = False
    record.event.set()
    t_record.join()