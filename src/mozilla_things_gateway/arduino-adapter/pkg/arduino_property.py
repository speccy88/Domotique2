from gateway_addon import Property
import socket
import sys
import time
import select
import logging

logger = logging.getLogger('arduino-adapter')

#List of constants for UDP socket communication
UDP_SEND_PORT = 5000
TIMEOUT_SECONDS = 3
RECV_BUFFER_LEN = 200

#Standard send function
def send(sock, ip, msg):  
    sock.sendto(msg.encode('utf-8'), (ip, UDP_SEND_PORT))
    sock.settimeout(TIMEOUT_SECONDS)
    try:
        recvmsg=sock.recv(RECV_BUFFER_LEN)
        logger.info("Reply:", recvmsg)
        return recvmsg
    
    except:
        logger.error("Timeout, no message received")

class ArduinoProperty(Property):
    def __init__(self, device, name, description):
        Property.__init__(self, device, name, description)
        self.pin = device.pin
        self.ip = device.ip
        self.set_cached_value(True)

class ArduinoDigitalOutputProperty(ArduinoProperty):
    def set_value(self, value):
        logger.info("set value + "+str(value))
        #Create and bind socket    
        sock = socket.socket(socket.AF_INET, # Internet
                    socket.SOCK_DGRAM) # UDP

        if value:
            MESSAGE = 'write:'+str(self.pin)+':1'
        else:
            MESSAGE = 'write:'+str(self.pin)+':0'

        logger.info(MESSAGE)
        retval = send(sock, self.ip, MESSAGE)
        logger.info("Return value: "+str(retval))
        sock.close()
        self.set_cached_value(value)
        self.device.notify_property_changed(self)
        logger.info("EXIT")

class ArduinoDigitalInputProperty(ArduinoProperty):
    def update(self):
        logger.info("in self.update before socket")
        sock = socket.socket(socket.AF_INET, # Internet
                    socket.SOCK_DGRAM) # UDP

        MESSAGE = 'status:'+str(self.pin)
        logger.info(MESSAGE)
        retval = send(sock, self.ip, MESSAGE)
        logger.info("Return value: "+str(retval))
        
        sock.close()
        if retval == b'1':
            value = True
            logger.info(value)
        else:
            value = False
            logger.info(value)
        self.set_cached_value(value)
        self.device.notify_property_changed(self)
        logger.info("EXIT")
