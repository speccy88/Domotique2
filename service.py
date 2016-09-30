from tornado.ioloop import IOLoop
import http_server
import couchdb
import devices

#ADR = 'http://192.168.2.29:5984/'
ADR = 'http://192.168.0.5:5984/'


#Setup HTTP Server and database   
couch = couchdb.Server(ADR)
db = couch['test']
app = http_server.Application()

#Run server and process loop
app.listen(8000)  
IOLoop.instance().spawn_callback(devices.init_device_loop, db)
IOLoop.instance().start()