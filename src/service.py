from tornado.ioloop import IOLoop
import http_server
import couchdb
import devices

ADR = 'http://127.0.0.1:5984/'

#Setup HTTP Server and database   
couch = couchdb.Server(ADR)

try:
    db = couch["devices"]
except:
    print("Unable to retrieve devices database")
    quit()

#Context
context = {"db": db}

app = http_server.Application(context)


#Run server and process loop
app.listen(8000)  
IOLoop.instance().spawn_callback(devices.init_device_loop, context)
IOLoop.instance().start()
