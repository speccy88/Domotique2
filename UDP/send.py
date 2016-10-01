import socket
import sys  

UDP_IP = "192.168.0.6"
UDP_PORT = 5000


#s = input('--> ')  
#MESSAGE = bytes(s, 'utf-8')
MESSAGE = raw_input('--> ')
#MESSAGE = s


print("UDP target IP:", UDP_IP)
print("UDP target port:", UDP_PORT)
print("Message:", MESSAGE)

sock = socket.socket(socket.AF_INET, # Internet
                     socket.SOCK_DGRAM) # UDP

sock.sendto(MESSAGE, (UDP_IP, UDP_PORT))
sock.settimeout(2.0)
recvmsg=sock.recv(1024)

print("Pin Status:", recvmsg)

#sock.close()
