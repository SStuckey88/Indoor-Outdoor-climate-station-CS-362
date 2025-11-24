import threading
from threading import Thread
from gpiozero import LED, Button
from queue import Queue
from nicegui import ui
from gpiozero import LED, Button

import backend
import display


redLed = LED(25)
greenLed = LED(5)
blueLed = LED(16)

redBtn = Button(4)
greenBtn = Button(23)

def backendThread(q):
    print("Starting Backend")
    backend.connect(q)
    
def displayThread():
    print("Starting Frontend")
    display.startView()
    
if __name__ == '__main__':
    
    q = Queue()
    q.put("test")
    
    backThread = Thread(target = backendThread, args=(q,))
    #backThread.daemon = True
    backThread.start()
    #backThread.join()
    
    frontThread = Thread(target = displayThread)
    frontThread.daemon = True
    frontThread.start()
    frontThread.join()

    if not redBtn.is_pressed:
        q.put("Red")
