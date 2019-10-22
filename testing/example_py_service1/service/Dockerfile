FROM python:latest
LABEL "maintainer"="FreeHackQuest Team <freehackquest@gmail.com>"
LABEL "repository"="https://github.com/freehackquest/fhq-jury-ad"

WORKDIR /root
RUN mkdir ./flags && chmod 777 /root/flags
COPY service.py /root/service.py

EXPOSE 4441

CMD cd /root/ && python3 service.py


