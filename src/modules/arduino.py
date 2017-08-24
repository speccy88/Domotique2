from modules.devices import Device
from tornado import gen
from udp import UDP_Communication

ARDUINO_UDP_PORT = 5000
DELIMITER = ":"
D = DELIMITER

class Arduino(Device):
    def __init__(self, device):
        super(Arduino, self).__init__(device)
        self.pin = self.device["pin"]
        self.ip = self.device["ip"] 
        self.port = 5000
        
        self.udp_comm = UDP_Communication(self.ip, self.port)

    @gen.coroutine    
    def setPin(self, state):
        cmd = "write" + D + str(self.pin) + D + str(state)
        print("Device ({}) command ({})".format(self.name,cmd))
        yield self.udp_comm.send(cmd)
    
    @gen.coroutine    
    def getPin(self):
        if self.enabled:
            cmd = "read" + D + str(self.pin)
            print("Device ({}) command ({})".format(self.name,cmd))
            data = yield self.udp_comm.send(cmd)
            print("reply = {}".format(data))
            return data
        else:
            return None
        
    @gen.coroutine 
    def value(self):
        value = yield self.getPin()        
        return value
    
    @gen.coroutine 
    def output(self, state):
        yield self.setPin(state)   

if __name__=="__main__":
    d1_data = {"_id": "test"}
    d1 = Arduino(d1_data)
    
