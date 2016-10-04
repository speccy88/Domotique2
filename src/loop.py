from datetime import datetime, timedelta
from modules.deviceFactory import deviceFactory
import tornado.ioloop

DELIMITER = ":"
D = DELIMITER

def init_device_loop(context):
    db = context["db"]
    devices_description = [db[device] for device in db]
    print(devices_description)

    devices = deviceFactory(devices_description)
    context["devices"] = devices

    pc = tornado.ioloop.PeriodicCallback(lambda: device_loop(devices), 1000)
    pc.start()
    context["pc"] = pc
    
def device_loop(devices):
    input_devices, output_devices = devices

    ### Need to implement loop to read input devices

    for device in output_devices:
        if device.enabled:
            current_time = datetime.now().time()
            if device.start_time < current_time < device.stop_time:
                device.output = 1
            else:
                device.output = 0

