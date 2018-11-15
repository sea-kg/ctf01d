FROM debian:9.5

EXPOSE 8080
COPY ./fhq-jury-ad/debian/9.5/fhq-jury-ad /usr/bin/fhq-jury-ad
RUN mkdir /usr/share/fhq-jury-ad
COPY ./jury.d /usr/share/fhq-jury-ad/jury.d

RUN apt-get update && apt-get install -y libcurl3 zlibc zlib1g libpng16-16 libmariadbclient18

# Fix for building on debian system (mysqlclient library)
# RUN ln -s /usr/lib/x86_64-linux-gnu/pkgconfig/mariadb.pc /usr/lib/x86_64-linux-gnu/pkgconfig/mysqlclient.pc

CMD fhq-jury-ad start


