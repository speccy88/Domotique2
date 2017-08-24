from tornado import ioloop
import http_server
import couchdb
import automation

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
app.listen(8000) 

io_loop = ioloop.IOLoop.current()

automation.init_device_loop(context)
io_loop.start()
