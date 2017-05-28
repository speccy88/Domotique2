import socket
import sys
import time
import select

#List of constats for UDP socket communication
UDP_IP = "192.168.0.7"
UDP_SEND_PORT = 5000
UDP_RECV_PORT = 1000
TIMEOUT_SECONDS = 0.5
RECV_BUFFER_LEN = 1024

#Add new shortcuts at the end of that list
shortcuts = [
        "w:3:1",
        "w:3:0",
        ]

#Parsing command line arguments, display help info if no arguments found
argc = len(sys.argv)
if argc != 2:
    print("-- send.py version 2.0 --")
    print("Enter a command directly or a shortcut number : 'python send.py w:3:1' 'python send.py 0'")
    print("You can alse enter 'python send.py loop' for the looping case\n")
    
    print("List of shortcuts")
    for i, cmd in enumerate(shortcuts):
        print("[{}] {}".format(i, cmd))
    
    quit()

#Create and bind socket    
sock = socket.socket(socket.AF_INET, # Internet
					socket.SOCK_DGRAM) # UDP

sock.bind(('', UDP_RECV_PORT))

#Standard send function
def send(msg):	
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

#Flash an LED        
def loop():	
    try:
        while(True):
            MESSAGE = 'W:3:0'
            sock.sendto(MESSAGE.encode('utf-8'), (UDP_IP, UDP_SEND_PORT))
            time.sleep(1)
            MESSAGE = 'W:3:1'
            sock.sendto(MESSAGE.encode('utf-8'), (UDP_IP, UDP_SEND_PORT))
            time.sleep(1)
    except KeyboardInterrupt:
        sock.close()
        quit()

        
cmd = sys.argv[1]
if cmd == "loop":
    loop()

#If there arguement is an integer, use the command in the shortcuts list    
try:
    index = int(cmd)
    send(shortcuts[index])

#Do nothing if the integer is out of shortcuts list range    
except IndexError:
    print("No shortcut found for index {}".format(cmd))
    
#If the arguement is a text string, send the command directly
except ValueError:
    send(cmd)

#Always close the socket at the end to prevent socket already in use errors    
sock.close()
    
