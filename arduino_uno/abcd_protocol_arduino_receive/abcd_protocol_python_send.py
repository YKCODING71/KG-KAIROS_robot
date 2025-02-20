import serial 
import time 
import random

seq = serial.Serial(
            baudrate=115200,
            parity=serial.PARITY_NONE,
            stopbits=serial.STOPBITS_ONE,
            bytesize=serial.EIGHTBITS,
            timeout=1
        )
seq.port = "COM12"
a = 0
b = 0
c = 0
d = 0

seq.open()

while True:
    data = 'a'
    data += str(random.randint(0, 100))
    data += 'b'
    data += str(random.randint(0, 100))
    data += 'c'
    data += str(random.randint(0, 100))
    data += 'd'
    data += str(random.randint(0, 100))
    data += 'e'
    seq.write(data.encode())

    time.sleep(0.5)