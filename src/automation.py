from datetime import datetime, timedelta
from modules.deviceFactory import deviceFactory
from astral import Astral, Location
from datetime import datetime, timedelta
from tornado import gen, ioloop


DELIMITER = ":"
D = DELIMITER

io_loop = ioloop.IOLoop.current()

def stringToTime(text):
    return datetime.strptime(text, '%H:%M').time()

def init_device_loop(context):
    db = context["db"]
    devices_description = [db[device] for device in db]
    print(devices_description)

    devices = deviceFactory(devices_description)
    context["devices"] = devices
    
    io_loop.spawn_callback(device_loop, devices)

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



@gen.coroutine    
def device_loop(devices):
    input_devices, output_devices = devices
    sensors_data = {}
    
    #Get all values of input devices
    values = yield [device.value() for device in input_devices]
    
    #Put the values in a dict with the key being the input device name for each value
    for value, device in zip(values, input_devices):
        sensors_data[device.name] = value
    
    print(sensors_data)
    
    for device in output_devices:
        if device.enabled:
            if device.calculate_expression(sensors_data):
                device_output = 1
            else:
                device_output = 0
        else:
            device_output = 0
            
        yield device.output(device_output)
        
    io_loop.stop()
