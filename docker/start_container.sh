#!/bin/bash

sudo mkdir -p /usr/share/fhq-jury-ad

sudo docker run --name=fhq-jury-ad -p 8080:8080 -v /usr/share/fhq-jury-ad:/usr/share/fhq-jury-ad fhq-jury-ad/fhq-jury-ad


# sudo docker run -it --name=jury-test debian:9.5 /bin/bash