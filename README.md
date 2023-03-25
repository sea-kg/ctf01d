# ctf01d

[![Docker Pulls](https://img.shields.io/docker/pulls/sea5kg/ctf01d.svg)](https://hub.docker.com/r/sea5kg/ctf01d/) [![Github Stars](https://img.shields.io/github/stars/sea-kg/ctf01d.svg?label=github%20%E2%98%85)](https://github.com/sea-kg/ctf01d/) [![Github Stars](https://img.shields.io/github/contributors/sea-kg/ctf01d.svg)](https://github.com/sea-kg/ctf01d/) [![Github Forks](https://img.shields.io/github/forks/sea-kg/ctf01d.svg?label=github%20forks)](https://github.com/sea-kg/ctf01d/) 

Jury System for attack-defence ctf game (ctf-scoreboard).
Also you can use it for training.

![scoreboard](https://raw.githubusercontent.com/sea-kg/ctf01d/master/misc/screens/screen1.png)

## Easy way to start/init it (based on docker-compose)

Requirements:
- docker 
- docker-compose

And two terminals (command lines):
- `terminal0` - will be run docker-compose
- `terminal1` - will configure our game


### terminal0

Download or upgrade to the latest version
```
docker pull sea5kg/ctf01d:latest
```

Create a folder with your game:
```
$ mkdir ~/my-first-game
$ cd ~/my-first-game
```

Create a `~/my-first-game/docker-compose.yml` file with the following content:
```
version: '3'

services:
  ctf01d_db:
    image: mysql:5.7
    volumes:
      - "./mysql_database:/var/lib/mysql"
    restart: always
    environment:
      MYSQL_ROOT_PASSWORD: KzhyntJxwt
      MYSQL_DATABASE: ctf01d
      MYSQL_USER: ctf01d
      MYSQL_PASSWORD: ctf01d
    networks:
      - ctf01d_net

  ctf01d_jury:
    depends_on:
      - ctf01d_db
    container_name: ctf01d_jury_my_game
    image: sea5kg/ctf01d:latest
    volumes:
      - "./data_game:/usr/share/ctf01d"
    environment:
      CTF01D_WORKDIR: "/usr/share/ctf01d"
      CTF01D_MYSQL_HOST: "ctf01d_db"
      CTF01D_MYSQL_DATABASE: "ctf01d"
      CTF01D_MYSQL_USER: "ctf01d"
      CTF01D_MYSQL_PASSWORD: "ctf01d"
    ports:
      - "8080:8080"
    restart: always
    links:
      - "ctf01d_db"
    networks:
      - ctf01d_net

networks:
  ctf01d_net:
    driver: bridge
```

And then:

```
$ docker-compose up
```

After successful start, you will see logs similar to the following:
```
ctf01d_jury_1  | 2021-05-17 03:05:29.680, 0x00007f10cd69b700 [WARN] Checker: another_some   example_service2 :  => service is down 
ctf01d_jury_1  | 2021-05-17 03:05:29.680, 0x00007f10cd69b700 [INFO] Checker: another_some   example_service2 : Elapsed milliseconds: 106ms
ctf01d_jury_1  | 2021-05-17 03:05:29.684, 0x00007f10ce69d700 [WARN] Checker: another_some   example_service1 :  => service is down 
ctf01d_jury_1  | 2021-05-17 03:05:29.684, 0x00007f10cde9c700 [WARN] Checker: so_some        example_service1 :  => service is down 
ctf01d_jury_1  | 2021-05-17 03:05:29.685, 0x00007f10cde9c700 [INFO] Checker: so_some        example_service1 : Elapsed milliseconds: 105ms
ctf01d_jury_1  | 2021-05-17 03:05:29.685, 0x00007f10cce9a700 [WARN] Checker: so_some        example_service2 :  => service is down 
ctf01d_jury_1  | 2021-05-17 03:05:29.685, 0x00007f10cce9a700 [INFO] Checker: so_some        example_service2 : Elapsed milliseconds: 109ms
ctf01d_jury_1  | 2021-05-17 03:05:29.685, 0x00007f10ce69d700 [INFO] Checker: another_some   example_service1 : Elapsed milliseconds: 110ms
ctf01d_jury_1  | 2021-05-17 03:05:29.685, 0x00007f10bf7fe700 [WARN] Checker: another_some   example_service3 :  => service is down 
ctf01d_jury_1  | 2021-05-17 03:05:29.686, 0x00007f10bf7fe700 [INFO] Checker: another_some   example_service3 : Elapsed milliseconds: 110ms
ctf01d_jury_1  | 2021-05-17 03:05:29.690, 0x00007f10bdffb700 [WARN] Checker: so_some        example_service3 :  => service is down 
ctf01d_jury_1  | 2021-05-17 03:05:29.690, 0x00007f10bdffb700 [INFO] Checker: so_some        example_service3 : Elapsed milliseconds: 111ms
ctf01d_jury_1  | 2021-05-17 03:05:29.694, 0x00007f10bcff9700 [WARN] Checker: another_some   example_service4 :  => service is down 
ctf01d_jury_1  | 2021-05-17 03:05:29.694, 0x00007f10bcff9700 [INFO] Checker: another_some   example_service4 : Elapsed milliseconds: 108ms
ctf01d_jury_1  | 2021-05-17 03:05:29.694, 0x00007f10affff700 [WARN] Checker: so_some        example_service4 :  => service is down 
ctf01d_jury_1  | 2021-05-17 03:05:29.695, 0x00007f10affff700 [INFO] Checker: so_some        example_service4 : Elapsed milliseconds: 107ms
```
*And you can also find dashboard on http://localhost:8080/*


### terminal1

In the new terminal/console we can change default configuration to the one we need.

Attach to a running container with a bash command line:
```
$ docker exec -it -w /root ctf01d_jury_my_game bash
root@df281aedde7d:~# ctf01d version
ctf01d v0.4.5
```

Now we can use some commands from `ctf01d`

For example, list of commands in the default config:
```
root@df281aedde7d:~# ctf01d teams ls
...
Teams:
 - another_some 
     name: Another Some
     ip-address: 127.0.1.1
     logo: /usr/share/ctf01d/html/images/teams/unknown.svg

 - so_some 
     name: So Some
     ip-address: 127.0.0.1
     logo: /usr/share/ctf01d/html/images/logo.png

```

Search for a predefined team in the teams-store (will download control files from different sources first):
```
root@df281aedde7d:~# ctf01d teams search neos
Found teams: 
team id='neosfun'; name: 'NeosFun'
```

In the future... I suppose that I will implement a command (something like `ctf01d teams install neosfun`) to simplify the configuration of the team-list

TODO

### Go back to `terminal0`

We need to restart docker-compose to re-read configuration.

So now press `Ctrl+C` to stop docker-compose.

And then start it again:
```
$ docker-compose up
```

## Rules

### 1. Basic

flag_timelive_in_min:
  - EN: flag lifetime (default: 1 minutes)
  - RU: время жизни флага (поумолчанию: 1 минут)

basic_costs_stolen_flag_in_points:
  - EN: Basic cost of stolen flag (default: 10 points)
  - RU: Базовая стоимость украденного флага (по умолчанию: 10 поинтов)

### 2. Acception of the defence flag / Принятие флага защиты 

EN:

Only the defence flag from the service is counted if:
- the flag was successfully put into the service
- the flag existed in the service throughout its lifetime 
- the flag was not stolen by other team(s)
- and the service was in UP state throughout the flag lifetime

RU:

Засчитываются только тот флаг защиты с сервиса, если:
- флаг был успешно запулен на сервис
- флаг просуществовал на сервисе все время своей жизни
- флаг не был украден другой командой (командами)
- и в течении всей жизни флага - сервис был в состоянии UP


### 3. Acception of the attack flag / Принятия флага атаки

EN:

The attack flag counts if:
- the flag has the correct format
- the flag does not belong to your team (not from your service)
- the flag from the same type of the service as yours, but your service must be in UP state
- the flag is dealt by your team for the first time (the same flag can be dealt by different teams)
- the flag is still alive (the flag has not expired)
- only during the game (flags are not accepted during coffee breaks)

RU:

Засчитывается флаг атаки, если:
- флаг имеет правильный формат
- флаг не принадлежит вашей команде (не с вашего сервиса)
- флаг с того же типа сервиса что и ваш, но ваш сервис должен быть в состоянии UP
- флаг сдается первый раз вашей командой (может сдаваться разными командами один и тот же флаг)
- флаг еще жив (не закончилось время жизни флага)
- только во время объявленной игры (во время кофебрейка флаги не принимаются)

### 4. How the flag cost is calculated

EN:
- The cost is different for the defended flag and the stolen flag (difference in the number of teams)
- Redistribute the cost for flags between services depending on the number of passed and defended flags
- The cost of flags should not depend on the "place" in the scoreboard table
- The cost of the flags should be different as services may have different delays between flag forwarding
- The cost of flags should be different since the complexity of the services is different

RU:
- Стоимость разная для защищенного флага и украденного флага (разница в количестве команд)
- Перераспределять стоимость за флаги между сервисами - в зависимости от количества сданных и защищенных флагов
- Стоимость флагов не должна зависить от "места" в рейтинговой таблице
- Стоимость флагов должна быть разной так как сервисы могут иметь разное время между забросами флагов
- Стоимость флагов должна быть разной так как сложность сервисов разная

Example on python:

``` python
basic_costs_stolen_flag_in_points = 10
services = [
    { "stolen_flags": 100, "defended_flags": 9 }, # service0 
    { "stolen_flags": 0, "defended_flags": 0 }, # service1
    { "stolen_flags": 12, "defended_flags": 50 }, # service2
    { "stolen_flags": 1, "defended_flags": 0 }, # service3
    { "stolen_flags": 0, "defended_flags": 10 } # service4
]
teams = 10 # number of teams
all_sf = 0 # number of all flags which was stolen
all_df = 0 # number of all flags which was defended
for s in services:
    all_sf = all_sf + s["stolen_flags"]
    all_df = all_df + s["defended_flags"]
print("all_sf=" + str(all_sf) + ", all_df=" + str(all_df))

sp4d = len(services) * basic_costs_stolen_flag_in_points
dp4d = len(services) * (teams - 1) * basic_costs_stolen_flag_in_points
print("sp4d=%d, dp4d=%d" % (sp4d, dp4d))

sf_rsum = 0
df_rsum = 0

# calcululate reverse proportional df_rsum and sf_rsum
for s in services:
    if s["stolen_flags"] > 0:
        s["rpsf"] = float(all_sf / s["stolen_flags"])
    else:
        s["rpsf"] = float(all_sf / 1.0)

    if s["defended_flags"] > 0:
        s["rpdf"] = float(all_df / s["defended_flags"])
    else:
        s["rpdf"] = float(all_df / 1.0)

    sf_rsum = sf_rsum + s["rpsf"]
    df_rsum = df_rsum + s["rpdf"]

# calculate flag costs
i = 0
for s in services:
    cost_attack_flag = 0
    if sf_rsum == 0:
        cost_attack_flag = sp4d
    else:
        cost_attack_flag = sp4d * (s["rpsf"] / sf_rsum)
        
    cost_defence_flag = 0
    if df_rsum == 0:
        cost_defence_flag = dp4d
    else:
        cost_defence_flag = dp4d * (s["rpdf"] / df_rsum)
    print("service%d: cost_attack_flag=%.2f, cost_defence_flag=%.2f" % (i, cost_attack_flag, cost_defence_flag))
    i = i + 1
```


### Download and basic configuration

```
$ sudo apt install git-core
$ cd ~
$ git clone http://github.com/sea-kg/ctf01d.git ctf01d.git
$ nano ~/ctf01d.git/data_sample/config.yml
```
Config files (see comments in file):
* `~/ctf01d.git/data_sample/config.yml` - one config

### Prepare to start with clearing all previous data

Previously created data-flags will be cleared

```
$ cd ~/ctf01d.git/
$ ./ctf01d -work-dir ./data_sample/ clean
```

### Run ctf01d

```
$ cd ~/ctf01d.git/
$ ./ctf01d -work-dir ./data_sample/ start
```

## Scoreboard

url: http://{HOST}:{PORT}/

Where

* {HOST} - host or ip, where jury system started
* {PORT} - configured scoreboard/flag port of the jury system


### Service statuses

* up - the flag putting/checking into the service is successful
* down - service is not available (maybe blocked port or service is down)
* corrupt - service is available (available tcp connection) but it's impossible to put/get the flag
* mumble - wait for a while(for example: for 5 sec), but the service doesn't reply
* shit - checker does not return correct response code

## Acceptance of the flag

* Acceptance of the flag: http://{HOST}:{PORT}/flag?teamid={TEAMID}&flag={FLAG}

Where

* {HOST} - host or ip at which the jury is available 
* {PORT} - configured scoreboard/flag port of the jury system
* {TEAMID} - number, your unique teamid (see scoreboard)
* {FLAG} - uuid, so that the jury knows that this is a flag from an enemy server

Example of sending a flag (via curl):

```
$ curl http://192.168.1.10:8080/flag?teamid=keva&flag=123e4567-e89b-12d3-a456-426655440000
```

http-code responses:

 * 400 - wrong parameters
 * 200 - flag is accepted
 * 403 - flag is not accepted (probable reasons: old, already accepted, not found)


# Checker script description

### Checker console interface

Usage: ```./checker.py <ip_address> <command> <flag_id> <flag> ```

Where:

  * ip_address - address of a machine with this service
  * command - command, can be "put" or "check"
  * flag_id - string (10), id of the flag [a-zA-Z0-9]{10}
  * flag - uuid, value of the flag [a-z0-9]{8}-[a-z0-9]{4}-[a-z0-9]{4}-[a-z0-9]{4}-[a-z0-9]{12}

Commands:

 * `put` - put the flag to the service
 * `check` - check the flag on the service

Call-examples:

 * ```./checker.py 127.0.0.1 put "1q2w3e4r5t" "6a331fd2-133a-4713-9587-12652d34666d"```
 * ```./checker.py 127.0.0.1 check "1q2w3e4r5t" "6a331fd2-133a-4713-9587-12652d34666d"```

### Possible return codes

 * 101 - service is up (work fine)
 * 102 - service is corrupt
 * 103 - service is mumbled (or the checker falls into an endless loop)
 * 104 - service is down
 * other - checker is shit


 # Jury API requests list
 
 * `http://{HOST}:{PORT}/api/v1/game` - info about the game
 * `http://{HOST}:{PORT}/api/v1/teams` - list of teams
 * `http://{HOST}:{PORT}/api/v1/services` - list of services
 * `http://{HOST}:{PORT}/api/v1/scoreboard` - scoreboard table teams-services
 * `http://{HOST}:{PORT}/team-logo/{TEAMID}` - team logos


# How to prepare vuln service

## Build and export docker image

1. Build your docker image

```
$ cd your_service_dirs
$ docker build --file "Dockerfile" --tag "somegame/your_server:0.0.1" .
```

2. Export your server as a "tar" archive (for distribution)

```
$ docker save "somegame/your_server:0.0.1" > somegame-your_server-0.0.1.tar
```

3. Import your server on vulnbox side (for a hacker's team)

```
$ docker load -i ./somegame-your_server-0.0.1.tar
```

## Prepare checker

### 1. Firstly you need to add `config.yml` file to the 'checkers' section for a jury system

For example:

```
checkers:
  - id: "service_ZxjQMahnoK" # work directory will be checker_service_ZxjQMahnoK
    service_name: "Service1"
    enabled: yes
    script_path: "./checker.py"
    script_wait_in_sec: 5 # max time for running script
    time_sleep_between_run_scripts_in_sec: 15 # like a round for service
```
where "service_ZxjQMahnoK" is a UNIQUE id within the game config

Prepare folder and create ./checker.py:

```
$ mkdir checker_service_ZxjQMahnoK
$ touch checker_service_ZxjQMahnoK/checker.py
$ chmod +x checker_service_ZxjQMahnoK/checker.py
```

### 2. Prepare checker script

You can write checker in any language, but you need to install expected requirements to Dockerfile with jury system

For example Dockerfile.jury with a jury:

```
FROM sea5kg/ctf01d:latest

# TODO add some packages if needs

# CMD already defined in sea5kg/ctf01d image
# CMD ["ctf01d","-work-dir","/root/data","start"]
```

Jury will be call your checker script like `./checker.py <ip_address> <command> <flag_id> <flag> `

Where:

  * ip_address - address of a machine with this service
  * command - command, can be "put" or "check"
  * flag_id - string (10), id of the flag [a-zA-Z0-9]{10}
  * flag - uuid, value of the flag [a-z0-9]{8}-[a-z0-9]{4}-[a-z0-9]{4}-[a-z0-9]{4}-[a-z0-9]{12}

Commands:

 * `put` - put the flag to the service
 * `check` - check the flag on the service

Call-examples:

 * ```./checker.py 127.0.0.1 put "1q2w3e4r5t" "6a331fd2-133a-4713-9587-12652d34666d"```
 * ```./checker.py 127.0.0.1 check "1q2w3e4r5t" "6a331fd2-133a-4713-9587-12652d34666d"```

Allowed return codes:

 * 101 - "service is up" (worke fine)
 * 102 - "service is corrupt" (something wrong with the service)
 * 103 - "service is mumbled" (or the checker falls into an endless loop)
 * 104 - "service is down"
 * any - "checker is shit"


For example checker script (in python):

```
#!/usr/bin/python
import sys
import math
import socket
import random
import time
import errno
import requests

# the flag putting/checking into the service is successful
def service_up():
    print("[service is worked] - 101")
    exit(101)

# service is available (available tcp connection) but it's impossible to put/get the flag
def service_corrupt():
    print("[service is corrupt] - 102")
    exit(102)

# waited for a time (for example: for 5 sec), but service hasn't replied
def service_mumble():
    print("[service is mumble] - 103")
    exit(103)

# service is not available (maybe blocked port or service is down)
def service_down():
    print("[service is down] - 104")
    exit(104)

if len(sys.argv) != 5:
    print("\nUsage:\n\t" + sys.argv[0] + " <host> (put|check) <flag_id> <flag>\n")
    print("Example:\n\t" + sys.argv[0] + " \"127.0.0.1\" put \"abcdifghr\" \"123e4567-e89b-12d3-a456-426655440000\" \n")
    print("\n")
    exit(0)

host = sys.argv[1]
port = 4102
command = sys.argv[2]
f_id = sys.argv[3]
flag = sys.argv[4]

# will be mumbled (2) - for test jury
# while True: time.sleep(10);

def put_flag():
    global host, port, f_id, flag
    # try put
    try:
        r = requests.post('http://' + host + ':' + str(port) + '/api/flags/' + f_id + '/' + flag)
        if r.status_code != 200:
            service_corrupt()
    except socket.timeout:
        service_down()
    except socket.error as serr:
        if serr.errno == errno.ECONNREFUSED:
            service_down()
        else:
            print(serr)
            service_corrupt()
    except Exception as e:
        print(e)
        service_corrupt()

def check_flag():
    global host, port, f_id, flag
    # try get
    flag2 = ""
    try:
        r = requests.get('http://' + host + ':' + str(port) + '/api/flags/' + f_id)
        if r.status_code != 200:
            service_corrupt()
        flag2 = r.json()['Flag']
    except socket.timeout:
        service_down()
    except socket.error as serr:
        if serr.errno == errno.ECONNREFUSED:
            service_down()
        else:
            print(serr)
            service_corrupt()
    except Exception as e:
        print(e)
        service_corrupt()

    if flag != flag2:
        service_corrupt()

if command == "put":
    put_flag()
    check_flag()
    service_up()

if command == "check":
    check_flag()
    service_up()
```

# FOR DEVELOPERS

## Ubuntu 20.04

Prepare database: mysql:

```
$ sudo apt install mysql-server mysql-common mysql-client
$ systemctl start mysql
```

Create database:
```
$ sudo mysql
...
mysql> CREATE DATABASE ctf01d CHARACTER SET utf8 COLLATE utf8_general_ci;
Query OK, 1 row affected, 2 warnings (0.01 sec)

mysql> CREATE USER 'ctf01d'@'localhost' IDENTIFIED BY 'ctf01d';
Query OK, 0 rows affected (0.00 sec)

mysql> GRANT ALL PRIVILEGES ON ctf01d.* TO 'ctf01d'@'localhost' WITH GRANT OPTION;
Query OK, 0 rows affected (0.00 sec)

mysql> FLUSH PRIVILEGES;
Query OK, 0 rows affected (0.01 sec)

mysql> exit;
Bye
```

Install package-requirements

```
sudo apt install git git-core\
    make cmake g++ pkg-config \
    libcurl4-openssl-dev \
    zlibc zlib1g zlib1g-dev \
    libpng-dev \
    default-libmysqlclient-dev
```

Clone source code of the project:
```
$ git clone https://github.com/sea-kg/ctf01d ~/ctf01d.git
```

Build
```
$ cd ~/ctf01d.git
$ ./build_simple.sh
```

*Maybe fix: `$ sudo ln -s /usr/lib/x86_64-linux-gnu/pkgconfig/mariadb.pc /usr/lib/x86_64-linux-gnu/pkgconfig/mysqlclient.pc`*
*More info: (https://bugs.debian.org/cgi-bin/bugreport.cgi?bug=878340)[https://bugs.debian.org/cgi-bin/bugreport.cgi?bug=878340]*

Start
```
$ cd ~/ctf01d.git
$ mkdir data_test
$ ./ctf01d -work-dir ./data_test -db-host localhost start
```

## Build in docker

1. In the first step we prepare docker network:

```
docker network create --driver=bridge ctf01d_net
```

2. Prepare mysql database
In the next step we just start the container with mysql (like a daemon - in background)

```
docker run -d --rm \
    --name ctf01d-mysql \
    -e MYSQL_ROOT_PASSWORD=KzhyntJxwt \
    -e MYSQL_DATABASE=ctf01d \
    -e MYSQL_USER=ctf01d \
    -e MYSQL_PASSWORD=ctf01d \
    --network ctf01d_net \
    mysql:8 \
    mysqld --default-authentication-plugin=mysql_native_password
```

We can look for docker status: `docker ps -a`

3. Prepare docker for builds:

*Notice: multistage build docker*

You need to download latest version of ctf01d-stage-build / ctf01d-stage-release or build it first

Download (docker pull):
```
$ docker pull sea5kg/ctf01d-stage-build
$ docker pull sea5kg/ctf01d-stage-release
```

Or build fresh images for stages:
```
$ cd ~/ctf01d.git/contrib/docker-build-stages/
$ ./build-stages-images.sh
```

You can see them in a list:
```
$ docker images
```

And now you can build image:

```
$ cd ~/ctf01d.git
$ docker build --rm=true -t "sea5kg/ctf01d:latest" .
```

4. Run dev docker-container, build and start

Run:
```
$ cd ~/ctf01d.git 
$ docker run -it --rm \
  -p 8081:8080 \
  -v `pwd`/:/root/ctf01d.dev \
  -w /root/ctf01d.dev \
  --name "ctf01d.dev" \
  --network ctf01d_net \
  sea5kg/ctf01d-stage-build:latest \
  bash
root@604feda3c718:~/ctf01d.dev#
```

Build:
```
root@604feda3c718:~/ctf01d.dev# ./clean.sh
root@604feda3c718:~/ctf01d.dev# ./build_simple.sh
```

Start: 
```
root@604feda3c718:~/ctf01d.dev# ./ctf01d -work-dir ./data_sample/ -db-host ctf01d-mysql start
```

Now you can see scoreboard on http://localhost:8081

# GAME SIMULATION

It's necessary for testing in conditions close to real game 
- 3 teams
- 4 services (written in different languages)
- 5 subnetworks (with masquerade - base on docker network)

requirements:
* `$ pip3 install docker`

Start
```
$ cd ~/ctf01d.git/game-simulation/
$ ./ctf01d-assistent.py start
```

After this command has run successfully, you can look for:
    * Scoreboard - http://localhost:8080
    * team1 - service1_py : `nc 10.10.11.1 4101`
    * team2 - service1_py : `nc 10.10.12.1 4101`
    * team3 - service1_py : `nc 10.10.13.1 4101`
    * team1 - service2_go : http://10.10.11.1:4102
    * team2 - service2_go : http://10.10.12.1:4102
    * team3 - service2_go : http://10.10.13.1:4102

To remove all images containers and networks:
```
$ cd ~/ctf01d.git/game-simulation/
$ ./ctf01d-assistent.py clean
```

# SPECIAL THANKS

* Danil Dudkin

# Online Attack-Defence

I have only one schmea now:

![schema1](https://raw.githubusercontent.com/sea-kg/ctf01d/master/misc/schemas/basic_schema_masquerade_openvpn.png)

# Similar Systems && Helpful Links

SibirCTF - Attack-Defence ctf system (python):

https://github.com/KevaTeam/ctf-attack-defense

FAUST CTF - Attack-Defence ctf system:

https://github.com/fausecteam/ctf-gameserver

In CTF - Attack-Defence ctf system:

https://github.com/inctf/inctf-framework

RuCTFe - Attack-Defence ctf system:

https://github.com/hackerdom/checksystem

Tin foil hat (?) - Attack-Defence ctf system:

https://github.com/jollheef/tin_foil_hat

floatec - Attack-Defence ctf system:

https://github.com/floatec/attack-defense-CTF-demo

udinIMM - Attack-Defence ctf system:

https://github.com/udinIMM/attack-defense-ctf

Google - Attack-Defence ctf system:

https://github.com/google/ctfscoreboard

hackthearch (ruby) - Attack-Defence ctf system:

https://github.com/mcpa-stlouis/hack-the-arch

ForcAD - Pure-python distributable Attack-Defence CTF platform, created to be easily set up.

https://github.com/pomo-mondreganto/ForcAD
