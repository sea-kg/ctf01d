#!/bin/bash

# sudo mkdir -p /usr/share/hackwars
# sudo cp -rf ../service*.sample /usr/share/hackwars/
# sudo cp -rf ../lazy-conf /usr/share/hackwars/

sudo systemctl disable lazy-conf.service
sudo systemctl disable service4.service

sudo cp -f ./lazy-conf.service  /etc/systemd/system/lazy-conf.service

sudo systemctl daemon-reload
sudo systemctl enable lazy-conf.service
sudo systemctl restart lazy-conf.service
