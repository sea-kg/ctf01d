FROM debian:9.5

EXPOSE 8080
COPY ./fhq-jury-ad /root/sources
RUN mkdir /usr/share/fhq-jury-ad
COPY ./jury.d /usr/share/fhq-jury-ad/jury.d

RUN apt-get update && apt-get install -y \
    make cmake \
    g++ \
    pkg-config \
    libcurl4-openssl-dev \
    zlibc zlib1g zlib1g-dev \
    libpng-dev \
    default-libmysqlclient-dev

# Fix for building on debian system (mysqlclient library)
RUN ln -s /usr/lib/x86_64-linux-gnu/pkgconfig/mariadb.pc /usr/lib/x86_64-linux-gnu/pkgconfig/mysqlclient.pc

RUN cd /root/sources && ./clean.sh
RUN cd /root/sources && ./build_simple.sh

# remove packages for a building
RUN apt-get remove -y default-libmysqlclient-dev zlib1g-dev libpng-dev pkg-config cmake make g++
RUN apt-get clean
RUN cp /root/sources/fhq-jury-ad /usr/bin/fhq-jury-ad 

CMD fhq-jury-ad start


