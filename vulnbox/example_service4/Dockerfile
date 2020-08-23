FROM python:latest
LABEL "maintainer"="FreeHackQuest Team <freehackquest@gmail.com>"
LABEL "repository"="https://github.com/freehackquest/ctf01d"

RUN mkdir /root/flags && chmod 777 /root/flags
COPY service.py /root/service.py

EXPOSE 4444

CMD cd /root/ && python2 service.py


