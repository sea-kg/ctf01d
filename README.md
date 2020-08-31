# ctf01d

[![Build Status](https://travis-ci.org/freehackquest/ctf01d.svg?branch=master)](https://travis-ci.org/freehackquest/ctf01d) [![Docker Pulls](https://img.shields.io/docker/pulls/sea5kg/ctf01d.svg)](https://hub.docker.com/r/sea5kg/ctf01d/) [![Docker layers](https://images.microbadger.com/badges/image/sea5kg/ctf01d.svg)](https://microbadger.com/images/freehackquest/ctf01d) [![Github Stars](https://img.shields.io/github/stars/freehackquest/ctf01d.svg?label=github%20%E2%98%85)](https://github.com/freehackquest/ctf01d/) [![Github Stars](https://img.shields.io/github/contributors/freehackquest/ctf01d.svg)](https://github.com/freehackquest/ctf01d/) [![Github Forks](https://img.shields.io/github/forks/freehackquest/ctf01d.svg?label=github%20forks)](https://github.com/freehackquest/ctf01d/)

Jury System for attack-defence ctf game  (ctf-scoreboard).
Or you can use it for training.

![scoreboard](https://raw.githubusercontent.com/freehackquest/ctf01d/master/misc/screens/screen1.png)

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
- the flag was successfully putted into the service
- the flag has existed on the service all the time of it's life
- the flag was not stolen by another team(s)
- and the service was in UP state throughout the life of the flag

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
- the flag from the same type of service as yours, but your service must be in UP state
- the flag is dealt for the first time by your team (the same flag may be dealt by different teams)
- the flag is still alive (the flag has not expired)
- only during the announced game (flags are not accepted during coffee breaks)

RU:

Засчитывается флаг атаки, если:
- флаг имеет правильный формат
- флаг не принадлежит вашей команде (не с вашего сервиса)
- флаг с того же типа сервиса что и ваш, но ваш сервис должен быть в состоянии UP
- флаг сдается первый раз вашей командой (может сдаваться разными командами один и тот же флаг)
- флаг еще жив (не закончилось время жизни флага)
- только во время объявленной игры (во время кофебрейка флаги не принимаются)

### 4. How to calculate costs of the flags

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

## Easy way to start (and init) docker-compose

docker-compose.yml
```
version: '3'

services:
  ctf01d_db:
    image: mysql:5.7
    volumes:
      - "./tmp/mysql_database:/var/lib/mysql"
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
    image: sea5kg/ctf01d:latest
    volumes:
      - "./data_sample:/usr/share/ctf01d"
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

And than:

```
$ docker-compose up
```

After first start look in './data' folder:
- You will find basic configuration
- After change of the configuration you need to restart 'docker-compose'

### Download and basic configuration

```
$ sudo apt install git-core
$ cd ~
$ git clone http://github.com/freehackquest/ctf01d.git ctf01d.git
$ nano ~/ctf01d.git/jury.d/config.yml
```
Config files (see comments in file):
* `~/ctf01d.git/jury.d/config.yml` - main config


* [BUILD: Ubuntu/Debian](https://github.com/freehackquest/ctf01d/blob/master/docs/BUILD_UBUNTU.md)
* [BUILD: Docker](https://github.com/freehackquest/ctf01d/blob/master/docs/BUILD_DOCKER.md)

### Prepare to start with clearing all previous data

Previously created data-flags will be clean

```
$ cd ~/ctf01d.git/ctf01d
$ ./ctf01d clean
```

### Run ctf01d

```
$ ./ctf01d -wd ../jury.d/ start
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
* mumble - waited for a time (for example: for 5 sec), but service hasn't replied
* shit - checker does not return correct response code

## Acceptance of the flag

* Acceptance of the flag: http://{HOST}:{PORT}/flag?teamid={TEAMID}&flag={FLAG}

Where

* {HOST} - host or ip, where jury system started
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

 * 101 - service is up  (worked fine)
 * 102 - service is corrupt
 * 103 - service is mumbled (or checker works endlessly)
 * 104 - service is down
 * other - checker is shit


 # Jury API requests list

 
 * `http://{HOST}:{PORT}/api/v1/game` - info about game
 * `http://{HOST}:{PORT}/api/v1/teams` - list of teams
 * `http://{HOST}:{PORT}/api/v1/services` - list of services
 * `http://{HOST}:{PORT}/api/v1/scoreboard` - scoreboard table teams-services
 * `http://{HOST}:{PORT}/team-logo/{TEAMID}` - list of teams


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

## Preapre checker

### 1. Firstly you need to add `config.yml` file to the 'checkers' section for a jury system

For example:

```
checkers:
  - id: "service_ZxjQMahnoK" # work directory will be checker_example_service4
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
FROM freehackquest/ctf01d:latest

# TODO add some packages if needs

CMD ["ctf01d","-ef","start"]
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

 * 101 - "service is up" (worked fine)
 * 102 - "service is corrupt" (something wrong with the service)
 * 103 - "service is mumbled" (or checker works endlessly)
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
$ git clone https://github.com/freehackquest/fhq-jury-ad ~/ctf01d.git
```

Build
```
$ cd ~/ctf01d.git
$ ./build_simple.sh
```

Start
```
$ cd ~/ctf01d.git
$ mkdir data_test
$ ./ctf01d -work-dir ./data_test -mysql-host localhost start
```

# Similar Systems && Helpful Links

### SibirCTF - Attack-Defence ctf system

https://github.com/KevaTeam/ctf-attack-defense

Basic Technology: python

### FAUST CTF - Attack-Defence ctf system

https://github.com/fausecteam/ctf-gameserver

### In CTF - Attack-Defence ctf system

https://github.com/inctf/inctf-framework

### RuCTFe - Attack-Defence ctf system 

https://github.com/hackerdom/checksystem

### Tin foil hat (?) - Attack-Defence ctf system

https://github.com/jollheef/tin_foil_hat

### floatec - Attack-Defence ctf system

https://github.com/floatec/attack-defense-CTF-demo

### udinIMM - Attack-Defence ctf system

https://github.com/udinIMM/attack-defense-ctf

### Google - Attack-Defence ctf system

https://github.com/google/ctfscoreboard

### hackthearch (ruby) - Attack-Defence ctf system

https://github.com/mcpa-stlouis/hack-the-arch
