from astral import Astral, Location
from datetime import datetime, timedelta

def stringToTime(text):
    return datetime.strptime(text, '%H:%M').time()

phase_of_day = ("dawn", "sunrise", "noon", "sunset", "dusk")

class Device:
    def __init__(self, device):
        self.device = device
        self.name = device["_id"]
        self.initAstral()
        self.updateStartStopTime()
    
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

    def updateStartStopTime(self):
        self.updateSun()
        if "start_time" in self.device:
            start_time = self.device["start_time"]
            if start_time in phase_of_day:
                self._start = self.sun[start_time].time()
            else:
                self._start = stringToTime(start_time)
        
        if "stop_time" in self.device:
            stop_time = self.device["stop_time"]
            if stop_time in phase_of_day:
                self._stop = self.sun[stop_time].time()
            else:
                self._stop = stringToTime(stop_time)

    @property
    def start(self):
        if datetime.now().date() != self.sun_date:
            updateStartStopTime()
        return self._start

    @property
    def stop(self):
        if datetime.now().date() != self.sun_date:
            updateStartStopTime()
        return self._stop

if __name__=="__main__":
    d1_data = {"_id": "test", "start_time": "16:00", "stop_time": "sunset"}
    d1 = Device(d1_data)
    print(d1.start)
    print(d1.stop)
    
