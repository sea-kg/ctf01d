# LABEL "maintainer"="Ecgenii Sopov <mrseakg@gmail.com>"
# LABEL "repository"="https://github.com/freehackquest/ctf01d"

# stage 0: build binary
FROM freehackquest/debian-10-for-cpp-build:latest

COPY ./ /root/
WORKDIR /root/
RUN ./clean.sh && ./build_simple.sh

WORKDIR /root/unit-tests.wsjcpp
RUN ./unit-tests

# stage 1: finish
FROM freehackquest/debian-10-for-cpp-common:latest

COPY --from=0 /root/ctf01d /usr/bin/ctf01d

EXPOSE 8080
CMD ["ctf01d","-work-dir","/usr/share/ctf01d","--extract-files","start"]
