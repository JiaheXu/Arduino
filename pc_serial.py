from serial import Serial
import time


serPort = "/dev/ttyACM0"
# serPort = "/dev/ttyUSB0"
ser = Serial(serPort, 9600)
   
# Send character 'S' to start the program
# ser.write(bytearray('S','ascii'))

# Read line   
while True:
    start = time.time()
    ser.write(bytearray('S','ascii'))
    bs = ser.readline()
    end = time.time()
    print( (end - start)* 1000, "ms" )
    print(bs)
