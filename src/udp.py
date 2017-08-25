from tornado import gen
import tornado.ioloop
import socket
import time

class UDP_Communication:
    def __init__(self, ip, port, timeout=6):
        self.ioloop = tornado.ioloop.IOLoop.current()
        self.callback = None
        self.ip = ip
        self.port = port
        self.timeout = timeout
        
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.sock.setblocking(0)
        
    def _handle_input(self, fd, events):
        #print("Inside _handle_input")       
        #print("FD={} events={}".format(fd, events))
        try:
            data, addr = self.sock.recvfrom(1024)
            #print("DATA = {} ADDRESS = {}".format(data, addr))
            if self._read_timeout:
                #print("Removing timeout")
                self.ioloop.remove_timeout(self._read_timeout)
                self.callback(data.decode())
        except:
            pass
        
    def _read(self, callback=None):
        self.callback = callback
        self._read_timeout = self.ioloop.call_later(self.timeout, self._timeout )
        self.ioloop.add_handler(self.sock.fileno(), self._handle_input, self.ioloop.READ)
    
    def _timeout(self):
        #print("Inside _timeout")
        self.ioloop.remove_handler(self.sock.fileno())
        self.callback("Timeout")
         
    
    @gen.coroutine
    def send(self, message):
        ADDR = (self.ip, self.port)
        MESSAGE = message.encode("ascii")
        self.sock.sendto(MESSAGE, ADDR)
        reply = yield gen.Task(self._read)
        return reply