from gateway_addon import Property
import socket
import sys
import time
import select

#List of constants for UDP socket communication
UDP_IP = "192.168.1.95"
UDP_SEND_PORT = 5000
UDP_RECV_PORT = 4000
TIMEOUT_SECONDS = 3
RECV_BUFFER_LEN = 200


#Standard send function
def send(sock, msg):  
    print("UDP target IP:", UDP_IP)
    print("UDP target port:", UDP_SEND_PORT)
    print("UDP listen port:", UDP_RECV_PORT)
    print("Receive timeout : {}s".format(TIMEOUT_SECONDS))    
    print("Sent command:", msg)

    sock.sendto(msg.encode('utf-8'), (UDP_IP, UDP_SEND_PORT))
    sock.settimeout(TIMEOUT_SECONDS)
    
    try:
        recvmsg=sock.recv(RECV_BUFFER_LEN)
        print("Reply:", recvmsg)
    
    except:
        print("Timeout, no message received")

    return recvmsg

class ArduinoProperty(Property):
    def __init__(self, device, name, description, value):
        Property.__init__(self, device, name, description)
        self.set_cached_value(value)


class ArduinoPlugProperty(ArduinoProperty):
    def set_value(self, value):
        self.set_cached_value(value)
        self.device.notify_property_changed(self)

    def update(self, value):
        if value != self.value:
            self.set_cached_value(value)
            self.device.notify_property_changed(self)

class ArduinoBulbProperty(ArduinoProperty):
    def set_value(self, value):
        print("set value + "+str(value))
        #Create and bind socket    
        sock = socket.socket(socket.AF_INET, # Internet
                    socket.SOCK_DGRAM) # UDP

        if value:
            MESSAGE = 'write:3:1'
        else:
            MESSAGE = 'write:3:0'

        print(MESSAGE)
        retval = send(sock, MESSAGE)
        print("Return value: "+str(retval))
        sock.close()
        self.set_cached_value(value)
        self.device.notify_property_changed(self)
        print("EXIT")
    
    def update(self, value):
        if value != self.value:
            self.set_cached_value(value)
            self.device.notify_property_changed(self)
