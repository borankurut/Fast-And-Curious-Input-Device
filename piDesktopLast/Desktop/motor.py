import RPi.GPIO as GPIO
import time

GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)

IN1=20
IN2=21
ENA=18

GPIO.setup(IN1,GPIO.OUT)
GPIO.setup(IN2,GPIO.OUT)
GPIO.setup(ENA,GPIO.OUT)
GPIO.output(IN1,GPIO.LOW)
GPIO.output(IN2,GPIO.LOW)

p=GPIO.PWM(ENA,5000)
p.start(0)

def ileri(hiz):
    GPIO.output(IN1,GPIO.HIGH)
    GPIO.output(IN2,GPIO.LOW)
    p.ChangeDutyCycle(hiz)

def geri(hiz):
    GPIO.output(IN1,GPIO.LOW)
    GPIO.output(IN2,GPIO.HIGH)
    p.ChangeDutyCycle(hiz)

def dur():
    GPIO.output(IN1,GPIO.LOW)
    GPIO.output(IN2,GPIO.LOW)
    p.ChangeDutyCycle(0)

while True:
    hiz=input("Hiz degeri giriniz(0-100 arasi) \n")    
    yon=input("Donus yonu seciniz (İleri icin i Geri icin g Dur icin s) \n")
    

    if yon=='i':
        ileri(100)

    elif yon=='g':
        geri(100)

    elif yon=='s':
        dur()
    
    else:
        print("Hatali komut girisi!")
        GPIO.cleanup()
        break
