import socket

UDP_IP = "192.168.2.199"
UDP_PORT = 5000


s = input('--> ')  
MESSAGE = bytes(s, 'utf-8')


print("UDP target IP:", UDP_IP)
print("UDP target port:", UDP_PORT)
print("message:", MESSAGE)

sock = socket.socket(socket.AF_INET, # Internet
                     socket.SOCK_DGRAM) # UDP
sock.sendto(MESSAGE, (UDP_IP, UDP_PORT))
