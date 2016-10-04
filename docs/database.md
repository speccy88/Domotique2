# Database Format
This document contains example devices that you can copy/paste in Futon

## Arduino
An Arduino device is a Arduino compatible board that communicate via a ENC28J60 module

### Digital output
```
{
   "_id": "LED1",
   "start_time": "13:00",
   "stop_time": "sunset",
   "type": "arduino",
   "mode": "digital_out",
   "ip": "127.0.0.1",
   "pin": 3,
   "enabled": true
}
```
