from datetime import datetime
import tornado.ioloop

def init_device_loop(db):
    devices = [db[device] for device in db]
    print(devices)
    pc = tornado.ioloop.PeriodicCallback(lambda: device_loop(devices), 1000)
    pc.start()
    
def device_loop(devices):
    for device in devices:
        print("Device name is : "+device["_id"])
        if all(k in device for k in ("start_time","stop_time")):
            start = datetime.strptime(device["start_time"], '%H:%M')
            stop = datetime.strptime(device["stop_time"], '%H:%M')
            current = datetime.now()
            if start.time() < current.time() < stop.time(): 
                print(True)
            else:
                print(False)
    #tornado.ioloop.IOLoop.instance().stop()
        
