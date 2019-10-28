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

### 1. In first you need create `service.conf` file for jury system

For example:

```
services.service_ZxjQMahnoK.enabled = yes
services.service_ZxjQMahnoK.name = Your Server Name
services.service_ZxjQMahnoK.script_path = ./checker.py
services.service_ZxjQMahnoK.script_wait_in_sec = 5
services.service_ZxjQMahnoK.time_sleep_between_run_scripts_in_sec = 15
```
where service_ZxjQMahnoK must be unique inside a game config

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


