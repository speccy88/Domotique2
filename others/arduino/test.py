import socket
import sys
from time import sleep
import select
import threading

#List of constats for UDP socket communication
UDP_IP = "192.168.1.95"
UDP_SEND_PORT = 5000
UDP_RECV_PORT = 4000
TIMEOUT_SECONDS = 3
RECV_BUFFER_LEN = 200

#sock.bind(('192.168.1.101', UDP_RECV_PORT))

def worker(num):
    """thread worker function"""
    print('Worker: %s' % num)
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    MESSAGE = 'write:'+str(num)+':0'
    for _ in range(5):
        sock.sendto(MESSAGE.encode('utf-8'), (UDP_IP, UDP_SEND_PORT))
        recvmsg=sock.recv(RECV_BUFFER_LEN)
        print(recvmsg)
        sleep(1)
    sock.close()
    return

threads = []
for i in range(3,5):
    t = threading.Thread(target=worker, args=(i,))
    threads.append(t)
    t.start()
