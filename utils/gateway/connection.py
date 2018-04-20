from tornado.websocket import websocket_connect
from tornado import ioloop
from tornado import gen
from time import sleep
import requests
import json
from pprint import pprint
import socket

user_info = json.load(open('.password'))

BASE_HEADERS = {"Content-Type": "application/json", "Accept": "application/json"}
PASSWORD = user_info["password"]
EMAIL = user_info["email"]
USER = user_info["user"]
URL = "https://" + USER + ".mozilla-iot.org"

def get_json_web_token():
    headers = BASE_HEADERS
    data = {"email": EMAIL, "password": PASSWORD}
    r = requests.post(URL+"/login", headers=headers, json=data)
    return r.json()["jwt"]

def write_jwt_to_file(jwt):
    with open(".jwt", "w") as f: 
        f.write(jwt) 

def load_jwt_from_file():
    with open(".jwt") as f: 
        jwt = f.read()
    return jwt 

def get_things(jwt):
    headers = dict(BASE_HEADERS)
    headers.update({"Authorization": "Bearer "+jwt})
    r = requests.get(URL+"/things", headers=headers)
    return r.json()

def get_property(jwt, thing, prop):
    headers = dict(BASE_HEADERS)
    headers.update({"Authorization": "Bearer "+jwt})
    r = requests.get(URL+"/things/" + thing + "/properties/" + prop, headers=headers)
    return r.json()

def set_property(jwt, thing, prop, value):
    headers = dict(BASE_HEADERS)
    headers.update({"Authorization": "Bearer "+jwt})
    data = {prop: value}
    r = requests.put(URL+"/things/" + thing + "/properties/" + prop, headers=headers, json=data)
    return r.json()

def broadcast():
    UDP_IP = "192.168.1.255"
    UDP_SEND_PORT = 5000
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    s.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)
    s.sendto("broadcast", (UDP_IP, UDP_SEND_PORT))
    s.close()


@gen.coroutine
def onconnect(future):
    MESSAGE_ON = '{"messageType": "setProperty",   "data": {"on": true   } }'
    MESSAGE_OFF = '{"messageType": "setProperty",   "data": {"on": false   } }'
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

def test_create_token():
    jwt = get_json_web_token()
    write_jwt_to_file(jwt)

def test_get_things():
    jwt = load_jwt_from_file()
    things = get_things(jwt)
    pprint(things)

def test_set_property():
    jwt = load_jwt_from_file()
    response = set_property(jwt, "test device Fred", "on", "true")
    pprint(response)

def test_websocket():
    THING = "test%20device%20Fred"
    jwt = load_jwt_from_file()
    URL = "wss://" + USER + ".mozilla-iot.org/things/" + THING + "?jwt=" + jwt
    future = websocket_connect(URL, callback = onconnect)
    ioloop.IOLoop.instance().start()

def test_http():
    jwt = load_jwt_from_file()
    set_property(jwt, "test device Fred", "on", True)
    print(get_property(jwt, "test device Fred", "on"))
    set_property(jwt, "test device Fred", "on", False)
    print(get_property(jwt, "test device Fred", "on"))

if __name__=="__main__":
    #test_websocket()
    #test_get_things()
    #test_set_property()
    broadcast()
