FROM golang:1.9.2-stretch
LABEL "maintainer"="FreeHackQuest Team <freehackquest@gmail.com>"
LABEL "repository"="https://github.com/freehackquest/fhq-jury-ad"

WORKDIR /home/example_service2

EXPOSE 4442

RUN go get github.com/go-sql-driver/mysql
RUN go get github.com/jmoiron/sqlx
RUN go get github.com/gorilla/mux

CMD exec go run server.go

# CMD ["go","run","server.go"]


