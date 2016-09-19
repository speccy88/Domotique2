import json
import zmq

ctx = zmq.Context.instance()
s = ctx.socket(zmq.REQ)
url = 'tcp://127.0.0.1:5555'
s.connect(url)

while True:
    choice = input("choice > ")
    
    if choice == '1':
        s.send_string("1")
        s.recv()
    elif choice == '2':
        s.send_string("2")
        s.recv()        
    elif choice == 'quit':
        break
    else:
        print("Wrong choice")