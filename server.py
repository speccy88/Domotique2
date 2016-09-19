import time
import zmq

ctx = zmq.Context.instance()
s = ctx.socket(zmq.REP)
url = 'tcp://127.0.0.1:5555'
s.bind(url)

def process():
    pass

while True:
    try:
        msg = s.recv(zmq.NOBLOCK)
    except zmq.Again:
        # no message to recv, do other things
        time.sleep(0.1)
        process()
    else:
        print("received %r" % msg)
        s.send_string('0')