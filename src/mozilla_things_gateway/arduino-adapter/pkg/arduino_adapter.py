from gateway_addon import Adapter
from .arduino_device import ArduinoDigitalInput, ArduinoDigitalOutput
import logging

logger = logging.getLogger('arduino-adapter')
hdlr = logging.FileHandler('/var/tmp/arduino.log')
formatter = logging.Formatter('%(asctime)s %(levelname)s %(message)s')
hdlr.setFormatter(formatter)
logger.addHandler(hdlr) 
logger.setLevel(logging.INFO)

class ArduinoAdapter(Adapter):
    def __init__(self, verbose=False):
        self.name = self.__class__.__name__
        Adapter.__init__(self,
                         'arduino-adapter',
                         'arduino-adapter',
                         verbose=verbose)

        self.handle_device_added(ArduinoDigitalOutput(self, "RED_LED1", "192.168.1.95", 3))
        self.handle_device_added(ArduinoDigitalOutput(self, "BLUE_LED1", "192.168.1.95", 5))
        self.handle_device_added(ArduinoDigitalInput(self, "BUTTON1", "192.168.1.95", 3))
        self.handle_device_added(ArduinoDigitalOutput(self, "RED_LED2", "192.168.1.90", 3))
        self.handle_device_added(ArduinoDigitalOutput(self, "BLUE_LED2", "192.168.1.90", 5))
        self.handle_device_added(ArduinoDigitalInput(self, "BUTTON2", "192.168.1.90", 3))
