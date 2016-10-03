from modules.devices import Device
from udp import UDPStream
import socket

ARDUINO_UDP_PORT = 5000

class Arduino(Device):
    def __init__(self, device):
        super(Arduino, self).__init__(device)
        self.ip = self.device["ip"]
        self.pin = self.device["pin"]
        self.mode = self.device["mode"]

    def send(self, cmd):
        udpsock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        udpsock.setblocking(False)
        udpsock.connect((self.ip, ARDUINO_UDP_PORT))
        s = UDPStream(udpsock)
        s.send(cmd.encode("ascii"))
        s.close()

if __name__=="__main__":
    d1_data = {"_id": "test", "start_time": "16:00", "stop_time": "sunset"}
    d1 = Arduino(d1_data)
    print(d1.start)
    print(d1.stop)

