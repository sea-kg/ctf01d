#!/bin/bash

cd example_service1
docker-compose stop
docker-compose rm
cd ..

cd example_service2
docker-compose stop
docker-compose rm
cd ..

cd example_service3
docker-compose stop
docker-compose rm
cd ..

cd example_service4
docker-compose stop
docker-compose rm
cd ..
