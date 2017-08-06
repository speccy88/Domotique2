from datetime import datetime, timedelta
from modules.deviceFactory import deviceFactory
from astral import Astral, Location
from datetime import datetime, timedelta
import tornado.ioloop

DELIMITER = ":"
D = DELIMITER

def stringToTime(text):
    return datetime.strptime(text, '%H:%M').time()

def init_device_loop(context):
    db = context["db"]
    devices_description = [db[device] for device in db]
    print(devices_description)

    devices = deviceFactory(devices_description)
    context["devices"] = devices

    pc = tornado.ioloop.PeriodicCallback(lambda: device_loop(devices), 1000)
    pc.start()
    context["pc"] = pc

def initAstral(self):
    a = Astral()
    a.solar_depression = 'civil'
    l = Location()
    l.name = 'Dolbeau-Mistassini'
    l.region = 'Canada'
    l.latitude = 48.9016
    l.longitude = -72.2156
    l.timezone = 'America/Montreal'
    l.elevation = 139
    self.location = l

def updateSun(self):
    self.sun_date = datetime.now().date()
    self.sun = self.location.sun(date=self.sun_date, local=True)
    
def device_loop(devices):
    input_devices, output_devices = devices
    sensors_data = {}
    
    for device in input_devices:
        sensors_data[device.name] = device.value
    
    print(sensors_data)
    
    for device in output_devices:
        device_output = 0
        if device.enabled:
            if device.time_enabled:
                current_time = datetime.now().time()
                if device.start_time < current_time < device.stop_time:
                    device_output = 1
                else:
                    device_output = 0
        else:
            device_output = 0
        device.output = device_output

    ### Need to implement loop to read analog devices

    ### Need to implement loop to read DHT22 devices
