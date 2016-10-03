from datetime import datetime, timedelta
from modules.arduino import Arduino
import tornado.ioloop

DELIMITER = ":"
D = DELIMITER

def init_device_loop(context):
    db = context["db"]
    devices_desc = [db[device] for device in db]
    print(devices_desc)

    devices = []
    for desc in devices_desc:
        if "type" in desc:
            if desc["type"] == "arduino":
                if all(k in desc for k in ("mode", "ip", "pin")):
                    devices.append(Arduino(desc))


    pc = tornado.ioloop.PeriodicCallback(lambda: device_loop(devices), 1000)
    pc.start()
    context["pc"] = pc
    
def device_loop(devices):
    for device in devices:
        pin = str(device.pin)
        current_time = datetime.now().time()
        if device.mode == "digital_out":
            if device.start_time < current_time < device.stop_time:
                cmd = "W" + D + pin + D + "1"
                print(device.name+" ON")
            else:
                cmd = "W" + D + pin + D + "0"
                print(device.name+" OFF")
            device.send(cmd)
