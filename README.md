# Domotique2

## Initial setup
From a fresh Raspberry Pi image, you can setup everything with the included start-up script 
```
wget -O - https://raw.githubusercontent.com/speccy88/Domotique2/master/utils/startup.sh | sh
```
In this introduction the IP address of the Raspberry Pi is assumed to be "192.168.2.29".
When done, you should be able to see the server is running by accessing port 8000 on the Raspberry Pi : http://192.168.2.29:8000/

CouchDB is installed with the start-up script and is used to add the devices to be automated. You can access Futon (web interface of the database) on this url : http://192.168.2.29:5984/_utils/

## Notes on GIT
If you want to use git to modify the code you need to setup your name and email address first to be able to commit
```
git config --global user.email "you@example.com"
git config --global user.name "Your Name"
```
Quick reference
```
git add -A  #Track new and deleted files to be commited
git commit -m "commit message"  #Save the changes you've done and add a quick description of the changes 
git push origin master  #
```

