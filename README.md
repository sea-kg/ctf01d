# fhq-jury-ad

[![Build Status](https://travis-ci.org/freehackquest/fhq-jury-ad.svg?branch=master)](https://travis-ci.org/freehackquest/fhq-jury-ad)

Jury System for a attack-defence ctf game.
Or you can use for training.


* [CHECKER_INFO: description](docs/CHECKER_INFO.md)
* [API: here](docs/SIMILAR.md)
* [TRAINING MODE: lazy-start](docs/LAZY_START.md)
* [SIMILAR: systems](docs/SIMILAR.md)

### Download and basic configuration

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

* [BUILD: Ubuntu/Debian](docs/BUILD_UBUNTU.md)
* [BUILD: MacOS](docs/BUILD_MACOS.md)

### Configure database

* [MYSQL DATABASE: create](docs/STORAGE_MYSQL.md)

After configure database options here:

```
$ nano ~/fhq-jury-ad.git/jury.d/conf.d/conf.ini
```

### Prepare to start

Previously data-flags will be clean

```
$ cd ~/fhq-jury-ad.git/fhq-jury-ad
$ ./fhq-jury-ad clean
```

### Run jury-ad

```
$ ./jury-ad start
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
$ curl http://192.168.1.10:8080/flag?teamid=1&flag=123e4567-e89b-12d3-a456-426655440000
```

http-code responses:

 * 400 - wrong parameters
 * 200 - flag accept
 * 403 - flag not accept (reason: old, already accepted, not found)

