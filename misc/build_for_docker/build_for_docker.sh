#!/bin/bash

SOURCES=`pwd`/../../fhq-jury-ad

if [[ "$(sudo docker images -q fhq-jury-ad:build-jury 2> /dev/null)" == "" ]]; then
  sudo docker build --rm=true -t "fhq-jury-ad:build-jury" .
fi

if [ ! "$(sudo docker ps -q -f name=build-jury)" ]; then
    if [ "$(sudo docker ps -aq -f status=exited -f name=build-jury)" ]; then
        # cleanup
        sudo docker rm build-jury
    fi
    # echo $SOURCES " -> /root/sources"
    sudo docker run -it --name=build-jury -v "$SOURCES:/root/sources" fhq-jury-ad:build-jury
fi