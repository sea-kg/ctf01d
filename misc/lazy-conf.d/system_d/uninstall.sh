#!/bin/bash

sudo systemctl stop lazy-conf.service

sudo systemctl disable lazy-conf.service

# sudo rm -rf /usr/share/hackwars

sudo rm -f /etc/systemd/system/lazy-conf.service

sudo systemctl daemon-reload
