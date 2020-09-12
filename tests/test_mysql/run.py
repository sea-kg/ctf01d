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
    os.system('killall ctf01d > /dev/null')

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
    os.system('../../ctf01d/ctf01d clean -wd ./ > jury_clean.log')
    os.system('../../ctf01d/ctf01d start -wd ./ > jury_start.log &')

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
teamid = 2
for log_line in content:
    if "team1_service1: Start script" in log_line:
        flag_id = re.search( r".* put ([A-Za-z0-9]+) .*", log_line).group(1)
        flag = re.search( r".* put [A-Za-z0-9]+ ([A-Za-z0-9-]+)", log_line).group(1)
        print(log_line)
        print("flag_id: " + flag_id)
        print("flag: " + flag)

def send_flag(flag):
    # try pass flag
    r = requests.get('http://' + jury_host + ':' + str(jury_port) + '/flag?teamid=' + str(teamid) + '&flag=' + flag)
    if r.status_code == 200:
        print("OK - " + r.text)
        return True
    elif r.status_code == 403:
        print("FAIL - " + r.text)
        return False
    else:
        print("Wrong params: " + r.text)
        return False

send_flag(flag)

count = 1000
n = 0
while n < 250:
    n = n + 1
    os.system('python2 ./random_send_flags.py ' + jury_host + ' ' + str(jury_port) + ' ' + str(teamid) + ' ' + str(count) + ' > /dev/null &')


while (True):
    pids = []
    # find and kill service
    for proc in psutil.process_iter():
        # check whether the process name matches
        command = ' '.join(proc.cmdline())
        if proc.name() == 'python2' and 'random_send_flags.py' in command:
            pids.append(proc.pid)
    if len(pids) == 0:
        break;
    print("Wait finish " + str(len(pids)) + " scripts")
    time.sleep(1)

# _cleanup()