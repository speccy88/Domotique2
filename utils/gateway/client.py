from tornado.websocket import websocket_connect
from tornado import ioloop
from tornado import gen
from time import sleep
import connection

THING = "test%20device%20Fred"

jwt = connection.load_jwt_from_file()

URL = "wss://" + connection.USER + ".mozilla-iot.org/things/" + THING + "?jwt=" + jwt
print(URL)


MESSAGE_ON = '{"messageType": "setProperty",   "data": {"on": true   } }'
MESSAGE_OFF = '{"messageType": "setProperty",   "data": {"on": false   } }'

@gen.coroutine
def onconnect(future):
    ws = future.result()
    while 1:
        ws.write_message(MESSAGE_ON)
        msg = yield ws.read_message()
        print(msg)
        yield gen.sleep(0.5)
        ws.write_message(MESSAGE_OFF)
        msg = yield ws.read_message()
        print(msg)
        yield gen.sleep(0.5)

 
if __name__ == '__main__':
    future = websocket_connect(URL, callback = onconnect)
    ioloop.IOLoop.instance().start()
