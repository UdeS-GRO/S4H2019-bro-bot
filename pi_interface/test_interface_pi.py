"""
 This Source Code Form is subject to the terms of the Mozilla Public
 License, v. 2.0. If a copy of the MPL was not distributed with this
 file, You can obtain one at http://mozilla.org/MPL/2.0/.
"""

import unittest
import sys
from tkinter import Tk, Button, Label, Entry, W, E, Checkbutton, BooleanVar, END, INSERT, Text
from threading import Thread,Condition,RLock

#sys.path.append('../')
import interface_pi

rx_line = []
stop_thread =Condition()


class TestInterface(unittest.TestCase):

    def setUp(self):

        while len(interface_pi.instructionListe) > 0:
            interface_pi.instructionListe.pop()

        self.clear_text(interface_pi.routine)
        self.clear_entry(interface_pi.instructEntry)
        self.update_events()

        interface_pi.routine.insert(INSERT, "x")
        self.update_events()

        interface_pi.listeMoteur1[0] = 0
        interface_pi.listeMoteur2[0] = 0
        interface_pi.listeMoteur3[0] = 0

        interface_pi.moteur1Entry.delete(0, 10)
        interface_pi.moteur2Entry.delete(0, 10)
        interface_pi.moteur3Entry.delete(0, 10)
        self.update_events()

        interface_pi.moteur1Entry.insert(0, 0)
        interface_pi.moteur2Entry.insert(0, 0)
        interface_pi.moteur3Entry.insert(0, 0)
        interface_pi.auto.deselect()
        interface_pi.loopRout.deselect()
        self.update_events()

        interface_pi.play = old_play


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


    # ======= Instruction list =======

    def test_add_instruction_list(self):
        #Setup the test
        test_str = "commande 1 007"
        self.update_events()

        interface_pi.instructEntry.insert(END, test_str)
        interface_pi.butAdd.event_generate("<Button-1>",when="tail")
        self.update_events()

        # Test
        self.assertEqual(interface_pi.instructionListe[0]," commande 1 007\n")

    def test_add_routine_list(self):
        #Setup the test
        test_str = "commande 1 008"
        self.update_events()
        interface_pi.instructEntry.insert(END, test_str)
        interface_pi.butAdd.event_generate("<Button-1>",when="tail")
        self.update_events()

        # Test
        self.assertEqual(interface_pi.routine.get('1.0',END),"x commande 1 008\n\n")

    def test_up(self):
        # Clear test
        self.update_events()

        #Setup the test
        test_str_1 = " commande 1 008\n"
        test_str_2 = " stop_cmd 5 701\n"
        self.update_events()
        interface_pi.routine.delete("1.0", END)
        interface_pi.instructionListe = [test_str_1, test_str_2]

        interface_pi.routine.insert("1.0", interface_pi.instructionListe[0])
        self.update_events()
        interface_pi.routine.insert("2.0", "x" + interface_pi.instructionListe[1])
        interface_pi.command = 2
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

    def test_down(self):

        #Setup the test
        test_str_1 = " commande h 450\n"
        test_str_2 = " start_cmd 3 819\n"
        self.update_events()
        interface_pi.routine.delete("1.0", END)
        interface_pi.instructionListe = [test_str_1, test_str_2]

        interface_pi.routine.insert("1.0", "x" + interface_pi.instructionListe[0])
        self.update_events()
        interface_pi.routine.insert("2.0", interface_pi.instructionListe[1])
        interface_pi.command = 1
        self.update_events()


        interface_pi.butDown.event_generate("<Button-1>",when="tail")
        self.update_events()

        # First test: Verify if the x move downward
        self.assertEqual( interface_pi.routine.get('1.0', '2.0'), " commande h 450\n")
        self.assertEqual(interface_pi.routine.get('2.0', END), "x start_cmd 3 819\n\n")

        #Second test: Verify that the x could not go lower than the last position
        interface_pi.butDown.event_generate("<Button-1>",when="tail")
        self.update_events()
        self.assertEqual( interface_pi.routine.get('1.0', '2.0'), " commande h 450\n")
        self.assertEqual(interface_pi.routine.get('2.0', END), "x start_cmd 3 819\n\n")

    def test_delete(self):
        # Setup the test
        test_str_1 = " commande h 798\n"
        test_str_2 = " start_cmd 3 125\n"
        test_str_3 = " gazon t 112\n"

        self.update_events()
        interface_pi.instructionListe = [test_str_1, test_str_2, test_str_3]

        interface_pi.routine.insert("1.0", interface_pi.instructionListe[0])

        interface_pi.routine.insert("2.0", "x" + interface_pi.instructionListe[1])
        interface_pi.command = 2

        interface_pi.routine.insert("3.0", interface_pi.instructionListe[2])
        self.update_events()

        interface_pi.butDelete.event_generate("<Button-1>", when="tail")
        self.update_events()

        # First test: Verify if the second command has been removed
        self.assertEqual(interface_pi.routine.get('1.0', '2.0'), " commande h 798\n")
        self.assertEqual(interface_pi.routine.get('2.0', END), "x gazon t 112\n\n")

        # Second test: Verify if the last command has been removed
        interface_pi.butDelete.event_generate("<Button-1>", when="tail")
        self.update_events()
        self.assertEqual(interface_pi.routine.get('1.0', END), "x commande h 798\n\n")

    def test_replace(self):
        # Setup the test
        test_str_1 = " commande o 111\n"
        test_str_2 = " start_cmd 9 222\n"
        test_str_3 = " gazon t 452\n"
        test_str_4 = "new_cmd 1"

        self.update_events()
        interface_pi.routine.delete("1.0", END)
        interface_pi.instructionListe = [test_str_1, test_str_2, test_str_3]

        interface_pi.routine.insert("1.0", interface_pi.instructionListe[0])

        interface_pi.routine.insert("2.0", "x" + interface_pi.instructionListe[1])
        interface_pi.command = 2

        interface_pi.routine.insert("3.0", interface_pi.instructionListe[2])

        interface_pi.instructEntry.insert(END, test_str_4)
        self.update_events()

        interface_pi.butReplace.event_generate("<Button-1>", when="tail")
        self.update_events()

        # First test: Verify if the second command has been replaced
        self.assertEqual(interface_pi.routine.get('1.0', '2.0'), " commande o 111\n")
        self.assertEqual(interface_pi.routine.get('2.0', '3.0'), "x new_cmd 1\n")
        self.assertEqual(interface_pi.routine.get('3.0', END), " gazon t 452\n\n")

    def test_execute(self):
        # Setup the test
        test_str_1 = " moveto 1 100\n"
        test_str_2 = " moveto 2 90\n"

        self.update_events()
        interface_pi.routine.delete("1.0", END)
        interface_pi.instructionListe = [test_str_1, test_str_2]

        interface_pi.routine.insert("1.0", interface_pi.instructionListe[0])

        interface_pi.routine.insert("2.0", "x" + interface_pi.instructionListe[1])
        interface_pi.command = 2

        self.update_events()

        interface_pi.butExecute.event_generate("<Button-1>", when="tail")
        self.update_events()

        # First test: Verify if the command has been send
        self.assertEqual(rx_line[-1], "moveto 2 90\n")

    def test_play_thread(self):
        #Setup the test

        interface_pi.play = dummy_play

        #First test: verify that the thread is active
        interface_pi.butRun.event_generate("<Button-1>", when="tail")
        self.update_events()
        self.assertTrue(interface_pi.play_thread.t_play.isAlive())

        #Second test: verify thath the thread is inactive
        with stop_thread:
            stop_thread.notify_all()
        interface_pi.play_thread.t_play.join(1)
        self.assertFalse(interface_pi.play_thread.t_play.isAlive())

    def test_play(self):
        # Setup the test
        test_str_1 = " commande h 987\n"
        test_str_2 = " start_cmd 3 654\n"
        test_str_3 = " gazon t 321\n"

        old_rx_line_len = len(rx_line)

        self.update_events()
        interface_pi.instructionListe = [test_str_1, test_str_2, test_str_3]

        interface_pi.routine.insert("1.0", "x" + interface_pi.instructionListe[0])
        interface_pi.command = 1

        interface_pi.routine.insert("2.0", interface_pi.instructionListe[1])

        interface_pi.routine.insert("3.0", interface_pi.instructionListe[2])

        self.update_events()

        # This tests need to be run in another thread because the Tkinter cannot be in another thread itself
        t_test = Thread(target=self.t_thread_test_play_func, args=(interface_pi.routine_event_cv, 100000000))
        t_test.start()

        # Run
        interface_pi.play()
        t_test.join()

    def loop_routine(self):
        # Setup the test
        test_str_1 = " commande h 987\n"
        test_str_2 = " start_cmd 3 654\n"
        test_str_3 = " gazon t 321\n"
        interface_pi.loopRout.select()
        self.update_events()

        interface_pi.instructionListe = [test_str_1, test_str_2, test_str_3]

        interface_pi.routine.insert("1.0", "x" + interface_pi.instructionListe[0])
        interface_pi.command = 1

        interface_pi.routine.insert("2.0", interface_pi.instructionListe[1])

        interface_pi.routine.insert("3.0", interface_pi.instructionListe[2])

        self.update_events()
        print("rendu ici 1" )
        # ** First sequence **
        old_rx_line_len = len(rx_line)
        # This tests need to be run in another thread because the Tkinter cannot be in another thread itself
        t_test = Thread(target=self.t_thread_test_play_func, args=(interface_pi.routine_event_cv, 100000000))
        t_test.start()

        # Run
        interface_pi.play()
        t_test.join()

        print("rendu ici 2")
        # ** Second sequence **
        old_rx_line_len = len(rx_line)
        # This tests need to be run in another thread because the Tkinter cannot be in another thread itself
        t_test = Thread(target=self.t_thread_test_play_func, args=(interface_pi.routine_event_cv, 100000000))
        t_test.start()

        # Run
        t_test.join()
        print("rendu ici 3")

        # ** Uncheck the loop : first sequence**
        interface_pi.loopRout.deselect()
        self.update_events()
        print("rendu ici 4")
        old_rx_line_len = len(rx_line)
        # This tests need to be run in another thread because the Tkinter cannot be in another thread itself
        t_test = Thread(target=self.t_thread_test_play_func, args=(interface_pi.routine_event_cv, 100000000))
        t_test.start()

        # Run
        t_test.join()

        # ** Uncheck the loop : should be no called sequence**
        self.assertFalse(TestInterface.wait_until_event_variable_to_set(event_var, 100000000))






    def t_thread_test_play_func(self, event_var, tick_timeout=1000):
        print("EVENNNNT TVAR :" + str(event_var.isSet()))
        # First test
        self.assertTrue(TestInterface.wait_until_event_variable_to_set(event_var, tick_timeout))
        self.assertEqual(rx_line[-1], "commande h 987\n")

        # Second Test
       # with interface_pi.routine_event_cv:
        event_var.set()

        self.assertTrue(TestInterface.wait_until_event_variable_to_set(event_var, tick_timeout))
        self.assertEqual(rx_line[-1], "start_cmd 3 654\n")

        # Third Test
        #with interface_pi.routine_event_cv:
        event_var.set()

        self.assertTrue(TestInterface.wait_until_event_variable_to_set(event_var, tick_timeout))
        self.assertEqual(rx_line[-1], "gazon t 321\n")

        #with interface_pi.routine_event_cv:
        event_var.set()

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

    def wait_until_len_change(old_value, list , tick_timeout=1000):
        counter = 0
        #for counter in range(0, tick_timeout):
        while 1:
            if len(list) != old_value:
                return True
                print("recu")
        return False

    def wait_until_event_variable_to_set(event_variable, tick_timeout=1000):
        counter = 0
        #for counter in range(0, tick_timeout):
        while 1:
            if not event_variable.isSet():
                print("recu")
                return True
        return False




def dummy_send( cmd_write):
    if cmd_write[0] == " ":
        cmd_write = cmd_write[1:]
    else:
        pass
    rx_line.append(cmd_write)
    print(cmd_write)

def dummy_play():
    with stop_thread:
        stop_thread.wait()

old_play = interface_pi.play

#Set the send function to send data to an array instead of serial port
interface_pi.send = dummy_send

if __name__ == '__main__':
    unittest.main()
    print("Dummy_serial:\n")