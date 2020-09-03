
#!/bin/bash

echo "Stop containers and clean"

rm -rf tmp

docker service rm

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

