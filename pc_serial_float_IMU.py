from serial import Serial
import time
import struct
import array

class serial_interface():

    def __init__(self, baudRate, serialPortName):  
    
        self.startMarker = '<'
        self.endMarker = '>'
        self.dataStarted = False
        self.dataBuf = ""
        self.messageComplete = False
        self.serialPort = Serial(port= serialPortName, baudrate = baudRate, timeout=0, rtscts=True)
        print("Serial port " + serialPortName + " opened  Baudrate " + str(baudRate))
        # self.waitForArduino()

    def recvLikeArduino( self ):

        while( self.serialPort.inWaiting() == 0):
            continue

        byte_array = self.serialPort.read(self.serialPort.inWaiting())
        byte_array = bytearray(byte_array)
        print(byte_array)
        print(len(byte_array))
        # print(byte_array[0])
        # print(byte_array[-1])
        if(byte_array[0] !=60 or byte_array[-1] != 62):
            return
        byte_string = byte_array.decode("utf-8")
        quat = []
        # todo send and recieve as float
        for i in range(4):
            quat.append(float( byte_string[4*i+1: 4*i+5]))
        print(quat)

    def waitForArduino( self ):

        # wait until the Arduino sends 'Arduino is ready' - allows time for Arduino reset
        # it also ensures that any bytes left over from a previous message are discarded
        print("Waiting for Arduino to reset")
        
        msg = ""
        while msg.find("Arduino is ready") == -1:
            msg = self.recvLikeArduino()
            if not (msg == 'XXX'): 
                print(msg)

    def sendToArduino(self, dataToSend):
        dataWithMarkers = bytearray(self.startMarker, "utf-8")
        dataWithMarkers += bytearray(dataToSend)
        dataWithMarkers += bytearray(self.endMarker, "utf-8")
        self.serialPort.write(dataWithMarkers) # encode needed for Python3
        # print("sent !!!")

serial_node = serial_interface(9600, "/dev/ttyACM0")
count = 0
prevTime = time.time()
int32_array = array.array('i', [ num for num in range(22)])
float_array = array.array('f', [ float(num+2) for num in range(22)])

start = time.time()
end = time.time()


while True:
    start = time.time()
    # serial_node.sendToArduino(float_array)
    arduinoReply = serial_node.recvLikeArduino()
    end = time.time()
    print( (end - start)* 1000, "ms" )


