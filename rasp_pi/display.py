import sqlite3
from nicegui import ui
from datetime import datetime

indoorTemp = 0
indoorHum = 0

indoorTempLabel = ui.label()
indoorHumLabel = ui.label()

x = []
tempY = []
humY = []    

def updateData():
    #Connect to database
    con = sqlite3.connect("climatedata.db")
    cur = con.cursor()
    cur.execute('SELECT * FROM climate ORDER BY timestamp DESC')
    rows = cur.fetchall()
    cur.close()
    
    indoorTemp = rows[0][1]
    indoorHum = rows[0][2]
    indoorTempLabel.set_text(f"Indoor Temperature: {indoorTemp}°C")
    indoorHumLabel.set_text(f"Indoor Humidity: {indoorHum}%")
    
    x = []
    tempY = []
    humY = []
    for row in reversed(rows):
        currDatetime = datetime.fromtimestamp(row[0])
        currTime = currDatetime.strftime("%I:%M")
        currDate = currDatetime.date()
        x.append(currTime)
        tempY.append(row[1])
        humY.append(row[2])
        
    tempChart.options['series'][0]['data'] = tempY
    tempChart.options['xAxis']['name'] = currDate
    tempChart.options['xAxis']['data'] = x
    tempChart.options['yAxis']['data'] = tempY
    
    humChart.options['series'][0]['data'] = humY
    humChart.options['xAxis']['name'] = currDate
    humChart.options['xAxis']['data'] = x
    humChart.options['yAxis']['data'] = humY

indoorTempLabel.set_text(f"Indoor Temperature: {indoorTemp}°")

indoorHumLabel.set_text(f"Indoor Humidity: {indoorHum}%")
    
tempChart = ui.echart({
    'xAxis': {'type': 'category', 'data': [], 'name':'Time', 'nameLocation':'center'},
    'yAxis': {'type': 'value', 'data': [], 'name':'Temperature', 'nameLocation':'center'},
    'series': [{'type': 'line', 'data': []}],
})

humChart = ui.echart({
    'xAxis': {'type': 'category', 'data': [], 'name':'Time', 'nameLocation':'center'},
    'yAxis': {'type': 'value', 'data': [], 'name':'Humidity', 'nameLocation':'center'},
    'series': [{'type': 'line', 'data': []}],
})

ui.timer(30.0, updateData)

ui.run()