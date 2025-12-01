# Group: 44
# Authors: Bryan Paulis bpaul21@uic.edu Sam Stuckey sstuc2@uic.edu Trey Wayne jwayn3@uic.edu
# Indoor and Outdoor Climate Stations
# 
# Abstact:
# This group project will present microcontroller based weather stations which are designed to measure and display environmental parameters such as temperature, humidity, and pressure.
# The project is intended to provide accurate and continuous readings of local weather conditions depending on where the Arduinos are located (indoor and outdoor). 
# This demonstrates systems and devices we have been learning throughout the semester such as input/output processing, serial communication, networking, and data visualization when dealing with results. 
# This project will demonstrate our understanding of such topics we have been introduced to this semester.
#
# Info: pi that compiles data into a sql data base and displays it
#

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
