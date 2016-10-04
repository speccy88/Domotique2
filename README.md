# Domotique2

## Initial setup
From a fresh Raspberry Pi image, you can setup everything with the included start-up script 
```
wget -O - https://raw.githubusercontent.com/speccy88/Domotique2/master/utils/startup.sh | sh
```
In this introduction the IP address of the Raspberry Pi is assumed to be "192.168.2.29".
When done, you should be able to see the server is running by accessing port 8000 on the Raspberry Pi : http://192.168.2.29:8000/

CouchDB is installed with the start-up script and is used to add the devices to be automated. You can access Futon (web interface of the database) on this url : http://192.168.2.29:5984/_utils/

When starting with a fresh image, it might be a good idea to change the default password with the command "passwd"

You can set a static IP by appending the text below to " /etc/dhcpcd.conf"
```
interface eth0
static ip_address=192.168.0.5/24
static routers=192.168.0.2
static domain_name_servers=192.168.0.2
```

## Database Format
[Go to documentation](docs/database.md)

## HTTP API
todo

## Systemctl commands to manage the service
```
sudo systemctl start dom    #Start the service
sudo systemctl stop dom     #Stop the service
sudo systemctl restart dom  #Restart the service
sudo systemctl enable dom   #Enable starting the service after a reboot
sudo systemctl disable dom  #Disable starting the service after a reboot
sudo systemctl status dom   #View the status of the service
sudo journalctl -e -u dom   #View logs of the service; Useful to see why the service does not start
```

## Notes on GIT
If you want to use git to modify the code you need to setup your name and email address first to be able to commit
```
git config --global user.email "you@example.com"
git config --global user.name "Your Name"
```
You can also store your username and password so you don't have to type them each time you push new changes
```
git config credential.helper store
```
### Quick reference
```
git add -A                      #Track new and deleted files to be commited
git commit -m "commit message"  #Save the changes you've done and add a quick description of the changes 
git push origin master          #Save local changes to online repository (you need to enter your GitHub username and password)
git pull                        #Get the latest version from the online repository
```

