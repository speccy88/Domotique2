class Device:
    def __init__(self, device):
        self.device = device
        self.name = device["_id"]
        self.enabled = device["enabled"]
        
        if "expression" in self.device:
            self.expression = self.device["expression"]
        
    def calculate_expression(self, data):
        exec_string = '_result = ({})'.format(self.expression)
        try:
            exec(exec_string, None, data)
        except:
            raise Exception('There is an error in the expression : {}'.format(self.expression))
        result = data['_result']
        del data['_result']
        return result

if __name__=="__main__":
    pass
