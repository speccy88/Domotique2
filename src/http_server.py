from tornado.ioloop import IOLoop
import tornado.web
import loop

class BaseHandler(tornado.web.RequestHandler):
    def initialize(self, context):
        self.context = context

class MainHandler(BaseHandler):
    def get(self):
        self.write("Home Automation Server Running!")

class StopHandler(BaseHandler):
    def get(self):
        self.write("stop")
        IOLoop.instance().stop()             

class ReloadHandler(BaseHandler):
    def get(self):
        self.write("reload")
        if self.context["pc"].is_running():
            self.context["pc"].stop()
            IOLoop.instance().spawn_callback(loop.init_device_loop, self.context)
                
class SetOutputHandler(BaseHandler):
    def get(self, name, state):
        devices = self.context["devices"][1] #output devices
        for device in devices:
            if device.name == name:
                device.output = state
                self.write("Device ({}) output is set to {}\n".format(name, state))
                return
        raise tornado.web.HTTPError(404)

class Application(tornado.web.Application):
    def __init__(self, context):
        handlers = [
            [r'/', MainHandler],
            [r"/reload", ReloadHandler],
            [r"/stop", StopHandler],
            [r'/set/(?P<name>\w+)/(?P<state>[01])', SetOutputHandler]
        ]
        args = {"context" : context}
        [handler.append(args) for handler in handlers]
        tornado.web.Application.__init__(self, handlers)
