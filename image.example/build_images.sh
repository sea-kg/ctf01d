#!/bin/bash

sudo mkdir -p /usr/share/hackwars
sudo mkdir -p /usr/share/hackwars/lazy-conf
sudo mkdir -p /usr/share/hackwars/service1.sample/flags
sudo mkdir -p /usr/share/hackwars/service2.sample/flags
sudo mkdir -p /usr/share/hackwars/service3.sample/flags
sudo mkdir -p /usr/share/hackwars/service4.sample/flags

sudo docker-compose rm

sudo docker rmi "jury-ad:lazy-conf"
sudo docker build --rm=true -t "jury-ad:lazy-conf" lazy-conf/

sudo docker rmi "jury-ad:service1.sample"
sudo docker build --rm=true -t "jury-ad:service1.sample" service1.sample/

sudo docker rmi "jury-ad:service2.sample"
sudo docker build --rm=true -t "jury-ad:service2.sample" service2.sample/

sudo docker rmi "jury-ad:service3.sample"
sudo docker build --rm=true -t "jury-ad:service3.sample" service3.sample/

sudo docker rmi "jury-ad:service4.sample"
sudo docker build --rm=true -t "jury-ad:service4.sample" service4.sample/
