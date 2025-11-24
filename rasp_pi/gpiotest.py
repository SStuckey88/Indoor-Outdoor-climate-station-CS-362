from gpiozero import LED, Button

redLed = LED(25)
greenLed = LED(5)
blueLed = LED(16)

redBtn = Button(4)
greenBtn = Button(23)

while True:
    if redBtn.is_pressed:
        redLed.off()
    else:
        redLed.on()
        
    if greenBtn.is_pressed:
        greenLed.off()
    else:
        greenLed.on()