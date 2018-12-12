# Install fhq-jury-ad from dockerhub

You can install and run jury system in docker:

https://hub.docker.com/r/freehackquest/fhq-jury-ad/

Any bugs report or proposal for improvent please sent to
* issues https://github.com/freehackquest/fhq-jury-ad/issues
* or to e-mail: freehackquest@gmail.com

## Download single container

```docker pull freehackquest/fhq-jury-ad:0.1.0```

### Full-sample

**You can look complete example here:**

https://github.com/freehackquest/fhq-jury-ad-jury-example


### Fisrt your need prepare conf.ini and container with your ctfgame

```
$ mkdir -p ~/ctfgame/jury.d/conf.d
$ mkdir -p ~/ctfgame/jury.d/conf.d/service1/
$ mkdir -p ~/ctfgame/jury.d/logs
$ mkdir -p ~/ctfgame/jury.d/html
$ mkdir -p ~/ctfgame/jury.d/servicehub.d
$ mkdir -p ~/ctfgame/docker_jury/
```


### Create and edit the ~/ctfgame/jury.d/conf.d/conf.ini
```
; SERVER

[server]
use_storage = mysql
scoreboard_port = 8080
scoreboard_html_folder = /usr/share/fhq-jury-ad/jury.d/html
scoreboard_random = no
[mysql_storage]
dbhost = ctfgame0_db
dbport = 3306
dbname = fhqjuryad
dbuser = fhqjuryad
dbpass = uW2r0V5kpK

[ram_storage]
dbpath = /usr/share/fhq-jury-ad/jury.d/dbram

[game]
name = Game Example
start = 2018-10-01 04:00:00 ; start time of game (UTC)
end = 2025-10-21 12:30:00 ; end time of game (UTC)
; you can change flag time live (in minutes)
flag_timelive_in_min = 10 ; in minutes

[service1]
enabled = yes
name = Hello1
script_path = ./service1/checker.py
script_wait_in_sec = 5 
time_sleep_between_run_scripts_in_sec = 25

; TEAMS

[team1]
name = Team_1
logo = images/teams/unknown.png
ip_address = 192.168.1.11
active = yes

[team2]
name = Team_2
logo = images/teams/unknown.png
ip_address = 192.168.1.12
active = yes
```

### Create and edit file with checker (example) ~/ctfgame/jury.d/conf.d/service1/checker.py
```
#!/usr/bin/python2
import sys
import math 
import socket
import random
import time
import errno

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
port = 4441
command = sys.argv[2]
f_id = sys.argv[3]
flag = sys.argv[4]

# will be mumble (2) - for test jury
# while True: time.sleep(10);

def put_flag():
    global host, port, f_id, flag
    # try put
    try:
        # print("try connect " + host + ":" + str(port))
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.settimeout(1)
        s.connect((host, port))
        result = s.recv(1024)
        # print(result)
        s.send("put" + "\n")
        result = s.recv(1024)
        s.send(f_id + "\n")
        result = s.recv(1024)
        s.send(flag + "\n")
        result = s.recv(1024)
        s.close()
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
        # print("try connect " + host + ":" + str(port))
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.settimeout(1)
        s.connect((host, port))
        result = s.recv(1024)
        # print(result)
        s.send("get\n")
        result = s.recv(1024)
        s.send(f_id + "\n")
        result = s.recv(1024)
        flag2 = result.strip()
        flag2 = flag2.split("FOUND FLAG: ");
        if len(flag2) == 2:
            flag2 = flag2[1]
        else:
            flag2 = ''
        s.close()
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

### On next step create a dockerfile with requirements for a checkers

~/ctfgame/docker_jury/Dockerfile
```
FROM freehackquest/fhq-jury-ad:0.1.0

# services dependencies
RUN apt-get install -y python python-requests python-pip libssl-dev


EXPOSE 8080

CMD fhq-jury-ad start
```

**And build image:**

```
$ cd ~/ctfgame/docker_jury/
$ sudo docker build --rm=true -t "ctfgame0/fhq-jury-ad:0.1.0" .
```

###  Please create file ~/ctfgame/docker-compose.yml
```
version: '2'

services:
  ctfgame0_db:
    image: mysql:5.7
    volumes:
      - "./dckr_mysql:/var/lib/mysql"
    restart: always
    environment:
      MYSQL_ROOT_PASSWORD: KzhyntJxwt
      MYSQL_DATABASE: fhqjuryad
      MYSQL_USER: fhqjuryad
      MYSQL_PASSWORD: uW2r0V5kpK
    networks:
      - ctf0

  ctfgame0_jury:
    depends_on:
      - ctfgame0_db
    image: ctfgame0/fhq-jury-ad:0.1.0
    volumes:
      - "./jury.d:/usr/share/fhq-jury-ad/jury.d"
    ports:
      - "8080:8080"
    restart: always
    links:
      - "ctfgame0_db"
    networks:
      - ctf0

networks:
  ctf0:
    driver: bridge
```

### So now we can start game:

```
$ cd ~/ctfgame/
$ sudo docker-compose up
```