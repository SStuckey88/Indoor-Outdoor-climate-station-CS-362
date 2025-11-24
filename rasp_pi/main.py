import threading
from threading import Thread
<<<<<<< HEAD
from gpiozero import LED, Button
from queue import Queue
=======
from nicegui import ui
from gpiozero import LED, Button
>>>>>>> ad49c6db0e78734bd1c8577686afef5d89eb7cd6

import backend
import display


redLed = LED(25)
greenLed = LED(5)
blueLed = LED(16)

redBtn = Button(4)
greenBtn = Button(23)

<<<<<<< HEAD
def backendThread(q):
    print("Starting Backend")
    backend.connect(q)
=======
def backendThread():
    print("Starting Backend")
    backend.connect()
>>>>>>> ad49c6db0e78734bd1c8577686afef5d89eb7cd6
    
def displayThread():
    print("Starting Frontend")
    display.startView()
    
if __name__ == '__main__':
<<<<<<< HEAD
    
    q = Queue()
    q.put("test")
    
    backThread = Thread(target = backendThread, args=(q,))
=======
    backThread = Thread(target = backendThread)
>>>>>>> ad49c6db0e78734bd1c8577686afef5d89eb7cd6
    #backThread.daemon = True
    backThread.start()
    #backThread.join()
    
    frontThread = Thread(target = displayThread)
    frontThread.daemon = True
    frontThread.start()
<<<<<<< HEAD
    frontThread.join()

    if not redBtn.is_pressed:
        q.put("Red")
    
=======
    frontThread.join()
>>>>>>> ad49c6db0e78734bd1c8577686afef5d89eb7cd6
