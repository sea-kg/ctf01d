#!/bin/bash

echo "Clean all images"

docker rmi -f sea5kg/ctf01d:latest
docker rmi -f ctf01d-game-simulation/service1_py:latest
docker rmi -f ctf01d-game-simulation/service2_go:latest
docker rmi -f ctf01d-game-simulation/service3_php:latest
docker rmi -f ctf01d-game-simulation/service4_cpp:latest

echo "Build images"

docker build -t sea5kg/ctf01d:latest ..

ret=$?
if [[ "$?" -ne "0" ]]; then
  exit -1
fi

docker build -t ctf01d-game-simulation/service1_py:latest ./vulnbox/service1_py

ret=$?
if [[ "$?" -ne "0" ]]; then
  exit -1
fi

docker build -t ctf01d-game-simulation/service2_go:latest ./vulnbox/service2_go

ret=$?
if [[ "$?" -ne "0" ]]; then
  exit -1
fi

docker build -t ctf01d-game-simulation/service3_php:latest ./vulnbox/service3_php

ret=$?
if [[ "$?" -ne "0" ]]; then
  exit -1
fi


docker build -t ctf01d-game-simulation/service4_cpp:latest ./vulnbox/service4_cpp

ret=$?
if [[ "$?" -ne "0" ]]; then
  exit -1
fi