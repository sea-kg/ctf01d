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

`libmysqlclient-dev` -> `default-libmysqlclient-dev`