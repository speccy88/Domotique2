#Get sensor data
sensors = {
    "light" : True,
    "rain" : False,
    "temperature" : 22,
    }

print(sensors)

def calculate_expression(expression, data):
    exec_string = '_result = ({})'.format(expression)
    try:
        exec(exec_string, None, data)
    except:
        raise Exception('There is an error in the expression : {}'.format(expression))
    result = sensors['_result']
    del sensors['_result']
    return result

cond2 = "(rain | light) & (temperature > 2)"
print(calculate_expression(cond2, sensors))

print(sensors)

