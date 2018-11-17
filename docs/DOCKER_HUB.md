# Install fhq-jury-ad from dockerhub

You can install and run jury system in docker:

https://hub.docker.com/r/freehackquest/fhq-jury-ad/


## docker-compose.yml

```
version: '2'

services:
  gameExampleDb:
    image: mysql:5.7
    volumes:
      - "/opt/game-example/docker_db/mysql:/var/lib/mysql"
    restart: always
    environment:
      MYSQL_ROOT_PASSWORD: root_pass
      MYSQL_DATABASE: fhqjuryad
      MYSQL_USER: fhqjuryad
      MYSQL_PASSWORD: fhqjuryadpass
    networks:
      - gameExampleNet

  gameExampleJury:
    depends_on:
      - gameExampleDb
    image: freehackquest/fhq-jury-ad:latest
    volumes:
      - "/opt/game-example/jury.d:/usr/share/fhq-jury-ad/jury.d"
    ports:
      - "8080:8080"
    restart: always
    links:
      - "gameExampleDb"
    environment:
      JURY_DB_HOST: gameExampleDb
      JURY_DB_PORT: 3306
      JURY_DB_NAME: fhqjuryad
      JURY_DB_USER: fhqjuryad
      JURY_DB_PASSWORD: fhqjuryadpass
    networks:
      - gameExampleNet

volumes:
  jury.d:

networks:
  gameExampleNet:
    driver: bridge
```

