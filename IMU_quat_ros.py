from serial import Serial
import time
import struct
import array
import rospy
from nav_msgs.msg import Odometry
from scipy.spatial.transform import Rotation
import copy 
import numpy as np
class serial_interface():

    def __init__(self, baudRate, serialPortName):  
    
        self.startMarker = '<'
        self.endMarker = '>'
        self.dataStarted = False
        self.dataBuf = ""
        self.messageComplete = False
        self.serialPort = Serial(port= serialPortName, baudrate = baudRate, timeout=0, rtscts=True)
        print("Serial port " + serialPortName + " opened  Baudrate " + str(baudRate))
        self.yaw_offset = 0.0
        self.last_yaw = 0.0

        self.odom_pub = rospy.Publisher( "IMU", Odometry, queue_size=1000)
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
        if( len(byte_array) <2 ):
            return
        if(byte_array[0] !=60 or byte_array[-1] != 62):
            return
        byte_array = byte_array[1:-1]
        quat = array.array('f')
        quat.frombytes(byte_array)
        print("quat: ", quat)
        print("self.yaw_offset: ", self.yaw_offset)
        if(np.linalg.norm(quat) < 1):
            return
        
        rot = Rotation.from_quat(quat)
        eul = rot.as_euler('xyz', degrees=True)
        print(eul)
        calib_eul = copy.deepcopy(eul)
        diff = min( abs(eul[2] - self.last_yaw), abs(eul[2] - self.last_yaw + 360.0))
        if( diff > 30.0):
            self.yaw_offset = 130.0
            print("self.yaw_offset: ", self.yaw_offset)

        
        calib_eul[2] += self.yaw_offset

        rot = Rotation.from_euler('xyz', calib_eul, degrees=True)
        quat = rot.as_quat()

        odom=Odometry()
        odom.pose.pose.orientation.x=quat[0]
        odom.pose.pose.orientation.y=quat[1]
        odom.pose.pose.orientation.z=quat[2]
        odom.pose.pose.orientation.w=quat[3]
        
        odom.header.stamp=rospy.Time.now()
        odom.header.frame_id="map"
        self.odom_pub.publish(odom)
        self.last_yaw = eul[2]

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


if __name__ == '__main__':
    rospy.init_node('headtracking_Node', anonymous=True)   
    serial_node = serial_interface(9600, "/dev/ttyACM0")
    rate = rospy.Rate(10) # 10hz
    while not rospy.is_shutdown():
        start = time.time()
        # serial_node.sendToArduino(float_array)
        arduinoReply = serial_node.recvLikeArduino()
        end = time.time()
        print( (end - start)* 1000, "ms" )
        rate.sleep()


