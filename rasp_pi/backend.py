import sqlite3
import serial
from datetime import datetime
from queue import Queue
# CREATE TABLE indoor(timestamp REAL, temp INT, hum INT, airQuality INT);
# CREATE TABLE outdoor(timestamp REAL, temp INT, hum INT, press REAL, light INT);

SERIAL_PORT = '/dev/ttyACM0'
BAUD_RATE = 9600
TIMEOUT = 3

def insertIndoorData(data):
    
    con = sqlite3.connect("climatedata.db")
    cur = con.cursor()
    cur.execute(f'''INSERT INTO indoor 
                (timestamp, temp, hum, airQuality) 
                values({data['time']}, {data['inTemp']}, {data['inHum']}, {data['tvoc']})''')
    con.commit()
    con.close()
    
def insertOutdoorData(data):
    
    con = sqlite3.connect("climatedata.db")
    cur = con.cursor()
    cur.execute(f'''INSERT INTO outdoor
                (timestamp, temp, hum, light, press) 
                values({data['time']}, {data['temp']}, {data['hum']}, {data['press']}, {data['light']})''')
    con.commit()
    con.close()
    
def openSerialPort(portName, baudRate, timeout):
    try:
        ser = serial.Serial(portName, baudRate, timeout=timeout)
        if ser.is_open:
            print("Connected to Arduino!")
            return ser
        else:
            print("Failed to connect to Arduino!")
            return None
    except serial.SerialException as e:
        print("Serial Port Error!")
        return None

#def main():
def connect(q):
    ser = openSerialPort(SERIAL_PORT, BAUD_RATE, TIMEOUT)
    line = ""
    while True:
        if ser:
            try:
                if ser.in_waiting > 0:
                    line = ser.readline().decode('utf-8')
                    firstChar = line[0]
                    message = line[1:].replace(":", "").strip()
                    if(firstChar == 'T'):
                        timestamp = 'T' + str(datetime.now().timestamp())
                        ser.write(timestamp.encode())
                    elif(firstChar == 'D'):
                        data = splitData(message)
                        if data != None:
                            try:
                                insertIndoorData(data)
                            except:
                                continue
                        print(message)
                    elif(firstChar == 'O'):
                        data = splitData(message)
                        if data != None:
                            try:
                                insertOutdoorData(data)
                            except:
                                continue
                        print(message)
                    else:
                        print(line)
                    
            except serial.SerialException as e:
                print("Serial Error!")
        else:
            ser = openSerialPort(SERIAL_PORT, BAUD_RATE, TIMEOUT)
            
        if not q.empty():
            input = q.get()
            print(input)
            
def splitData(message):
    try:
        data = dict(item.split("=") for item in message.split(";"))
        return data
    except:
        return None