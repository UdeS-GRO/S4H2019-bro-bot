"""
 * Copyright 2019 Etienne Villemure
 This Source Code Form is subject to the terms of the Mozilla Public
 License, v. 2.0. If a copy of the MPL was not distributed with this
 file, You can obtain one at http://mozilla.org/MPL/2.0/.
"""

from tkinter import Tk, Button, Label, Entry, W, E, END
angle1 = 0
angle2 = 0
angle3 = 0

listeMoteur1 = [angle1]
listeMoteur2 = [angle2]
listeMoteur3 = [angle3]

listeButton = []

def leftKey(event):
#    btt = event.GetEventObject()
    
    moteur1Entry.delete(0,10)
    liste[0] -= 1
    
    moteur1Entry.insert(0,liste[0])
    print("Left key pressed")


def rightKey(event):
#    btt = event.GetEventObject()
    moteur1Entry.delete(0,10)
    liste[0] += 1
    
    moteur1Entry.insert(0,liste[0])
    print("Right key pressed")
    
def reset(event):
    angle = 0
    moteur1Entry.delete(0,10)
    moteur2Entry.delete(0,10)
    moteur3Entry.delete(0,10)
    moteur1Entry.insert(0,angle)
    moteur2Entry.insert(0,angle)
    moteur3Entry.insert(0,angle)

root = Tk()

#-------------moteur 1-------------
Label(root,text = "moteur 1").grid(row=0,sticky=W, padx=4)

left1Button = Button(root,text="<-")
right1Button = Button(root,text="->")

left1Button.bind('<Button-1>', leftKey)
left1Button.grid(row=0, column = 1, sticky = E, padx=4, pady=4)

right1Button.bind('<Button-1>', rightKey)
right1Button.grid(row=0, column = 2, sticky = E, padx=4,pady=4)

moteur1Entry = Entry(root)
moteur1Entry.grid(row=0,column=3,sticky=E, padx=4, pady=4)

#-------------moteur 2-------------
Label(root,text = "moteur 2").grid(row=1,sticky=W, padx=4)

left2Button = Button(root,text="<-")
right2Button = Button(root,text="->")

left2Button.bind('<Button-2>', leftKey)
left2Button.grid(row=1, column = 1, sticky = E, padx=4, pady=4)

right2Button.bind('<Button-2>', rightKey)
right2Button.grid(row=1, column = 2, sticky = E, padx=4,pady=4)

moteur2Entry = Entry(root)
moteur2Entry.grid(row=1,column=3,sticky=E, padx=4,pady=4)

#-------------moteur 3-------------
Label(root,text = "moteur 3").grid(row=2,sticky=W, padx=4)

left3Button = Button(root,text="<-")
right3Button = Button(root,text="->")

left3Button.bind('<Button-3>', leftKey)
left3Button.grid(row=2, column = 1, sticky = E, padx=4,pady=4)

right3Button.bind('<Button-3>', rightKey)
right3Button.grid(row=2, column = 2, sticky = E, padx=4,pady=4)

moteur3Entry = Entry(root)
moteur3Entry.grid(row=2,column=3,sticky=E, padx=4,pady=4)


#-------------Liste bouton------------

listeButton = [left1Button,right1Button,
               left2Button,right2Button,
               left3Button,right3Button]

#-------------Reset-------------------

resetButton = Button(root,text="reset")
resetButton.bind("<Button-1>",reset)
resetButton.grid(row=3, column=1, sticky = E)

#-------------Quit button-------------
bouton = Button(root, text = "close", command=root.quit)
bouton.grid(row = 3, column = 0)


root.mainloop()
root.destroy()

#from tkinter import Tk, Frame, Button
#
#
#def key(event):
#    """shows key or tk code for the key"""
#    if event.keysym == 'Escape':
#        root.destroy()
#    if event.char == event.keysym:
#        # normal number and letter characters
#        print( 'Normal Key %r' % event.char )
#    elif len(event.char) == 1:
#        # charcters like []/.,><#$ also Return and ctrl/key
#        print( 'Punctuation Key %r (%r)' % (event.keysym, event.char) )
#    else:
#        # f1 to f12, shift keys, caps lock, Home, End, Delete ...
#        print( 'Special Key %r' % event.keysym )
#        
#root = Tk()
#print( "Press a key (Escape key to exit):" )
#root.bind_all('<Key>', key)
## don't show the tk window
#root.mainloop()










