from modules.arduino import Arduino

def deviceFactory(devices_description):
    input_devices = []
    output_devices = []

    for desc in devices_description:
        if desc["device_type"] == "arduino":
            if all(k in desc for k in ("mode", "ip", "pin")):
                ### Arduino digital out pin
                if desc["mode"] == "out":
                    output_devices.append(Arduino(desc))
                elif desc["mode"] == "in":
                    input_devices.append(Arduino(desc))

    # All devices returned in a tuple
    return (input_devices, output_devices)
