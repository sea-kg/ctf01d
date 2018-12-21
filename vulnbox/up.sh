#!/bin/bash


docker-compose \
   -f example_service1/docker-compose.yml \
   -f example_service2/docker-compose.yml \
   -f example_service3/docker-compose.yml \
   -f example_service4/docker-compose.yml \
   up --build
