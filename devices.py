from astral import Astral, Location
from datetime import datetime
from udp import UDPStream
import tornado.ioloop
import socket

#udpsock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
#udpsock.setblocking(False)
#udpsock.connect( ('tracker.openbittorrent.com', 80) )
#s = UDPStream(udpsock)
#s.send( 'some data' )
#data = yield gen.Task( s.read_chunk )

ARDUINO_UDP_PORT = 5000
DELIMITER = ":"
D = DELIMITER

def getSun():
    a = Astral()
    a.solar_depression = 'civil'
    l = Location()
    l.name = 'Dolbeau-Mistassini'
    l.region = 'Canada'
    l.latitude = 48.9016
    l.longitude = -72.2156
    l.timezone = 'America/Montreal'
    l.elevation = 139
    sun = l.sun(date=datetime.now().date(), local=True)
    return sun
    
def init_device_loop(context):
    db = context["db"]
    devices = [db[device] for device in db]
    print(devices)
    pc = tornado.ioloop.PeriodicCallback(lambda: device_loop(devices), 1000)
    pc.start()
    context["pc"] = pc
    
    
def device_loop(devices):
    for device in devices:
        print("Device name is : "+device["_id"])
        if "type" in device:
            ### ARDUINO
            if device["type"] == "arduino":
                if all(k in device for k in ("mode", "ip", "pin")):
                    udpsock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
                    udpsock.setblocking(False)
                    udpsock.connect((device["ip"], ARDUINO_UDP_PORT))
                    s = UDPStream(udpsock)
                    pin = str(device["pin"])
                    if device["mode"] == "digital_out":
                        if all(k in device for k in ("start_time","stop_time")):
                            if "dusk_on" in device:
                                start = getSun()["dusk"]
                            else:    
                                start = datetime.strptime(device["start_time"], '%H:%M')
                            stop = datetime.strptime(device["stop_time"], '%H:%M')
                            current = datetime.now()
                            if start.time() < current.time() < stop.time():
                                cmd = "W" + D + pin + D + "1"
                                print(device["_id"]+" ON")
                            else:
                                cmd = "W" + D + pin + D + "0"
                                print(device["_id"]+" OFF")
                            
                            s.send(cmd.encode("ascii"))
                            s.close()
        
