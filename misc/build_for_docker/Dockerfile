FROM debian:9.5

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

CMD cd /root/sources && ./clean.sh && ./build_simple.sh && mkdir -p debian/9.5 && mv fhq-jury-ad debian/9.5/fhq-jury-ad


