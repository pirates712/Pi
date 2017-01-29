import socket
import struct
import time
from input import Input


class piCmd(object):
    __slots__ = [ 'lMotorVal', 'rMotorVal', 'lMotorBrake', 'rMotorBrake' ]
    def getVals(self):
        return (self.lMotorVal, self.rMotorVal, self.lMotorBrake, self.rMotorBrake)

controller = Input(12000, 32768, 255)

clientsocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
clientsocket.connect(('192.168.0.200', 9595))

running = True
while running:
    #time.sleep(.05)
    time.sleep(1)
    state = controller.getState()
    #print( state )
       
    cmd = piCmd()
    cmd.lMotorVal = int(state["rtrigger"])
    cmd.rMotorVal = int(state["rtrigger"])
    cmd.lMotorBrake = 0
    cmd.rMotorBrake = 0
    
    data = cmd.getVals()
    numElements = len(data)
    packet = struct.pack('B'*numElements, *data)
    clientsocket.send(packet)

    if state["back"] == 1:
        running = False




#data = clientsocket.recv(1024)
clientsocket.close()
#print("rxed data", data)
