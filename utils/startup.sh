#Script to install all pre-requisites to run the home automation service

#Ensure current directory is home
cd

#Required executables and useful tools
sudo apt-get -y install git vim htop couchdb python3 python3-pip

#Required Python 3 libraries
sudo pip3 install tornado couchdb astral

#Allow CouchDB access from any IP address and restart the process
sudo sed -i "/bind_address = 127.0.0.1/c\bind_address = 0.0.0.0" /etc/couchdb/default.ini
sudo systemctl restart couchdb

#Create devices database
curl -X PUT http://127.0.0.1:5984/devices

#Customize VIM
cat <<EOT > ~/.vimrc
set ts=4
set expandtab
syntax on
set background=dark
EOT

#Set correct time zone info (change if required)
sudo cp /usr/share/zoneinfo/America/Montreal /etc/localtime

#Clone Github repository
git clone https://github.com/speccy88/Domotique2

#Setup automatic start of the service
sudo cp Domotique2/utils/dom.service /lib/systemd/system/
sudo systemctl daemon-reload
sudo systemctl enable dom
sudo systemctl start dom

#End
echo "All Done!"
