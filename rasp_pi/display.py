import sqlite3
from nicegui import ui
from datetime import datetime

latestSync = 0
indoorTemp = 0
indoorHum = 0
pressure = 0
tvoc = 0

latestSyncLabel = ui.label()
indoorTempLabel = ui.label()
indoorHumLabel = ui.label()
pressureLabel = ui.label()
tvocLabel = ui.label()

x = []
tempY = []
humY = []
presY = []

def updateData():
    #Connect to database
    con = sqlite3.connect("climatedata.db")
    cur = con.cursor()
    cur.execute('SELECT * FROM climate ORDER BY timestamp DESC')
    rows = cur.fetchall()
    cur.close()
    
    latestSync = rows[0][0]
    indoorTemp = rows[0][1]
    indoorHum = rows[0][2]
    pressure = rows[0][3]
    tvoc = rows[0][4]
    latestSyncLabel.set_text(f"Latest Sync: {latestSync}")
    indoorTempLabel.set_text(f"Indoor Temperature: {indoorTemp}°C")
    indoorHumLabel.set_text(f"Indoor Humidity: {indoorHum}%")
    pressureLabel.set_text(f"Bar. Pressure: {pressure} Pa")
    tvocLabel.set_text(f"TVOC: {tvoc} PPB")
    
    x = []
    tempY = []
    humY = []
    presY = [] 
    
    for row in reversed(rows):
        currDatetime = datetime.fromtimestamp(row[0])
        currTime = currDatetime.strftime("%I:%M")
        currDate = currDatetime.date()
        x.append(currTime)
        tempY.append(row[1])
        humY.append(row[2])
        presY.append(row[3])
        
    tempChart.options['series'][0]['data'] = tempY
    tempChart.options['xAxis']['name'] = currDate
    tempChart.options['xAxis']['data'] = x
    tempChart.options['yAxis']['data'] = tempY
    
    humChart.options['series'][0]['data'] = humY
    humChart.options['xAxis']['name'] = currDate
    humChart.options['xAxis']['data'] = x
    humChart.options['yAxis']['data'] = humY
    
    presChart.options['series'][0]['data'] = presY
    presChart.options['xAxis']['name'] = currDate
    presChart.options['xAxis']['data'] = x
    presChart.options['yAxis']['data'] = presY
    
latestSyncLabel.set_text(f"Latest Sync: {latestSync}")
indoorTempLabel.set_text(f"Indoor Temperature: {indoorTemp}°")
indoorHumLabel.set_text(f"Indoor Humidity: {indoorHum}%")
pressureLabel.set_text(f"Bar. Pressure: {pressure} Pa")
tvocLabel.set_text(f"TVOC: {tvoc} PPM")
    
tempChart = ui.echart({
    'xAxis': {'type': 'category', 'data': [], 'name':'Time', 'nameLocation':'center'},
    'yAxis': {'type': 'value', 'data': [], 'name':'Temperature', 'nameLocation':'center'},
    'series': [{'type': 'line', 'data': []}],
}, theme={
    'color': ['#b687ac', '#28738a', '#a78f8f'],
    'backgroundColor': 'rgba(254,248,239,1)',
})

humChart = ui.echart({
    'xAxis': {'type': 'category', 'data': [], 'name':'Time', 'nameLocation':'center'},
    'yAxis': {'type': 'value', 'data': [], 'name':'Humidity', 'nameLocation':'center'},
    'series': [{'type': 'line', 'data': []}],
})

presChart = ui.echart({
    'xAxis': {'type': 'category', 'data': [], 'name':'Time', 'nameLocation':'center'},
    'yAxis': {'type': 'value', 'data': [], 'name':'Barometric Pressure', 'nameLocation':'center'},
    'series': [{'type': 'line', 'data': []}],
})

ui.timer(5.0, updateData)

ui.query('body').style('background-image: linear-gradient(to bottom, #3A3A52, #020111); color: white;')

ui.run()