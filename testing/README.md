
# fhq-jury-ad / testing

Run testing with several services and teams inside a docker network

build && run command:

`docker-compose up --build`

## Jury logs:

jury.d/logs


## Links

- Jury Scoreboard / Flags Acceptance: `http://localhost:8080/`

Service 1 (example_py_service1): 
 - team1: localhost:4101
 - team2: localhost:4201
 - team3: localhost:4301

Service 2 (example_go_service2): 
 - team1: localhost:4101
 - team2: localhost:4201
 - team3: localhost:4301
