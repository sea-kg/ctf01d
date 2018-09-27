#!/bin/bash

sudo mkdir -p /usr/share/hackwars
sudo cp -rf ../service*.sample /usr/share/hackwars/
sudo cp -rf ../lazy-conf /usr/share/hackwars/

sudo systemctl disable lazy-conf.service
sudo systemctl disable service1.service
sudo systemctl disable service2.service
sudo systemctl disable service3.service
sudo systemctl disable service4.service

sudo cp -f ./lazy-conf.service  /etc/systemd/system/lazy-conf.service
sudo cp -f ./service1.service  /etc/systemd/system/service1.service
sudo cp -f ./service2.service  /etc/systemd/system/service2.service
sudo cp -f ./service3.service  /etc/systemd/system/service3.service
sudo cp -f ./service4.service  /etc/systemd/system/service4.service

sudo systemctl daemon-reload
sudo systemctl enable lazy-conf.service
sudo systemctl enable service1.service
sudo systemctl enable service2.service
sudo systemctl enable service3.service
sudo systemctl enable service4.service
sudo systemctl restart lazy-conf.service
sudo systemctl restart service1.service
sudo systemctl restart service2.service
sudo systemctl restart service3.service
sudo systemctl restart service4.service
