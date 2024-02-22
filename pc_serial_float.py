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
        bs = self.serialPort.readline()
        print(bs)
        # if self.serialPort.inWaiting() > 0 and self.messageComplete == False:
        #     x = self.serialPort.read().decode("utf-8") # decode needed for Python3
            
        #     if self.dataStarted == True:
        #         if x != self.endMarker:
        #             self.dataBuf = self.dataBuf + x
        #         else:
        #             self.dataStarted = False
        #             self.messageComplete = True
        #     elif x == self.startMarker:
        #         self.dataBuf = ''
        #         self.dataStarted = True
        
        # if (self.messageComplete == True):
        #     self.messageComplete = False
        #     return self.dataBuf
        # else:
        #     return "XXX" 

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
        print("length: ", len(dataWithMarkers))
        self.serialPort.write(dataWithMarkers) # encode needed for Python3
        print("sent !!!")

serial_node = serial_interface(9600, "/dev/ttyACM0")
count = 0
prevTime = time.time()
int32_array = array.array('i', [ num for num in range(22)])
float_array = array.array('f', [ float(num) for num in range(22)])
while True:

    reply_msg = serial_node.recvLikeArduino()
    
    if not (reply_msg == 'XXX'):
        print ("Time %s  Reply %s" %(time.time(), arduinoReply))    
        # send a message at intervals

    if time.time() - prevTime > 0.10:
        # serial_node.sendToArduino(int32_array)
        serial_node.sendToArduino(float_array)
        prevTime = time.time()
        count += 1
