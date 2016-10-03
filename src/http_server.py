from tornado.ioloop import IOLoop
import tornado.web
import devices

class MainHandler(tornado.web.RequestHandler):
    def get(self):
        self.write("Home Automation Server Running!")

class ExampleHandler(tornado.web.RequestHandler):
    def get(self, *args):
        for arg in args:
            print(arg)
        self.write(self.request.path)

class StopHandler(tornado.web.RequestHandler):
    def get(self):
        self.write("stop")
        IOLoop.instance().stop()             

class ReloadHandler(tornado.web.RequestHandler):
    def initialize(self, context):
        self.context = context
    def get(self):
        self.write("reload")
        if self.context["pc"].is_running():
            self.context["pc"].stop()
            IOLoop.instance().spawn_callback(devices.init_device_loop, self.context)
                
        
class Application(tornado.web.Application):
    def __init__(self, context):
        handlers = [
            (r'/', MainHandler),
            (r"/reload", ReloadHandler, {"context" : context}),
            (r"/stop", StopHandler),
            #(r'/example/([0-9]{4})/([0-9]{2}).*', ExampleHandler)
        ]
        tornado.web.Application.__init__(self, handlers)
