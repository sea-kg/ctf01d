#!/bin/bash


./stop_and_clean.sh



docker stop ctf01d_team1_service1_py
docker stop ctf01d_team1_service2_go
docker stop ctf01d_team1_service3_php
docker stop ctf01d_team1_service4_cpp

docker stop ctf01d_team2_service1_py
docker stop ctf01d_team2_service2_go
docker stop ctf01d_team2_service3_php
docker stop ctf01d_team2_service4_cpp

docker stop ctf01d_team3_service1_py
docker stop ctf01d_team3_service2_go
docker stop ctf01d_team3_service3_php
docker stop ctf01d_team3_service4_cpp

docker network rm ctf01d_team1_net
docker network rm ctf01d_team2_net
docker network rm ctf01d_team3_net
docker network rm ctf01d_jury_net

function createNetwork {
  echo "create ctf01d_$1_net"
  docker network create \
    --driver bridge \
    --gateway $2.1 \
    --subnet=$2.0/24 \
    --ip-range=$2.3/24 \
    "ctf01d_$1_net"
}

createNetwork "team1" "10.10.11"
createNetwork "team2" "10.10.12"
createNetwork "team3" "10.10.13"
createNetwork "jury" "10.10.100"

exit;
# TODO 

# start service1 

function startService1Py {
  echo "Start ctf01d_$1_service1_py"
  docker run \
    -d \
    --rm \
    --memory 128MB \
    --memory-swap 128MB \
    -p "$2.2:4101":4101 \
    -v `pwd`/vulnbox/service1_py/flags:/root/flags \
    --net "ctf01d_$1_net" \
    --name "ctf01d_$1_service1_py" \
    ctf01d-game-simulation/service1_py:latest
}

startService1Py "team1" "10.10.1"
startService1Py "team2" "10.10.2"
startService1Py "team3" "10.10.3"

function startService2Go {
  echo "Start ctf01d_$1_service2_go"

  docker run \
    -d \
    --rm \
    --memory 128MB \
    --memory-swap 128MB \
    -p "$2.1:4101":4101 \
    -v `pwd`/vulnbox/service1_py/flags:/root/flags \
    --net "ctf01d_$1_net" \
    --name "ctf01d_$1_service1_py" \
    ctf01d-game-simulation/service1_py:latest
}

# 

docker service create \
  --env MYSQL_USER= \
  --env MYSQL_PASSWORD= \
  --env MYSQL_HOST= \
  --env MYSQL_DATABASE= \
  --publish "10.10.11.2:4102":4102 \
  --network "ctf01d_team1_net" \
  --limit-memory 64MB \
  --replicas 1 \
  --restart-condition "any" \
  --name "ctf01d_team1_service2_go_db" \
  ctf01d-game-simulation/service2_go:latest

# MYSQL_ROOT_PASSWORD: service2_go
# MYSQL_DATABASE: example_service2
# MYSQL_USER: example_service2
# MYSQL_PASSWORD: example_service2

--mode global \
# --publish "10.10.1.1:4102":4102 \

docker service create \
  --env MYSQL_USER=service2_go \
  --env MYSQL_PASSWORD=service2_go \
  --env MYSQL_HOST=ctf01d_team1_service2_go_db \
  --env MYSQL_DATABASE=service2_go \
  --network "ctf01d_team1_net" \
  --limit-memory 64MB \
  --restart-condition "any" \
  --name "ctf01d_team1_service2_go" \
  ctf01d-game-simulation/service2_go:latest

") + ":" + os.Getenv("") + "@tcp(" + os.Getenv("") + ":3306)/" + os.Getenv("")


docker run \
  -d \
  --rm \
  --memory 128MB \
  --memory-swap 128MB \
  -p "$2.1:4101":4101 \
  -v `pwd`/vulnbox/service1_py/flags:/root/flags \
  --net "ctf01d_$1_net" \
  --name "ctf01d_$1_service1_py" \
  ctf01d-game-simulation/service1_py:latest