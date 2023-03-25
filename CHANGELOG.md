# Changelog

All notable changes to ctf01d project will be documented in this file.
 
The format is based on [Keep a Changelog](http://keepachangelog.com/)
and this project adheres to [Semantic Versioning](http://semver.org/).

## [v0.4.5] - ??

* Removed python2 from ctf01d docker image
* Added `nano` and `vim` to ctf01d docker image
* Fixed init default configs on first-start container
* Implemented command `ctf01d teams search <keys>`
* Added file `ctf01d-store/all-stores.json`
* Added file `ctf01d-store/teams-examples.json`
* Fixed read all from pipe of checker

## [v0.4.4] - 2020-09-22 (2020 Sep 22)

### Added

* Copied script fhqjad-store from fhq-server
* Fixed #52 added 'ctf01d teams list'
* Added to main find workdir authomaticly
* Fixed #53 added 'ctf01d services list' 

### Changed

* Updated README.md
* Updated wsjcpp-core to v0.2.1
* Updated wsjcpp-arguments to v0.2.1


## [v0.4.3] - 2020-09-12 (2020 Sep 12)

### Added

- Migrated to wsjcpp system c++ packages managment
- Installed wsjcpp arguments
- Updated wsjcpp-core and installed original nlohmann_json package
- Added unit test for service_costs_and_statistics
- Added 'for developers' to readme
- Implemented '-db-host hostname' parameter
- Prepare Dockerfiles for ctf01d-stage-build / ctf01d-stage-release
- Implemented script 'game-simulation/ctf01d-assistent.py' with jury / service1_py / service2_go
- Implemented game-simulation/vulnbox/service3_php
- Create EmployFlags, EmployScoreboard. 

### Changed

- Updated version to 0.4.3
- Renamed freehackquest/fhq-jury-ad to sea-kg/ctf01d
- Fixed #41 Renamed fhq-jury-ad to ctf01d
- Updated rules and screen in README.md
- Redesign parse arguments to wsjcpp-arguments model
- Renamed testing to game-simulation
- Added unit test for flag.
- Redesign fill json for statistics by services
- Redesign Config to EmployConfig
- Renamed class Service to Ctf01dServiceDef
- Renamed class Team to Ctf01dTeamDef
- Renamed class Flag to Ctf01dFlag
- Redesign extract sample files and config
- Redesign Dockerfile for ctf01d
- Redesign calculation costs

### Deprecated

Nope

### Removed

- Removed vulnbox
- Removed not need anymore file update_resources.py

### Fixed

- Fixed english translates in README.md file
- Fixed clean flags

### Security

Nope











