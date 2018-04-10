from gateway_addon import Adapter

from .arduino_device import ArduinoBulb, ArduinoPlug


_TIMEOUT = 3


class ArduinoAdapter(Adapter):
    def __init__(self, verbose=False):
        self.name = self.__class__.__name__
        Adapter.__init__(self,
                         'arduino-adapter',
                         'arduino-adapter',
                         verbose=verbose)

        self.pairing = False
        self.start_pairing(_TIMEOUT)

    def start_pairing(self, timeout):
        self.pairing = True

        id = 'test device Fred'
        #device = ArduinoPlug(self, id)
        device = ArduinoBulb(self, id)
        self.handle_device_added(device)
        self.pairing = False


    def cancel_pairing(self):
        self.pairing = False
