# stage 0: build binary
FROM debian:10.0
LABEL "maintainer"="FreeHackQuest Team <freehackquest@gmail.com>"
LABEL "repository"="https://github.com/freehackquest/fhq-jury-ad"

RUN apt-get -y update && apt-get install -y \
    make cmake \
    g++ \
    pkg-config \
    libcurl4-openssl-dev \
    zlibc zlib1g zlib1g-dev \
    libpng-dev \
    default-libmysqlclient-dev

# Fix for building on debian system (mysqlclient library)
# RUN ln -s /usr/lib/x86_64-linux-gnu/pkgconfig/mariadb.pc /usr/lib/x86_64-linux-gnu/pkgconfig/mysqlclient.pc

COPY ./fhq-jury-ad /root/sources
WORKDIR /root/sources
RUN ./clean.sh && ./build_simple.sh

# stage 1: finish
FROM debian:10.0
RUN apt-get -y update \
    && apt-get install -y \
    libcurl4 \
    zlibc \
    zlib1g \
    libpng16-16 \
    libmariadb3 \
    libpthread-stubs0-dev \
    libssl-dev \
    python \
    python-pip \
    python3 \
    python3-pip \
    && pip install requests \
    && pip3 install requests

COPY --from=0 /root/sources/fhq-jury-ad /usr/bin/fhq-jury-ad
COPY ./jury.d /usr/share/fhq-jury-ad/jury.d

EXPOSE 8080
CMD ["fhq-jury-ad","-ef","start"]
