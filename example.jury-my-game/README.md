# Prepare your game use FHQ Jury Attack Defence (example)

## Requriments

* docker
* docker-compose
* git

## How to prepare jury for your game

1. Clone project `git clone https://github.com/freehackquest/fhq-jury-ad /opt/fhq-jury-ad.git`
2. Move /opt/fhq-jury-ad.git/example.jury-my-game /opt/my_game

'/opt/my_game' - it's specific folder. If you wish to change it: please don't fogot change this in everywhere

3. Change jury configs in folder /opt/my_game/jury.d/conf.d
4. Change docker-compose paths /opt/my_game/docker-compose
5. 

4. Configure dependencies for checkers in /opt/my_game/docker_jury/Dockerfile
5. Build your docker_jury `cd /opt/my_game/docker_jury && ./build_docker.sh`
6. Start docker compose: `cd /opt/my_game && docker-compose up`

## Usefull comamnd for cleanup

`$ cd /opt/my_game && sudo docker-compose rm`
