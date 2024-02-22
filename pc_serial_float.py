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

        if self.serialPort.in_waiting > 0:
            # Read the byte array
            # byte_array = self.serialPort.read(88)
            byte_array = self.serialPort.read(self.serialPort.in_waiting)
            print("Received:", len(byte_array))

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
        # print("length: ", len(dataWithMarkers))
        self.serialPort.write(dataWithMarkers) # encode needed for Python3
        # print("sent !!!")

serial_node = serial_interface(9600, "/dev/ttyACM0")
count = 0
prevTime = time.time()
int32_array = array.array('i', [ num for num in range(22)])
float_array = array.array('f', [ float(num) for num in range(22)])

start = time.time()
end = time.time()


while True:

    arduinoReply = serial_node.recvLikeArduino()
    end = time.time()
    print( (end - start)* 1000, "ms" )
    # if not (arduinoReply == 'XXX'):
    #     print ("Time %s  Reply %s" %(time.time(), arduinoReply))    
    #     # send a message at intervals

    serial_node.sendToArduino(float_array)
    start = time.time()
    prevTime = start


# while True:
#     start = time.time()
#     ser.write(bytearray(struct.pack("f", value)))
#     bs = ser.readline()
#     end = time.time()
#     print( (end - start)* 1000, "ms" )
#     print(bs)
