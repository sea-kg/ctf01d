## MySQL Database  (server.use_storage = mysql)

### Install requirements

```
$ sudo apt install mysql-server
$ sudo apt install mysql-client
```

### Create database

Run console-mysql-client
```
$ mysql -u root -p
```
Create database and user:

```
> CREATE DATABASE fhqjuryad CHARACTER SET utf8 COLLATE utf8_general_ci;
> CREATE USER 'fhqjuryad'@'localhost' IDENTIFIED BY 'fhqjuryad';
> GRANT ALL PRIVILEGES ON fhqjuryad.* TO 'fhqjuryad'@'localhost' WITH GRANT OPTION;
> FLUSH PRIVILEGES;
```

Tables will be create on first start app
