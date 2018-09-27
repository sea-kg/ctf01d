#!/bin/bash

# backup
BACKUP_DIR="`pwd`/backups"
echo "BACKUP_DIR = $BACKUP_DIR"

if [ ! -d $BACKUP_DIR ]; then
  mkdir $BACKUP_DIR
fi

DBNAME=fhqjuryad
DBUSER=fhqjuryad
DBPASS=fhqjuryad

# dump mysql  
if [ -d $1 ]; then
  MYSQLDUMP_SQL="$BACKUP_DIR/fhqjuryad_sql_`date +%Y%m%d-%H%M%S`.sql"
  MYSQLDUMP_TAR_GZ="$BACKUP_DIR/fhqjuryad_sql_`date +%Y%m%d-%H%M%S`.tar.gz"
  echo "MYSQLDUMP_SQL = $MYSQLDUMP_SQL"
  echo "MYSQLDUMP_TAR_GZ = $MYSQLDUMP_TAR_GZ"
  mysqldump -u$DBUSER \
  -p$DBPASS \
  $DBNAME \
  > $MYSQLDUMP_SQL
  tar -zcvf $MYSQLDUMP_TAR_GZ $MYSQLDUMP_SQL
  rm $MYSQLDUMP_SQL
fi
