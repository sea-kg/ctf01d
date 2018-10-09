FROM debian:9.5
# 

EXPOSE 8080
COPY ./fhq-jury-ad /root/fhq-jury-ad
RUN mkdir /usr/share/fhq-jury-ad

RUN apt update && apt install -y \
     make cmake \
     pkg-config \
     libcurl4-openssl-dev \
     zlibc zlib1g zlib1g-dev \
     libpng-dev

RUN apt install default-libmysqlclient-dev
RUN apt install mysql-server

RUN cd /root/fhq-jury-ad && ./build_simple.sh


CMD /etc/init.d/mysql start && /root/fhq-jury-ad/fhq-jury-ad start


