### Build fhq-jury-ad

```
$ git clone https://github.com/freehackquest/fhq-jury-ad ~/fhq-jury-ad.git
$ cd ~/fhq-jury-ad.git
$ docker build --rm=true -t "freehackquest/fhq-jury-ad:latest" .
```

### Run docker

```
$ docker run --name=fhq-jury-ad -p 8080:8080 -v ./jury.d:/usr/share/fhq-jury-ad/jury.d freehackquest/fhq-jury-ad:latest
```