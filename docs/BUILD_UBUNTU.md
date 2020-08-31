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