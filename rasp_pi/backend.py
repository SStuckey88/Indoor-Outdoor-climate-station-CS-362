import sqlite3
import serial
from datetime import datetime
from time import sleep
#table: climate
#CREATE TABLE climate(timestamp REAL, inTemp INT, inHum INT, inAirQuality INT, outTemp INT, outHum INT, outpressure REAL);

SERIAL_PORT = '/dev/ttyACM0'
BAUD_RATE = 9600
TIMEOUT = 1

def insertClimateData(data):
    
    con = sqlite3.connect("climatedata.db")
    cur = con.cursor()
    cur.execute(f'''INSERT INTO climate 
                (timestamp, inTemp, inHum, inAirQuality) 
                values({data['time']}, {data['inTemp']}, {data['inHum']}, {data['tvoc']})''')
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
def connect():
    ser = openSerialPort(SERIAL_PORT, BAUD_RATE, TIMEOUT)
    
    while True:
        if ser:
            try:
                if ser.in_waiting > 0:
                    line = ser.readline().decode('utf-8').rstrip()
                    firstChar = line[0]
                    message = line[1:]
                    if(firstChar == 'T'):
                        timestamp = 'T' + str(datetime.now().timestamp())
                        ser.write(timestamp.encode())
                        print(message)
                    elif(firstChar == 'D'):
                        data = dict(item.split("=") for item in message.split(";"))
                        insertClimateData(data)
                        print(message)
                    elif(firstChar == 'J'):
                        print(message)
                    else:
                        print(message)
            except serial.SerialException as e:
                print("Serial Error!")
        else:
            ser = openSerialPort(SERIAL_PORT, BAUD_RATE, TIMEOUT)

# if __name__ == '__main__':
#     main()