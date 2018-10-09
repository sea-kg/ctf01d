#!/bin/bash

# sudo mkdir -p /usr/share/fhq-jury-ad/
cd ../
sudo docker rmi "fhq-jury-ad:fhq-jury-ad"
sudo docker build --rm=true -t "fhq-jury-ad:fhq-jury-ad" .
