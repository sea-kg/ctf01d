# fhq-jury-ad

Jury System for a attack-defence ctf game

## Download and basic configuration

```
$ sudo apt install git-core
$ cd ~ 
$ git clone http://github.com/freehackquest/jury.git fhq-jury-ad.git
$ cd fhq-jury-ad.git
$ sudo mkdir /usr/share/fhq-jury-ad
$ sudo ln -s `pwd`/jury.d /usr/share/fhq-jury-ad/jury.d
$ cp ~/fhq-jury-ad.git/jury.d/conf.d/conf.ini.sample ~/fhq-jury-ad.git/jury.d/conf.d/conf.ini
$ nano ~/fhq-jury-ad.git/jury.d/conf.d/conf.ini
```

**Look and edit /usr/share/fhq-jury-ad/jury.d/conf.d/conf.ini** - more information inside conf.inf in comments 

## Build fhq-jury-ad system (Ubuntu)

```
$ sudo apt install cmake make
$ sudo apt install default-libmysqlclient-dev
$ sudo apt install pkg-config
$ sudo apt install libcurl4-openssl-dev
$ sudo apt install zlibc zlib1g zlib1g-dev
$ sudo apt install libpng-dev
$ cd ~/fhq-jury-ad.git/fhq-jury-ad
$ ./clean.sh
$ ./build_simple.sh
```

## Build jury system (Debian)

`libmysqlclient-dev` -> `default-libmysqlclient-dev`


Previously data-flags will be clean

```
$ cd ~/fhq-jury-ad.git/fhq-jury-ad
$ ./fhq-jury-ad clean
```

## Run jury-ad

```
$ ./jury-ad start
```

## Run for testing or for training (Lazy-Start)

1. Start vulnerable images (with service lazy-conf) and configure team name
2. Start ```./fhq-jury-ad lazy-start```

## Scoreboard && Acceptance of flag

* Scoreboard: http://{HOST}:{PORT}/
* Acceptance of flag: http://{HOST}:{PORT}/flag?teamid={TEAMID}&flag={FLAG}

Where 

* {HOST} - host or ip, where jury system started
* {PORT} - scoreboard/flag port, where jury system started
* {TEAMID} - number, your unique teamid (see scoreboard)
* {FLAG} - uuid, so... it's flag from enimy server

Example of send flag (curl):
```
$ curl http://192.168.1.10:8080/flag?teamid=1&flag=123e4567-e89b-12d3-a456-426655440000
```
http-code responses:

 * 400 - wrong prameters
 * 200 - flag accept
 * 403 - flag not accept (reason: old, already accepted, not found)

## MySQL Database  (use_storage is mysql)

### Install requriments

```
$ sudo apt install mysql-server
$ sudo apt install mysql-client
```

### Create database

Run console-mysql-client
```
$ mysql -u root -p
```
Create database and user:

```
> CREATE DATABASE fhqjuryad CHARACTER SET utf8 COLLATE utf8_general_ci;
> CREATE USER 'fhqjuryad'@'localhost' IDENTIFIED BY 'fhqjuryad';
> GRANT ALL PRIVILEGES ON fhqjuryad.* TO 'fhqjuryad'@'localhost' WITH GRANT OPTION;
> FLUSH PRIVILEGES;
```
Create tables:

```
$ mysql -p -u fhqjuryad fhqjuryad < sql/mysql_fhqjuryad.sql
```

## Checker description

### Service statuses

* up - put-check flag to service success
* down - service is not available (maybe blocked port or service is down)
* corrupt - service is available (available tcp connect) but protocol wrong could not put/get flag
* mumble - waited time (for example: 5 sec) but service did not have time to reply
* shit - checker not return correct response code

### Checker console interface

 Usage: ```./checker.py <ip_address> <command> <flag_id> <flag> ```

Where:

  * ip_address - address of machine with this service
  * command - command, can be "put" or "check"
  * flag_id - string (10), id of flag [a-zA-Z0-9]{10}
  * flag - uuid, value of flag [A-Z0-9]{8}-[A-Z0-9]{4}-[A-Z0-9]{4}-[A-Z0-9]{4}-[A-Z0-9]{12}

 * `put` - put flag to service
 * `check` - check flag on service

 * ```./checker.py 127.0.0.1 put "1q2w3e4r5t" "6a331fd2-133a-4713-9587-12652d34666d"```
 * ```./checker.py 127.0.0.1 check "1q2w3e4r5t" "6a331fd2-133a-4713-9587-12652d34666d"```


### Possible return codes

 * 101 - service is up  (worked fine)
 * 102 - service is corrupt
 * 103 - service is mumble
 * 104 - service is down

 ## Jury API requests list

 * `/api/v1/teams` - list of teams
 * `/api/v1/services` - list of services
 * `/api/v1/scoreboard` - scoreboard table teams x services