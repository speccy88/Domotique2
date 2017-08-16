from modules.devices import Device
from udp import UDPStream
import socket

ARDUINO_UDP_PORT = 5000
DELIMITER = ":"
D = DELIMITER

class Arduino(Device):
    def __init__(self, device):
        super(Arduino, self).__init__(device)
        self.pin = self.device["pin"]
        self.ip = self.device["ip"]

    def setPin(self, state):
        cmd = "write" + D + str(self.pin) + D + str(state)
        print("Device ({}) command ({})".format(self.name,cmd))
        self.send(cmd)

    @property
    def output(self):
        pass

    @output.setter
    def output(self, state):
        self.setPin(state)   

    def send(self, cmd):
        udpsock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        udpsock.setblocking(False)
        udpsock.connect((self.ip, ARDUINO_UDP_PORT))
        s = UDPStream(udpsock)
        s.send(cmd.encode("ascii"))
        s.close()

if __name__=="__main__":
    d1_data = {"_id": "test"}
    d1 = Arduino(d1_data)
    
