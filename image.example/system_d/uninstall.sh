#!/bin/bash

sudo systemctl stop lazy-conf.service
sudo systemctl stop service1.service
sudo systemctl stop service2.service
sudo systemctl stop service3.service
sudo systemctl stop service4.service

sudo systemctl disable lazy-conf.service
sudo systemctl disable service1.service
sudo systemctl disable service2.service
sudo systemctl disable service3.service
sudo systemctl disable service4.service

sudo rm -rf /usr/share/hackwars

sudo rm -f /etc/systemd/system/lazy-conf.service
sudo rm -f /etc/systemd/system/service1.service
sudo rm -f /etc/systemd/system/service2.service
sudo rm -f /etc/systemd/system/service3.service
sudo rm -f /etc/systemd/system/service4.service

sudo systemctl daemon-reload