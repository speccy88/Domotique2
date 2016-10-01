from tornado.ioloop import IOLoop
import http_server
import couchdb
import devices

#ADR = 'http://192.168.2.29:5984/'
ADR = 'http://192.168.0.5:5984/'


#Setup HTTP Server and database   
couch = couchdb.Server(ADR)
db = couch["test"]

#Context
context = {"db": db}

app = http_server.Application(context)


#Run server and process loop
app.listen(8000)  
IOLoop.instance().spawn_callback(devices.init_device_loop, context)
IOLoop.instance().start()