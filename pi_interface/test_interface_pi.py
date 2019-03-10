import unittest
import sys
from tkinter import Tk, Button, Label, Entry, W, E, Checkbutton, BooleanVar, END, INSERT, Text

#sys.path.append('../')
import interface_pi

rx_line = []

class TestInterface(unittest.TestCase):

    def test_add_instruction_list(self):
        test_str = "commande 1 007"
        self.update_events()

        interface_pi.instructEntry.insert(END, test_str)
        interface_pi.butAdd.event_generate("<Button-1>",when="tail")
        self.update_events()
        self.assertEqual(interface_pi.instructionListe[0]," commande 1 007")
        self.clear_entry(interface_pi.instructEntry)
        interface_pi.instructionListe.pop()


    def test_add_routine_list(self):
        #Setup the test
        test_str = "commande 1 008"
        self.update_events()
        interface_pi.instructEntry.insert(END, test_str)
        interface_pi.butAdd.event_generate("<Button-1>",when="tail")
        self.update_events()

        # Test
        self.assertEqual(interface_pi.routine.get('1.0',END),"x commande 1 008\n")

        #Clear test
        self.clear_entry(interface_pi.instructEntry)
        interface_pi.instructionListe.pop()

    def update_events(self):
        interface_pi.root.update()
        interface_pi.root.update_idletasks()

    def clear_entry(self,entry):
        entry.delete(0, END)
        self.update_events()


def dummy_send(self, cmd_write):
    rx_line.append(cmd_write)
    print(rx_line[1])

if __name__ == '__main__':
    interface_pi.send = dummy_send
    unittest.main()

# instructEntry.insert(END, "hello_world")

# instructEntry.insert(END, "deuxieme")