from tornado.ioloop import IOLoop
import tornado.web

class MainHandler(tornado.web.RequestHandler):
    def get(self, *args):
        for arg in args:
            print(arg)
        self.write(self.request.path)

class StopHandler(tornado.web.RequestHandler):
    def get(self):
        self.write("stop")
        IOLoop.instance().stop()             
        
class Application(tornado.web.Application):
    def __init__(self):
        handlers = [
            (r"/stop", StopHandler),
            (r'/articles/([0-9]{4})/([0-9]{2}).*', MainHandler)
        ]
        tornado.web.Application.__init__(self, handlers)