### Build fhq-jury-ad system (Ubuntu)

```
$ sudo apt install cmake make
$ sudo apt install libmysqlclient-dev
$ sudo apt install pkg-config
$ sudo apt install libcurl4-openssl-dev
$ sudo apt install zlibc zlib1g zlib1g-dev
$ sudo apt install libpng-dev
$ cd ~/fhq-jury-ad.git/fhq-jury-ad
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