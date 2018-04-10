from gateway_addon import Device
import gateway_addon
import threading
import time

from .arduino_property import ArduinoBulbProperty, ArduinoPlugProperty

_POLL_INTERVAL = 5

class ArduinoDevice(Device):
    def __init__(self, adapter, _id):
        Device.__init__(self, adapter, _id)

        self.description = _id+" desc"
        self.name = _id+" name"

        t = threading.Thread(target=self.poll)
        t.daemon = True
        t.start()

class ArduinoPlug(ArduinoDevice):
    def __init__(self, adapter, _id):
        ArduinoDevice.__init__(self, adapter, _id)
        self.type = 'onOffSwitch'
        self.properties['on'] = ArduinoPlugProperty(self, 'on', {'type': 'boolean'}, True)

    def poll(self):
        while True:
            time.sleep(_POLL_INTERVAL)
            state = True
            for prop in self.properties.values():
                prop.update(state)

class ArduinoBulb(ArduinoDevice):
    def __init__(self, adapter, _id):
        ArduinoDevice.__init__(self, adapter, _id)
        state = True
        self.type = 'onOffLight'
        self.properties['on'] = ArduinoBulbProperty(self,'on',{'type': 'boolean'},True)

    def poll(self):
        while True:
            time.sleep(_POLL_INTERVAL)

