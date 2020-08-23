### Build ctf01d system (Ubuntu)

```
$ sudo apt install cmake make \
    libmysqlclient-dev \
    pkg-config \
    libcurl4-openssl-dev \
    zlibc zlib1g zlib1g-dev \
    libpng-dev \
$ cd ~/ctf01d.git/ctf01d
$ ./clean.sh
$ ./build_simple.sh
```

### Build jury system (Debian)

after 
```
$ sudo apt install libmysqlclient-dev
```

Please apply fix:

```
$ sudo ln -s /usr/lib/x86_64-linux-gnu/pkgconfig/mariadb.pc /usr/lib/x86_64-linux-gnu/pkgconfig/mysqlclient.pc
```

More info: (https://bugs.debian.org/cgi-bin/bugreport.cgi?bug=878340)[https://bugs.debian.org/cgi-bin/bugreport.cgi?bug=878340]