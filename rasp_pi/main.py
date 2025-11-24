import threading
from threading import Thread
from nicegui import ui
from gpiozero import LED, Button

import backend
import display


redLed = LED(25)
greenLed = LED(5)
blueLed = LED(16)

redBtn = Button(4)
greenBtn = Button(23)

def backendThread():
    print("Starting Backend")
    backend.connect()
    
def displayThread():
    print("Starting Frontend")
    display.startView()
    
if __name__ == '__main__':
    backThread = Thread(target = backendThread)
    #backThread.daemon = True
    backThread.start()
    #backThread.join()
    
    frontThread = Thread(target = displayThread)
    frontThread.daemon = True
    frontThread.start()
    frontThread.join()