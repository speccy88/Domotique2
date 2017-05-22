from modules.arduino import Arduino

def deviceFactory(devices_description):
    input_devices = []
    output_devices = []
    analog_devices = []
    dht22_devices = []
    for desc in devices_description:
        if "device_type" in desc:
            ### Arduino device type
            if desc["device_type"] == "arduino":
                if all(k in desc for k in ("device_mode", "device_ip", "device_pin")):
                    ### Arduino digital out pin
                    if desc["device_mode"] == "digital_out":
                        output_devices.append(Arduino(desc))
                    elif desc["device_mode"] == "digital_in" and 0:
                        input_devices.append(Arduino(desc))
                    elif desc["device_mode"] == "analog_in" and 0:
                        analog_devices.append(Arduino(desc))
                    elif desc["device_mode"] == "DHT22" and 0:
                        dht22_devices.append(Arduino(desc))
                    ### Arduino other modes
                    else:
                        pass
            ### ESP8266 device type
            elif desc["device_type"] == "ESP8266":
                pass
			### Other devices types continue here
            else:
                pass

    # All devices returned in a tuple
    return (input_devices, output_devices)
