import sqlite3
from gpiozero import LED
import serial
from datetime import datetime

#Connect to Arduino over Serial
ser = serial.Serial('/dev/ttyACM0', 9600, timeout=1)

#table: climate
#columns: timestamp, indoorTemp, indoorHumidity, indoorAirQuality, outdoorTemp, outdoorHumidity, outdoorPressure

def insertClimateData(data):
    timestamp = datetime.now().timestamp()
    con = sqlite3.connect("climatedata.db")
    cur = con.cursor()
    cur.execute(f'''INSERT INTO climate 
                (timestamp, indoorTemp, indoorHum) 
                values({timestamp}, {data['temp']}, {data['hum']})''')
    con.commit()
    con.close()
    
def printDB():
    con = sqlite3.connect("climatedata.db")
    cur = con.cursor()
    query = 'SELECT * FROM climate'
    cur.execute(query)
    rows = cur.fetchall()
    print(rows)
    con.close()
    
def main():
    while True:
        if ser.in_waiting > 0:
            line = ser.readline().decode('utf-8').rstrip()
            data = dict(item.split("=") for item in line.split(";"))
            insertClimateData(data)
            #printDB()

if __name__ == '__main__':
    main()