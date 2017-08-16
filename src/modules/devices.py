class Device:
    def __init__(self, device):
        self.device = device
        self.name = device["_id"]
        self.enabled = device["enabled"]

    @property
    def value(self):
        return False

if __name__=="__main__":
    pass
