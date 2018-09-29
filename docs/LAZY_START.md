# Lazy Start

## What is it?

* It's mode for start training configuration.

## What need?

Any image of team with vulnerable service(s) must have special lazy-conf service.
Every team must configure team name use helper script in home directory: `./lazy-conf-change-team-name.py`

## How it work?

Jury running in "lazy-start" mode. `./fhq-jury-ad lazy-start`

* first time will be scanning network on servers with lazy-conf service
* game start time parameter will be replaced to now
* game end time parameter will be replaced to now + 2 hours
* List of teams will be replaced from result of scanning