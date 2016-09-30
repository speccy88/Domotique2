import tornado.ioloop
import tornado.web
from tornado.tcpserver import TCPServer

class MainHandler(tornado.web.RequestHandler):
    def get(self):
        global x
        x+=1
        #self.write("root")

class ToggleHandler(tornado.web.RequestHandler):
    def get(self):
        if periodic.is_running():
            periodic.stop()
            self.write("Stopping...")
        else:
            periodic.start()
            self.write("Starting...")            
        
class StopHandler(tornado.web.RequestHandler):
    def get(self):
        self.write("stop")
        tornado.ioloop.IOLoop.current().stop()        
        
def make_app():
    return tornado.web.Application([
        (r"/", MainHandler),
        (r"/stop", StopHandler),
        (r"/toggle", ToggleHandler),
    ], debug=False)

    
#example tcp fonctionnel
def message(text):
    print(text+str(x))    

def messageSlow(text):
    print(text)
    from time import sleep
    sleep(2)
    
def handle_stream(stream, address):
        #print(dir(stream))
        stream.write(b"Allo mon gars\n\r")
        def on_body(data):
            print(data)
            stream.close()
        stream.read_bytes(2, on_body)    
        #stream.close()


        
periodic = tornado.ioloop.PeriodicCallback(lambda: message("PC"), 1000)
periodic2 = tornado.ioloop.PeriodicCallback(lambda: messageSlow("PC Slow"), 5000)
x=1

    
if __name__ == "__main__":
    app = make_app()
    app.listen(8888)
    
    #example tcp
    server = TCPServer()
    server.handle_stream = handle_stream
    server.listen(8000)
    
    
    periodic.start()
    periodic2.start()
    #tornado.ioloop.PeriodicCallback(lambda: message("allo"), 1000).start()
    tornado.ioloop.IOLoop.current().start()