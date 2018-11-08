#!/usr/bin/env python
# -*- coding: utf-8 -*-

import requests
import time
import uuid
import os
import subprocess
from os import listdir
from os.path import isfile, join
import psutil
import time
import re
import shutil

logs_path = './logs'

def _cleanup():
    os.system('killall fhq-jury-ad > /dev/null')

    # find and kill service
    os.system('python2 ../service1_test/service1_test.py stop')
    os.system('python2 ../service1_test/service1_test.py clean')

    logs_files = [f for f in listdir(logs_path) if isfile(join(logs_path, f))]
    for fname in logs_files:
        if fname.endswith('.log'):
            print('Remove file ' + logs_path + '/' + fname)
            os.remove(logs_path + '/' + fname)

def _start_service():
    os.system('python2 ../service1_test/service1_test.py start')

def _start_jury():
    os.system('../../fhq-jury-ad/fhq-jury-ad clean -wd ./ > /dev/null &')
    os.system('../../fhq-jury-ad/fhq-jury-ad start -wd ./ > /dev/null &')

# clean folder with logs
_cleanup()
_start_service()
_start_jury()

# wait 5 seconds
time.sleep(5)

## get information about teams and services
jury_host = "localhost"
jury_port = 8080

r = requests.get('http://' + jury_host + ':' + str(jury_port) + '/api/v1/teams')
teams = r.json()

ip_address1 = teams['team1']['ip_address']
ip_address2 = teams['team2']['ip_address']

# find log file by jury
log_file = ''
logs_files2 = [f for f in listdir(logs_path) if isfile(join(logs_path, f))]
for fname in logs_files2:
    if fname.endswith('.log'):
        log_file = logs_path + '/' + fname

with open(log_file) as f:
    content = f.readlines()
# you may also want to remove whitespace characters like `\n` at the end of each line
content = [x.strip() for x in content] 

ptrn = re.compile(r""".*(?P<name>\w*?).*""", re.VERBOSE)

flag_id = ''
flag = ''
for log_line in content:
    if "team1_service1: Start script" in log_line:
        flag_id = re.search( r".* put ([A-Za-z0-9]+) .*", log_line).group(1)
        flag = re.search( r".* put [A-Za-z0-9]+ ([A-Za-z0-9-]+)", log_line).group(1)
        print(log_line)
        print("flag_id: " + flag_id)
        print("flag: " + flag)

def send_flag(flag):
    # try pass flag
    r = requests.get('http://' + jury_host + ':' + str(jury_port) + '/flag?teamid=' + str(2) + '&flag=' + flag)
    if r.status_code == 200:
        print("OK")
        return True
    elif r.status_code == 403:
        print("FAIL - " + r.text)
        return False
    else:
        print("Wrong params")
        return False

send_flag(flag)
time.sleep(2)
send_flag(flag)


_cleanup()