#!/bin/bash

sudo docker-compose rm
sudo docker rmi -f "jury-ad:lazy-conf"
sudo docker rmi -f "jury-ad:service1.sample"
sudo docker rmi -f "jury-ad:service2.sample"
sudo docker rmi -f "jury-ad:service3.sample"
sudo docker rmi -f "jury-ad:service4.sample"
