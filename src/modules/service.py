from tornado.ioloop import IOLoop
import http_server
import couchdb
import loop

ADR = 'http://127.0.0.1:5984/'

#Setup HTTP Server and database   
couch = couchdb.Server(ADR)

try:
    db = couch["devices"]
except:
    print("Unable to retrieve devices database")
    quit()

try:
    loc = couch["locations"]
except:
    print("Unable to retrieve locations database")
    quit()
	
#Context
context = {"db": db}

app = http_server.Application(context)


#Run server and process loop
app.listen(8000)  
IOLoop.instance().spawn_callback(loop.init_device_loop, context)
IOLoop.instance().start()