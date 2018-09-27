# Prepare Image Sample

## Use a docker-compose

### Configure your Team Name before build images:

```
$ sudo nano team/__init__.py
```

## Build images and run

Require: 
* docker
* docker-compose

```
$ sudo mkdir -p /usr/share/hackwars
$ sudo ./build_images.sh
$ sudo docker-compose up
```

### Remove
```
$ sudo remove_images.sh
```

## Use a systemctl (for manual virtual machine configuring)

### Install:
```
$ cd ./system_d/
$ sudo ./install.sh
```

### Configure your team name

```
$ sudo nano /usr/share/hackwars/lazy-conf/team/__init__.py
$ systemctl restart lazy-conf.service
```

### Check the working service

```$ sudo systemctl status service1```

### Uninstall:
```
$ cd ./system_d/
$ sudo ./uninstall.sh
```



