import unittest
import sys
from tkinter import Tk, Button, Label, Entry, W, E, Checkbutton, BooleanVar, END, INSERT, Text

#sys.path.append('../')
import interface_pi

rx_line = []


class TestInterface(unittest.TestCase):

    # Instruction list

    def test_add_instruction_list(self):
        #Setup the test
        test_str = "commande 1 007"
        self.update_events()

        interface_pi.instructEntry.insert(END, test_str)
        interface_pi.butAdd.event_generate("<Button-1>",when="tail")
        self.update_events()

        # Test
        self.assertEqual(interface_pi.instructionListe[0]," commande 1 007")

        # Clear test
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

        # Clear test
        self.clear_entry(interface_pi.instructEntry)
        interface_pi.instructionListe.pop()

    #======= Motor buttons =======

    # Motor 1
    def test_leftKeyM1(self):
        # Setup the test
        self.update_events()
        #interface_pi.instructEntry.insert(END, test_str)
        interface_pi.left1Button.event_generate("<Button-1>", when="tail")
        self.update_events()

        # Test first increment
        self.assertEqual(interface_pi.listeMoteur1[0], -10,"Valeur de la commande du moteur 1 ne s'incrémente pas de -10")
        self.assertEqual(interface_pi.moteur1Entry.get(), '-10', "La valeur -10 du moteur 1 ne s'affiche pas dans son entry")
        self.update_events()

        # Test second increment
        interface_pi.left1Button.event_generate("<Button-1>", when="tail")
        self.update_events()
        self.assertEqual(interface_pi.listeMoteur1[0], -20,"Valeur de la commande du moteur 1 ne s'incrémente pas de -20")
        self.assertEqual(interface_pi.moteur1Entry.get(), '-20', "La valeur -20 du moteur 1 ne s'affiche pas dans son entry")
        self.update_events()

        # Clear test
        self.clear_entry(interface_pi.instructEntry)
        interface_pi.listeMoteur1[0] = 0
        interface_pi.moteur1Entry.delete(0,10)

    def test_RightKeyM1(self):
        # Setup the test
        self.update_events()
        #interface_pi.instructEntry.insert(END, test_str)
        interface_pi.right1Button.event_generate("<Button-1>", when="tail")
        self.update_events()

        # Test first increment
        self.assertEqual(interface_pi.listeMoteur1[0], 10,"Valeur de la commande du moteur 1 ne s'incrémente pas de 10")
        self.assertEqual(interface_pi.moteur1Entry.get(), '10', "La valeur 10 du moteur 1 ne s'affiche pas dans son entry")
        self.update_events()

        # Test second increment
        interface_pi.right1Button.event_generate("<Button-1>", when="tail")
        self.update_events()
        self.assertEqual(interface_pi.listeMoteur1[0], 20,"Valeur de la commande du moteur 1 ne s'incrémente pas de 20")
        self.assertEqual(interface_pi.moteur1Entry.get(), '20', "La valeur 20 du moteur 1 ne s'affiche pas dans son entry")
        self.update_events()

        # Clear test
        self.clear_entry(interface_pi.instructEntry)
        interface_pi.listeMoteur1[0] = 0
        interface_pi.moteur1Entry.delete(0,10)

    def test_leftKeyM1_automatic(self):
        # Setup the test
        self.update_events()
        interface_pi.auto.select()
        interface_pi.left1Button.event_generate("<Button-1>", when="tail")
        self.update_events()

        # Test first increment
        self.assertEqual(rx_line[-1], 'moveto 1 -10\n', "La commande envpoyé au OpenCr n'est pas  -10 pour le moteur 1")
        self.update_events()

        # Test second increment
        interface_pi.left1Button.event_generate("<Button-1>", when="tail")
        self.update_events()
        self.assertEqual(rx_line[-1], 'moveto 1 -20\n', "La commande envpoyé au OpenCr n'est pas  -20 pour le moteur 1")
        self.update_events()

        # Clear test
        self.clear_entry(interface_pi.instructEntry)
        interface_pi.listeMoteur1[0] = 0
        interface_pi.auto.deselect()
        self.update_events()
        interface_pi.moteur1Entry.delete(0,10)

    def test_RightKeyM1_automatic(self):
        # Setup the test
        self.update_events()
        interface_pi.auto.select()
        interface_pi.right1Button.event_generate("<Button-1>", when="tail")
        self.update_events()

        # Test first increment
        self.assertEqual(rx_line[-1], 'moveto 1 10\n', "La commande envpoyé au OpenCr n'est pas  10 pour le moteur 1")
        self.update_events()

        # Test second increment
        interface_pi.right1Button.event_generate("<Button-1>", when="tail")
        self.update_events()
        self.assertEqual(rx_line[-1], 'moveto 1 20\n', "La commande envpoyé au OpenCr n'est pas  20 pour le moteur 1")
        self.update_events()

        # Clear test
        self.clear_entry(interface_pi.instructEntry)
        interface_pi.listeMoteur1[0] = 0
        interface_pi.auto.deselect()
        self.update_events()
        interface_pi.moteur1Entry.delete(0,10)

    # Motor 2
    def test_leftKeyM2(self):
        # Setup the test
        self.update_events()
        interface_pi.left2Button.event_generate("<Button-1>", when="tail")
        self.update_events()

        # Test first increment
        self.assertEqual(interface_pi.listeMoteur2[0], -10,"Valeur de la commande du moteur 2 ne s'incrémente pas de -10")
        self.assertEqual(interface_pi.moteur2Entry.get(), '-10', "La valeur -10 du moteur 2 ne s'affiche pas dans son entry")
        self.update_events()

        # Test second increment
        interface_pi.left2Button.event_generate("<Button-1>", when="tail")
        self.update_events()
        self.assertEqual(interface_pi.listeMoteur2[0], -20,"Valeur de la commande du moteur 2 ne s'incrémente pas de -20")
        self.assertEqual(interface_pi.moteur2Entry.get(), '-20', "La valeur -20 du moteur 2 ne s'affiche pas dans son entry")
        self.update_events()

        # Clear test
        self.clear_entry(interface_pi.instructEntry)
        interface_pi.listeMoteur2[0] = 0
        interface_pi.moteur2Entry.delete(0,10)

    def test_RightKeyM2(self):
        # Setup the test
        self.update_events()
        interface_pi.right2Button.event_generate("<Button-1>", when="tail")
        self.update_events()

        # Test first increment
        self.assertEqual(interface_pi.listeMoteur2[0], 10,"Valeur de la commande du moteur 2 ne s'incrémente pas de -10")
        self.assertEqual(interface_pi.moteur2Entry.get(), '10', "La valeur 10 du moteur 2 ne s'affiche pas dans son entry")
        self.update_events()

        # Test second increment
        interface_pi.right2Button.event_generate("<Button-1>", when="tail")
        self.update_events()
        self.assertEqual(interface_pi.listeMoteur2[0], 20,"Valeur de la commande du moteur 2 ne s'incrémente pas de -20")
        self.assertEqual(interface_pi.moteur2Entry.get(), '20', "La valeur -20 du moteur 2 ne s'affiche pas dans son entry")
        self.update_events()

        # Clear test
        self.clear_entry(interface_pi.instructEntry)
        interface_pi.listeMoteur2[0] = 0
        interface_pi.moteur2Entry.delete(0,10)

    def test_leftKeyM2_automatic(self):
        # Setup the test
        self.update_events()
        interface_pi.auto.select()
        interface_pi.left2Button.event_generate("<Button-1>", when="tail")
        self.update_events()

        # Test first increment
        self.assertEqual(rx_line[-1], 'moveto 2 -10\n', "La commande envpoyé au OpenCr n'est pas  -10 pour le moteur 1")
        self.update_events()

        # Test second increment
        interface_pi.left2Button.event_generate("<Button-1>", when="tail")
        self.update_events()
        self.assertEqual(rx_line[-1], 'moveto 2 -20\n', "La commande envpoyé au OpenCr n'est pas  -20 pour le moteur 1")
        self.update_events()

        # Clear test
        self.clear_entry(interface_pi.instructEntry)
        interface_pi.listeMoteur1[0] = 0
        interface_pi.auto.deselect()
        interface_pi.moteur2Entry.delete(0,10)
        self.update_events()

    def test_RightKeyM2_automatic(self):
        # Setup the test
        self.update_events()
        interface_pi.auto.select()
        interface_pi.right2Button.event_generate("<Button-1>", when="tail")
        self.update_events()

        # Test first increment
        self.assertEqual(rx_line[-1], 'moveto 2 10\n', "La commande envpoyé au OpenCr n'est pas  10 pour le moteur 2")
        self.update_events()

        # Test second increment
        interface_pi.right2Button.event_generate("<Button-1>", when="tail")
        self.update_events()
        self.assertEqual(rx_line[-1], 'moveto 2 20\n', "La commande envpoyé au OpenCr n'est pas  20 pour le moteur 2")
        self.update_events()

        # Clear test
        self.clear_entry(interface_pi.instructEntry)
        interface_pi.listeMoteur2[0] = 0
        interface_pi.auto.deselect()
        self.update_events()
        interface_pi.moteur2Entry.delete(0,10)

    # Motor 3
    def test_leftKeyM3(self):
        # Setup the test
        self.update_events()
        interface_pi.left3Button.event_generate("<Button-1>", when="tail")
        self.update_events()

        # Test first increment
        self.assertEqual(interface_pi.listeMoteur3[0], -10,"Valeur de la commande du moteur 3 ne s'incrémente pas de -10")
        self.assertEqual(interface_pi.moteur3Entry.get(), '-10', "La valeur -10 du moteur 3 ne s'affiche pas dans son entry")
        self.update_events()

        # Test second increment
        interface_pi.left3Button.event_generate("<Button-1>", when="tail")
        self.update_events()
        self.assertEqual(interface_pi.listeMoteur3[0], -20,"Valeur de la commande du moteur 3 ne s'incrémente pas de -20")
        self.assertEqual(interface_pi.moteur3Entry.get(), '-20', "La valeur -20 du moteur 3 ne s'affiche pas dans son entry")
        self.update_events()

        # Clear test
        self.clear_entry(interface_pi.instructEntry)
        interface_pi.listeMoteur3[0] = 0
        interface_pi.moteur3Entry.delete(0,10)

    def test_RightKeyM3(self):
        # Setup the test
        self.update_events()
        interface_pi.right3Button.event_generate("<Button-1>", when="tail")
        self.update_events()

        # Test first increment
        self.assertEqual(interface_pi.listeMoteur3[0], 10,"Valeur de la commande du moteur 3 ne s'incrémente pas de 10")
        self.assertEqual(interface_pi.moteur3Entry.get(), '10', "La valeur 10 du moteur 3 ne s'affiche pas dans son entry")
        self.update_events()

        # Test second increment
        interface_pi.right3Button.event_generate("<Button-1>", when="tail")
        self.update_events()
        self.assertEqual(interface_pi.listeMoteur3[0], 20,"Valeur de la commande du moteur 3 ne s'incrémente pas de 20")
        self.assertEqual(interface_pi.moteur3Entry.get(), '20', "La valeur 20 du moteur 3 ne s'affiche pas dans son entry")
        self.update_events()

        # Clear test
        self.clear_entry(interface_pi.instructEntry)
        interface_pi.listeMoteur3[0] = 0
        interface_pi.moteur3Entry.delete(0,10)

    def test_leftKeyM3_automatic(self):
        # Setup the test
        self.update_events()
        interface_pi.auto.select()
        interface_pi.left3Button.event_generate("<Button-1>", when="tail")
        self.update_events()

        # Test first increment
        self.assertEqual(rx_line[-1], 'moveto 3 -10\n', "La commande envoyé au OpenCr n'est pas  -10 pour le moteur 3")
        self.update_events()

        # Test second increment
        interface_pi.left3Button.event_generate("<Button-1>", when="tail")
        self.update_events()
        self.assertEqual(rx_line[-1], 'moveto 3 -20\n', "La commande envpoyé au OpenCr n'est pas  -20 pour le moteur 3")
        self.update_events()

        # Clear test
        self.clear_entry(interface_pi.instructEntry)
        interface_pi.listeMoteur3[0] = 0
        interface_pi.auto.deselect()
        interface_pi.moteur3Entry.delete(0,10)
        self.update_events()

    def test_RightKeyM3_automatic(self):
        # Setup the test
        self.update_events()
        interface_pi.auto.select()
        interface_pi.right3Button.event_generate("<Button-1>", when="tail")
        self.update_events()

        # Test first increment
        self.assertEqual(rx_line[-1], 'moveto 3 10\n',
                         "La commande envpoyé au OpenCr n'est pas  10 pour le moteur 1")
        self.update_events()

        # Test second increment
        interface_pi.right3Button.event_generate("<Button-1>", when="tail")
        self.update_events()
        self.assertEqual(rx_line[-1], 'moveto 3 20\n',
                         "La commande envpoyé au OpenCr n'est pas  20 pour le moteur 1")
        self.update_events()

        # Clear test
        self.clear_entry(interface_pi.instructEntry)
        interface_pi.listeMoteur3[0] = 0
        interface_pi.auto.deselect()
        self.update_events()
        interface_pi.moteur3Entry.delete(0, 10)


    #=======  =======
    def update_events(self):
        interface_pi.root.update()
        interface_pi.root.update_idletasks()

    def clear_entry(self,entry):
        entry.delete(0, END)
        self.update_events()


def dummy_send( cmd_write):
    rx_line.append(cmd_write)
    #print("dummy_serial:" + rx_line[-1])

interface_pi.send = dummy_send

if __name__ == '__main__':
    interface_pi.send = dummy_send
    unittest.main()
    print("Dummy_serial:\n")

# instructEntry.insert(END, "hello_world")

# instructEntry.insert(END, "deuxieme")