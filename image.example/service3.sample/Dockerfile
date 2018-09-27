FROM python:latest
MAINTAINER Evgenii Sopov <mrseakg@gmail.com>

RUN mkdir /root/flags && chmod 777 /root/flags
COPY service.py /root/service.py

EXPOSE 4443

CMD cd /root/ && python2 service.py


