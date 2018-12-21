FROM python:latest
MAINTAINER Evgenii Sopov <mrseakg@gmail.com>

RUN mkdir /root/team && chmod 777 /root/team && mkdir /root/jury && chmod 777 /root/jury
COPY service.py /root/service.py
COPY team/__init__.py /root/team/__init__.py
COPY jury/__init__.py /root/jury/__init__.py

EXPOSE 17345

CMD cd /root/ && python2 service.py


