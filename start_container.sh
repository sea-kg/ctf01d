#!/bin/bash

if [[ "$(sudo docker images -q freehackquest/fhq-jury-ad:latest 2> /dev/null)" == "" ]]; then
    echo "Not found image freehackquest/fhq-jury-ad. Please build it"
    exit;
fi

if [ ! "$(sudo docker ps -q -f name=fhq-jury-ad)" ]; then
    if [ "$(sudo docker ps -aq -f status=exited -f name=fhq-jury-ad)" ]; then
        sudo docker start -i fhq-jury-ad
        exit;
    fi
    sudo docker run --name=fhq-jury-ad -p 8080:8080 -v `pwd`/jury.d:/usr/share/fhq-jury-ad/jury.d freehackquest/fhq-jury-ad:latest
fi
