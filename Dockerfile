# LABEL "maintainer"="FreeHackQuest Team <freehackquest@gmail.com>"
# LABEL "repository"="https://github.com/freehackquest/fhq-jury-ad"

# stage 0: build binary
FROM freehackquest/debian-10-for-cpp-build:latest

COPY ./fhq-jury-ad /root/sources
WORKDIR /root/sources
RUN ./clean.sh && ./build_simple.sh

# stage 1: finish
FROM freehackquest/debian-10-for-cpp-common:latest

COPY --from=0 /root/sources/fhq-jury-ad /usr/bin/fhq-jury-ad
COPY ./jury.d /usr/share/fhq-jury-ad/jury.d

EXPOSE 8080
CMD ["fhq-jury-ad","-ef","start"]
