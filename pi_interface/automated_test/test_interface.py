import unittest
import sys
from tkinter import Tk, Button, Label, Entry, W, E, Checkbutton, BooleanVar, END, INSERT, Text

#sys.path.append('../')
import interface

rx_line = []

class TestInterface(unittest.TestCase):

    def test_add_instruction_list(self):
        test_str = "commande 1 007"
        self.update_events()

        interface.instructEntry.insert(END, test_str)
        interface.butAdd.event_generate("<Button-1>",when="tail")
        self.update_events()
        self.assertEqual(interface.instructionListe[0]," commande 1 007")
        self.clear_entry(interface.instructEntry)


    def test_add_routine_list(self):
        test_str = "commande 1 008"
        self.update_events()

        interface.instructEntry.insert(END, test_str)
        interface.butAdd.event_generate("<Button-1>",when="tail")
        self.update_events()
        self.assertEqual(interface.instructionListe[0]," commande 1 007")
        self.clear_entry(interface.instructEntry)
        interface.instructionListe.pop()

    def update_events(self):
        interface.root.update()
        interface.root.update_idletasks()

    def clear_entry(self,entry):
        entry.delete(0, END)
        self.update_events()


def dummy_send(self, cmd_write):
    rx_line.append(cmd_write)
    print(rx_line[1])

if __name__ == '__main__':

    interface.send = dummy_send
    unittest.main()

# instructEntry.insert(END, "hello_world")

# instructEntry.insert(END, "deuxieme")