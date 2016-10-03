from tornado import gen
from lxml import html
from tornado.ioloop import IOLoop
from tornado.httpclient import AsyncHTTPClient

URL = "https://meteo.gc.ca/city/pages/qc-64_metric_f.html"
URL2 = "http://google.ca/search?q=dusk"

@gen.coroutine
def getData():
    http_client = AsyncHTTPClient()
    response = yield http_client.fetch(URL2)
    tree = html.fromstring(response.body)
    #data = tree.xpath('//*[@id="data"]/div/div[3]/dl/dd[1]/text()')[0]
    #data = tree.xpath('.')
    
    print(data)

if __name__=="__main__":
    IOLoop.current().run_sync(getData)