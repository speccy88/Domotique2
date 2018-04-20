from .arduino_property import ArduinoDigitalOutputProperty, ArduinoDigitalInputProperty
from gateway_addon import Device
import gateway_addon
import threading
import time
import logging
logger = logging.getLogger('arduino-adapter')

_POLL_INTERVAL = 1

class ArduinoDevice(Device):
    def __init__(self, adapter, _id, ip_address, pin):
        Device.__init__(self, adapter, _id)

        self.description = "Arduino device"
        self.name = _id
        self.pin = pin
        self.ip = ip_address

class ArduinoDigitalOutput(ArduinoDevice):
    def __init__(self, adapter, _id, ip_address, pin):
        ArduinoDevice.__init__(self, adapter, _id, ip_address, pin)
        self.type = 'onOffLight'
        self.properties['on'] = ArduinoDigitalOutputProperty(self,'on',{'type': 'boolean'})

class ArduinoDigitalInput(ArduinoDevice):
    def __init__(self, adapter, _id, ip_address, pin):
        ArduinoDevice.__init__(self, adapter, _id, ip_address, pin)
        self.type = 'binarySensor'
        self.properties['on'] = ArduinoDigitalInputProperty(self,'on',{'type': 'boolean'})
        
        t = threading.Thread(target=self.poll)
        t.daemon = True
        t.start()

    def poll(self):
        while True:
            logger.info("update inside poll")
            self.properties['on'].update()
            time.sleep(_POLL_INTERVAL)

