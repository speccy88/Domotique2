class Device:
    def __init__(self, device):
        self.device = device
        self.name = device["_id"]
        self.enabled = device["enabled"]
        
        if "expression" in self.device:
            self.expression = self.device["expression"]

if __name__=="__main__":
    pass
