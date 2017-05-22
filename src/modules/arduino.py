from modules.devices import Device
from udp import UDPStream
import socket

ARDUINO_UDP_PORT = 5000
DELIMITER = ":"
D = DELIMITER

House_IP = "192.168.0.6"
Garage_IP = "192.168.0.7"
Pool_IP = "192.168.0.8"

class Arduino(Device):
    def __init__(self, device):
        super(Arduino, self).__init__(device)
        self.pin = self.device["device_pin"]
        self.mode = self.device["device_mode"]
        if self.device["device_ip"] == "House":
            self.ip = House_IP
        elif self.device["device_ip"] == "Garage":
            self.ip = Garage_IP        
        elif self.device["device_ip"] == "Pool":
            self.ip = Pool_IP
        else:
            self.ip = self.device["device_ip"]

    def setPin(self, state):
        cmd = "W" + D + str(self.pin) + D + str(state)
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
    d1_data = {"_id": "test", "time_start": "16:00", "time_stop": "sunset"}
    d1 = Arduino(d1_data)
    print(d1.start)
    print(d1.stop)

