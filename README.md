# fhq-jury-ad

[![Build Status](https://travis-ci.org/freehackquest/fhq-jury-ad.svg?branch=master)](https://travis-ci.org/freehackquest/fhq-jury-ad) [![Docker Pulls](https://img.shields.io/docker/pulls/freehackquest/fhq-jury-ad.svg)](https://hub.docker.com/r/freehackquest/fhq-jury-ad/) [![Docker layers](https://images.microbadger.com/badges/image/freehackquest/fhq-jury-ad.svg)](https://microbadger.com/images/freehackquest/fhq-jury-ad) [![Github Stars](https://img.shields.io/github/stars/freehackquest/fhq-jury-ad.svg?label=github%20%E2%98%85)](https://github.com/freehackquest/fhq-jury-ad/) [![Github Stars](https://img.shields.io/github/contributors/freehackquest/fhq-jury-ad.svg)](https://github.com/freehackquest/fhq-jury-ad/) [![Github Forks](https://img.shields.io/github/forks/freehackquest/fhq-jury-ad.svg?label=github%20forks)](https://github.com/freehackquest/fhq-jury-ad/)

Jury System for a attack-defence ctf game.
Or you can use for training.

![scoreboard](https://raw.githubusercontent.com/freehackquest/fhq-jury-ad/master/misc/screens/screen1.png)

## Rules

### 1. Basic

flag_timelive_in_min:
  - EN: flag lifetime (default: 1 minutes)
  - RU: время жизни флага (поумолчанию: 1 минут)

basic_costs_stolen_flag_in_points:
  - EN: Basic cost of stolen flag (default: 10 points)
  - RU: Базовая стоимость украденного флага (по умолчанию: 10 поинтов)

### 2. Acception of flag defence / Принятие флага защиты 

EN:

Only that flag defence from the service is counted if:
- the flag was successfully putted to the service
- the flag has existed on the service all the time of it's life
- the flag was not stolen by another team (s)
- And throughout the life of the flag - the service was in UP state

RU:

Засчитываются только тот флаг защиты с сервиса, если:
- флаг был успешно запулен на сервис
- флаг просуществовал на сервисе все время своей жизни
- флаг не был украден другой командой (командами)
- И в течении всей жизни флага - сервис был в состоянии UP


### 3. Acception of flag attack / Принятия флага атаки

EN:

The attack flag counts if:
- the flag has the correct format
- the flag does not belong to your team (not from your service)
- a flag from the same type of service as yours, but your service must be in UP state
- the flag is dealt the first time by your team (the same flag may be dealt by different teams)
- the flag is still alive (the flag has not expired)
- only during the announced game (flags are not accepted during coffeebreaking)

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
- Redistribute the cost for flags between services - depending on the number of passed and defended flags
- The cost of flags should not depend on the "place" in the scoreboard table
- The cost of the flags should be different as services may have different times between flag throws
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

## Easy way docker-compose

docker-compose.yml
```
version: '3'

services:
  fhqjuryad_db:
    image: mysql:5.7
    volumes:
      - "./tmp/mysql_database:/var/lib/mysql"
    restart: always
    environment:
      MYSQL_ROOT_PASSWORD: KzhyntJxwt
      MYSQL_DATABASE: fhqjuryad
      MYSQL_USER: fhqjuryad
      MYSQL_PASSWORD: fhqjuryad
    networks:
      - fhqjuryad_net

  fhqjuryad_jury:
    depends_on:
      - fhqjuryad_db
    image: freehackquest/fhq-jury-ad:test
    volumes:
      - "./data:/usr/share/fhq-jury-ad/jury.d"
    environment:
      CTF01D_WORKDIR: "/usr/share/fhq-jury-ad/jury.d"
    ports:
      - "8080:8080"
    restart: always
    links:
      - "fhqjuryad_db"
    networks:
      - fhqjuryad_net

networks:
  fhqjuryad_net:
    driver: bridge
```

And than:

```
$ docker-compose up
```

After first start just look in './data' folder

### Download and basic configuration

```
$ sudo apt install git-core
$ cd ~
$ git clone http://github.com/freehackquest/fhq-jury-ad.git fhq-jury-ad.git
$ nano ~/fhq-jury-ad.git/jury.d/config.yml
```
Config files (look comments in file):
* `~/fhq-jury-ad.git/jury.d/config.yml` - main config


* [BUILD: Ubuntu/Debian](https://github.com/freehackquest/fhq-jury-ad/blob/master/docs/BUILD_UBUNTU.md)
* [BUILD: Docker](https://github.com/freehackquest/fhq-jury-ad/blob/master/docs/BUILD_DOCKER.md)

### Configure database

* [MYSQL DATABASE: create](https://github.com/freehackquest/fhq-jury-ad/blob/master/docs/STORAGE_MYSQL.md)

After configure database options here:

```
$ nano ~/fhq-jury-ad.git/jury.d/config.yml
```

### Prepare to start with clean all previous data

Previously data-flags will be clean

```
$ cd ~/fhq-jury-ad.git/fhq-jury-ad
$ ./fhq-jury-ad clean
```

### Run fhq-jury-ad

```
$ ./fhq-jury-ad -wd ../jury.d/ start
```

## Scoreboard

url: http://{HOST}:{PORT}/

Where

* {HOST} - host or ip, where jury system started
* {PORT} - scoreboard/flag port, where jury system started


### Service statuses

* up - put-check flag to service success
* down - service is not available (maybe blocked port or service is down)
* corrupt - service is available (available tcp connect) but protocol wrong could not put/get flag
* mumble - waited time (for example: 5 sec) but service did not have time to reply
* shit - checker not return correct response code

## Acceptance of flag

* Acceptance of flag: http://{HOST}:{PORT}/flag?teamid={TEAMID}&flag={FLAG}

Where

* {HOST} - host or ip, where jury system started
* {PORT} - scoreboard/flag port, where jury system started
* {TEAMID} - number, your unique teamid (see scoreboard)
* {FLAG} - uuid, so... it's flag from enemy server

Example of send flag (curl):

```
$ curl http://192.168.1.10:8080/flag?teamid=keva&flag=123e4567-e89b-12d3-a456-426655440000
```

http-code responses:

 * 400 - wrong parameters
 * 200 - flag accept
 * 403 - flag not accept (reason: old, already accepted, not found)



# Checker script description

### Checker console interface

Usage: ```./checker.py <ip_address> <command> <flag_id> <flag> ```

Where:

  * ip_address - address of machine with this service
  * command - command, can be "put" or "check"
  * flag_id - string (10), id of flag [a-zA-Z0-9]{10}
  * flag - uuid, value of flag [a-z0-9]{8}-[a-z0-9]{4}-[a-z0-9]{4}-[a-z0-9]{4}-[a-z0-9]{12}

Commands:

 * `put` - put flag to service
 * `check` - check flag on service

Call-examples:

 * ```./checker.py 127.0.0.1 put "1q2w3e4r5t" "6a331fd2-133a-4713-9587-12652d34666d"```
 * ```./checker.py 127.0.0.1 check "1q2w3e4r5t" "6a331fd2-133a-4713-9587-12652d34666d"```

### Possible return codes

 * 101 - service is up  (worked fine)
 * 102 - service is corrupt
 * 103 - service is mumble (or checker infinity work)
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

2. Export your server as "tar" archive (for distribution)

```
$ docker save "somegame/your_server:0.0.1" > somegame-your_server-0.0.1.tar
```

3. Import your server on vulnbox side (for a hacker's team)

```
$ docker load -i ./somegame-your_server-0.0.1.tar
```

## Preapre checker

### 1. In first you need add to section 'checkers' `config.yml` file for jury system

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
where service_ZxjQMahnoK must be unique inside a game config

Prepare folder and create ./checker.py:

```
$ mkdir checker_service_ZxjQMahnoK
$ touch checker_service_ZxjQMahnoK/checker.py
$ chmod +x checker_service_ZxjQMahnoK/checker.py
```

### 2. Prepare checker script

You can write checker on any language, but you need install expected requirements to Dockerfile with jury system, so

For example Dockerfile.jury with jury:

```
FROM freehackquest/fhq-jury-ad:latest

# TODO add some packages if needs

CMD ["fhq-jury-ad","-ef","start"]
```

Jury will be call you checker script like  `./checker.py <ip_address> <command> <flag_id> <flag> `

Where:

  * ip_address - address of machine with this service
  * command - command, can be "put" or "check"
  * flag_id - string (10), id of flag [a-zA-Z0-9]{10}
  * flag - uuid, value of flag [a-z0-9]{8}-[a-z0-9]{4}-[a-z0-9]{4}-[a-z0-9]{4}-[a-z0-9]{12}

Commands:

 * `put` - put flag to service
 * `check` - check flag on service

Call-examples:

 * ```./checker.py 127.0.0.1 put "1q2w3e4r5t" "6a331fd2-133a-4713-9587-12652d34666d"```
 * ```./checker.py 127.0.0.1 check "1q2w3e4r5t" "6a331fd2-133a-4713-9587-12652d34666d"```

Allowed return codes one of them:

 * 101 - "service is up"  (worked fine)
 * 102 - "service is corrupt" (something worng with service)
 * 103 - "service is mumble" (or checker infinity work)
 * 104 - "service is down"
 * any - "checker is shit"


For example checker script (python):

```
#!/usr/bin/python
import sys
import math 
import socket
import random
import time
import errno
import requests

# put-get flag to service success
def service_up():
    print("[service is worked] - 101")
    exit(101)

# service is available (available tcp connect) but protocol wrong could not put/get flag
def service_corrupt():
    print("[service is corrupt] - 102")
    exit(102)

# waited time (for example: 5 sec) but service did not have time to reply
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

# will be mumble (2) - for test jury
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

# Simular Systems && Helpful Links

## SibirCTF - Attack-Defence ctf system

https://github.com/KevaTeam/ctf-attack-defense

Basic Technology: python

## FAUST CTF - Attack-Defence ctf system

https://github.com/fausecteam/ctf-gameserver

## In CTF - Attack-Defence ctf system

https://github.com/inctf/inctf-framework

## RuCTFe - Attack-Defence ctf system 

https://github.com/hackerdom/checksystem

## Tin foil hat (?) - Attack-Defence ctf system

https://github.com/jollheef/tin_foil_hat

## floatec - Attack-Defence ctf system

https://github.com/floatec/attack-defense-CTF-demo

## udinIMM - Attack-Defence ctf system

https://github.com/udinIMM/attack-defense-ctf

## Google - Attack-Defence ctf system

https://github.com/google/ctfscoreboard

## hackthearch (ruby) - Attack-Defence ctf system

https://github.com/mcpa-stlouis/hack-the-arch
