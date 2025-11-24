import sqlite3
from nicegui import ui
from datetime import datetime
from queue import Queue

latestSync = 0
indoorTemp = 0
indoorHum = 0
tvoc = 0

x = []
tempY = []
humY = []

@ui.page('/')
def page():

   
    indoorTemp = ui.label("Indoor Temperature")
    indoorTempLabel = ui.label().style('font-size: 1000%;')

    indoorHum = ui.label("Indoor Humidity")
    indoorHumLabel = ui.label().style('font-size: 500%;')

    tvocLabel = ui.label()

    latestSyncLabel = ui.label()

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

    def updateData():
        #Connect to database
        con = sqlite3.connect("climatedata.db")
        cur = con.cursor()
        cur.execute('SELECT * FROM climate ORDER BY timestamp DESC')
        rows = cur.fetchall()
        cur.close()
        
        latestSync = datetime.fromtimestamp(rows[0][0]).strftime("%I:%M")
        indoorTemp = rows[0][1]
        indoorHum = rows[0][2]
        tvoc = rows[0][3]
        latestSyncLabel.set_text(f"Latest Sync: {latestSync}")
        indoorTempLabel.set_text(f"{indoorTemp}°C")
        indoorHumLabel.set_text(f"{indoorHum}%")
        tvocLabel.set_text(f"TVOC: {tvoc} PPB")
        
        x = []
        tempY = []
        humY = []
        
        for row in reversed(rows):
            currDatetime = datetime.fromtimestamp(row[0])
            currTime = currDatetime.strftime("%I:%M")
            currDate = currDatetime.date()
<<<<<<< HEAD
            if(currDate == datetime.now().date()):
                x.append(currTime)
                tempY.append(row[1])
                humY.append(row[2])
=======
            x.append(currTime)
            tempY.append(row[1])
            humY.append(row[2])
>>>>>>> ad49c6db0e78734bd1c8577686afef5d89eb7cd6
            
        tempChart.options['series'][0]['data'] = tempY
        tempChart.options['xAxis']['name'] = currDate
        tempChart.options['xAxis']['data'] = x
        tempChart.options['yAxis']['data'] = tempY
        
        humChart.options['series'][0]['data'] = humY
        humChart.options['xAxis']['name'] = currDate
        humChart.options['xAxis']['data'] = x
        humChart.options['yAxis']['data'] = humY

    ui.timer(5.0, updateData)

    ui.query('body').style('background-image: linear-gradient(to bottom, #82ADDB, #EBB2B1); color: white; cursor: none;')
    

def startView():
    ui.run(reload=False)
    
if __name__ in {"__main__", "__mp_main__"}:
    ui.run()