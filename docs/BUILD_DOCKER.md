### Build ctf01d

```
$ git clone https://github.com/freehackquest/ctf01d ~/ctf01d.git
$ cd ~/ctf01d.git
$ docker build --rm=true -t "freehackquest/ctf01d:test" .
```

### rebuild and start

```
$ cd ~/ctf01d.git
$ docker build --rm=true -t "freehackquest/ctf01d:test" .
$ docker rm ctf01d # if exists
$ docker run --name=ctf01d -p 8080:8080 -v ./jury.d:/usr/share/ctf01d/data freehackquest/ctf01d:test
```


### Run docker

```
$ cd ~/ctf01d.git
$ docker run --name=ctf01d -p 8080:8080 -v ./jury.d:/usr/share/ctf01d/data freehackquest/ctf01d:test
```

or restart

```
$ docker start -i ctf01d
```
