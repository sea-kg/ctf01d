#!/bin/bash

if [ ! -d "./fhq-jury-ad/debian/9.5/fhq-jury-ad" ]; then
    cd misc/build_for_docker
    ./build_for_docker.sh
    cd ../../    
fi

sudo docker rmi "freehackquest/fhq-jury-ad:0.1.0"
sudo docker build --rm=true -t "freehackquest/fhq-jury-ad:0.1.0" .