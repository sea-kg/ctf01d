#!/bin/bash

cd example_service1
docker-compose build
docker-compose up --no-start
docker-compose start
cd ..

cd example_service2
docker-compose build
docker-compose up --no-start
docker-compose start
cd ..

cd example_service3
docker-compose build
docker-compose up --no-start
docker-compose start
cd ..

cd example_service4
docker-compose build
docker-compose up --no-start
docker-compose start
cd ..
