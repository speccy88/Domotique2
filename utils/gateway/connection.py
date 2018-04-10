import requests
import json
from pprint import pprint

user_info = json.load(open('.password'))

BASE_HEADERS = {"Content-Type": "application/json", "Accept": "application/json"}
PASSWORD = user_info["password"]
EMAIL = user_info["email"]
URL = "https://" + user_info["user"] + ".mozilla-iot.org"

def get_json_web_token():
    headers = BASE_HEADERS
    data = {"email": EMAIL, "password": PASSWORD}
    r = requests.post(URL+"/login", headers=headers, json=data)
    return r.json()["jwt"]

def write_jwt_to_file(jwt):
    with open(".jwt", "w") as f: 
        f.write(jwt) 

def load_jwt_from_file():
    with open(".jwt") as f: 
        jwt = f.read()
    return jwt 

def get_things(jwt):
    headers = dict(BASE_HEADERS)
    headers.update({"Authorization": "Bearer "+jwt})
    r = requests.get(URL+"/things", headers=headers)
    return r.json()

def get_property(jwt, thing, prop):
    headers = dict(BASE_HEADERS)
    headers.update({"Authorization": "Bearer "+jwt})
    r = requests.get(URL+"/things/" + thing + "/properties/" + prop, headers=headers)
    return r.json()

def set_property(jwt, thing, prop, value):
    headers = dict(BASE_HEADERS)
    headers.update({"Authorization": "Bearer "+jwt})
    data = {prop: value}
    r = requests.put(URL+"/things/" + thing + "/properties/" + prop, headers=headers, json=data)
    return r.json()

if __name__=="__main__":
    #jwt = get_json_web_token()
    #write_jwt_to_file(jwt)
    
    jwt = load_jwt_from_file()
    things = get_things(jwt)
    pprint(things)

    set_property(jwt, "test device Fred", "on", True)
    print(get_property(jwt, "test device Fred", "on"))


    set_property(jwt, "test device Fred", "on", False)
    print(get_property(jwt, "test device Fred", "on"))
