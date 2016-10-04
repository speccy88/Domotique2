from modules.arduino import Arduino

def deviceFactory(devices_description):
    input_devices = []
    output_devices = []
    for desc in devices_description:
        if "type" in desc:
            ### Arduino device type
            if desc["type"] == "arduino":
                if all(k in desc for k in ("mode", "ip", "pin")):
                    ### Arduino digital out pin
                    if desc["mode"] == "digital_out":
                        output_devices.append(Arduino(desc))
                    ### Arduino other modes
                    elif desc["mode"] == "other":
                        pass
            ### Other devices types continue here
            elif desc["type"] == "other":
                pass

    # All devices returned in a tuple
    return (input_devices, output_devices)
