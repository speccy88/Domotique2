# HTTP API
This document contains the list of available commands on the HTTP server

In the examples, the IP address and port of the Raspberry Pi is going to be "http://192.168.2.29:8000/"

Names surrounded by curly brackets are the parameters passed in the GET request.
There are several ways to execute the commands, either from a web browser, languages libraries or executables.
In a Linux command line, you can use curl
```
curl "192.168.2.29:8000/set/LED1/1"
```


## Command list
```
#SET
http://192.168.2.29:8000/set/{name}/{state}
name = device name in database
state = 0 or 1 (low/high)

#...
```
