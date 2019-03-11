import unittest
import sys
from tkinter import Tk, Button, Label, Entry, W, E, Checkbutton, BooleanVar, END, INSERT, Text

#sys.path.append('../')
import interface_pi

rx_line = []


class TestInterface(unittest.TestCase):

    #======= Motor buttons =======

    # ****** Motor 1 ******
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

        interface_pi.listeMoteur1[0] = 0
        interface_pi.auto.deselect()
        self.update_events()
        interface_pi.moteur1Entry.delete(0,10)

    # ****** Motor 2 ******
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

        interface_pi.listeMoteur2[0] = 0
        interface_pi.auto.deselect()
        self.update_events()
        interface_pi.moteur2Entry.delete(0,10)

    # ****** Motor 3 ******
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

        interface_pi.listeMoteur3[0] = 0
        interface_pi.auto.deselect()
        self.update_events()
        interface_pi.moteur3Entry.delete(0, 10)

    # ****** other motors button ******
    def test_automatic(self):
        # Setup the test
        self.update_events()

        # First test:
        self.assertFalse(interface_pi.automatic(), "Automatic send est coché alors qu'il ne devrait pas l'être")

        # Second test:
        interface_pi.auto.select()
        self.update_events()
        self.assertTrue(interface_pi.automatic(), "Automatic send n'est pas coché alors qu'il devrait l'être")

        # Clear test
        interface_pi.auto.deselect()
        self.update_events()

    def test_sendAngle(self):
        # Setup the test
        self.update_events()
        interface_pi.moteur1Entry.delete(0,10)
        interface_pi.moteur2Entry.delete(0,10)
        interface_pi.moteur3Entry.delete(0,10)

        interface_pi.moteur1Entry.insert(0, 100)
        interface_pi.moteur2Entry.insert(0, 200)
        interface_pi.moteur3Entry.insert(0, 300)
        self.update_events()
        interface_pi.sendButton.event_generate("<Button-1>", when = "tail")
        self.update_events()

        # First test : verify the update of the listeMoteur values
        self.assertEqual(interface_pi.listeMoteur1[0], 100,"La valeur d'angle du moteur 1 dans la list des moteur ne vas pas à 100")
        self.assertEqual(interface_pi.listeMoteur2[0], 200,"La valeur d'angle du moteur 2 dans la list des moteur ne vas pas à 200")
        self.assertEqual(interface_pi.listeMoteur3[0], 300,"La valeur d'angle du moteur 3 dans la list des moteur ne vas pas à 300")
        self.update_events()

        # Second test: verify the msg send to the OpenCr
        self.assertEqual(rx_line[-3], "moveto 1 100\n","La mauvaise commande est envoyée au moteur 1")
        self.assertEqual(rx_line[-2], "moveto 2 200\n","La mauvaise commande est envoyée au moteur 2")
        self.assertEqual(rx_line[-1], "moveto 3 300\n","La mauvaise commande est envoyée au moteur 3")

        # Clear test

        interface_pi.listeMoteur1[0] = 0
        interface_pi.listeMoteur2[0] = 0
        interface_pi.listeMoteur3[0] = 0

        interface_pi.moteur1Entry.delete(0,10)
        interface_pi.moteur2Entry.delete(0,10)
        interface_pi.moteur3Entry.delete(0,10)

        interface_pi.moteur1Entry.insert(0, 0)
        interface_pi.moteur2Entry.insert(0, 0)
        interface_pi.moteur3Entry.insert(0, 0)
        self.update_events()

    def test_reset(self):
        # Setup the test
        self.update_events()
        interface_pi.moteur1Entry.delete(0, 10)
        interface_pi.moteur2Entry.delete(0, 10)
        interface_pi.moteur3Entry.delete(0, 10)

        interface_pi.moteur1Entry.insert(0, 100)
        interface_pi.moteur2Entry.insert(0, 200)
        interface_pi.moteur3Entry.insert(0, 300)
        self.update_events()
        interface_pi.sendButton.event_generate("<Button-1>", when="tail")
        self.update_events()
        interface_pi.resetButton.event_generate("<Button-1>", when="tail")
        self.update_events()

        # First test : verify if all values the angles values of the motors returned to 0
        self.assertEqual(interface_pi.listeMoteur1[0], 0)
        self.assertEqual(interface_pi.listeMoteur2[0], 0)
        self.assertEqual(interface_pi.listeMoteur3[0], 0)

        self.assertEqual(interface_pi.moteur1Entry.get(), '0')
        self.assertEqual(interface_pi.moteur2Entry.get(), '0')
        self.assertEqual(interface_pi.moteur3Entry.get(), '0')

        # Clear test
        self.update_events()


    # ======= Instruction list =======

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
        self.assertEqual(interface_pi.routine.get('1.0',END),"x commande 1 008\n\n")

        # Clear test
        self.clear_entry(interface_pi.instructEntry)
        interface_pi.instructionListe.pop()

    def test_up(self):
        #Setup the test
        test_str_1 = " commande 1 008\n"
        test_str_2 = " stop_cmd 5 701\n"
        self.update_events()
        interface_pi.routine.delete("1.0", END)
        interface_pi.instructionListe = [test_str_1, test_str_2]

        interface_pi.routine.insert("1.0", interface_pi.instructionListe[0])
        self.update_events()
        interface_pi.routine.insert("2.0", "x" + interface_pi.instructionListe[1])
        interface_pi.command += 1
        self.update_events()


        interface_pi.butUp.event_generate("<Button-1>",when="tail")
        self.update_events()

        # First test: Verify if the x move upward
        self.assertEqual( interface_pi.routine.get('1.0', '2.0'), "x commande 1 008\n")
        self.assertEqual(interface_pi.routine.get('2.0', END), " stop_cmd 5 701\n\n")

        #Second test: Verify that the x could not go higher than first position
        interface_pi.butUp.event_generate("<Button-1>",when="tail")
        self.update_events()
        self.assertEqual( interface_pi.routine.get('1.0', '2.0'), "x commande 1 008\n")
        self.assertEqual(interface_pi.routine.get('2.0', END), " stop_cmd 5 701\n\n")


        # Clear test
        interface_pi.instructionListe.pop()
        interface_pi.instructionListe.pop()
        self.clear_text(interface_pi.routine)
        interface_pi.command = 1
        self.update_events()

    def test_down(self):
        #Setup the test
        test_str_1 = " commande 1 008\n"
        test_str_2 = " stop_cmd 5 701\n"
        self.update_events()
        interface_pi.routine.delete("1.0", END)
        interface_pi.instructionListe = [test_str_1, test_str_2]

        interface_pi.routine.insert("1.0", interface_pi.instructionListe[0])
        self.update_events()
        interface_pi.routine.insert("2.0", "x" + interface_pi.instructionListe[1])
        interface_pi.command += 1
        self.update_events()


        interface_pi.butUp.event_generate("<Button-1>",when="tail")
        self.update_events()

        # First test: Verify if the x move upward
        self.assertEqual( interface_pi.routine.get('1.0', '2.0'), "x commande 1 008\n")
        self.assertEqual(interface_pi.routine.get('2.0', END), " stop_cmd 5 701\n\n")

        #Second test: Verify that the x could not go higher than first position
        interface_pi.butUp.event_generate("<Button-1>",when="tail")
        self.update_events()
        self.assertEqual( interface_pi.routine.get('1.0', '2.0'), "x commande 1 008\n")
        self.assertEqual(interface_pi.routine.get('2.0', END), " stop_cmd 5 701\n\n")


        # Clear test
        interface_pi.instructionListe.pop()
        interface_pi.instructionListe.pop()
        self.clear_text(interface_pi.routine)
        self.update_events()


    #======= Miscellaneous =======
    def update_events(self):
        interface_pi.root.update()
        interface_pi.root.update_idletasks()

    def clear_entry(self,entry):
        entry.delete(0, END)
        self.update_events()

    def clear_text(self,text):
        text.delete("1.0", END)
        self.update_events()


def dummy_send( cmd_write):
    rx_line.append(cmd_write)
    #print("dummy_serial:" + rx_line[-1])

#Set the send function to send data to an array instead of serial port
interface_pi.send = dummy_send

if __name__ == '__main__':
    unittest.main()
    print("Dummy_serial:\n")