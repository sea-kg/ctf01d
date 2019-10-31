FROM golang:1.9.2-stretch
LABEL "maintainer"="FreeHackQuest Team <freehackquest@gmail.com>"
LABEL "repository"="https://github.com/freehackquest/fhq-jury-ad"

WORKDIR /go/src/app

COPY ./src/ /go/src/app

# Better use a localfolders
RUN go get github.com/go-sql-driver/mysql
RUN go get github.com/jmoiron/sqlx
RUN go get github.com/gorilla/mux

EXPOSE 4202

CMD exec go run server.go

# CMD ["go","run","server.go"]


