#!/usr/bin/env python
# -*- coding: utf-8 -*-

import requests
import sys
import time
import uuid
import random

host = "localhost"
port = 8080
teamid = "t01"

i = 0
while i < 1000000000:
    i = i + 1
    teamid = random.randint(1, 20)
    teamid = 't' + str(teamid).rjust(2, '0')

    print(teamid)
    flag = str(uuid.uuid4())
    flag = flag[4:]
    flag = 'c01d' + flag
    flag = flag[:-8]
    flag += str(random.randint(1, 99999999)).rjust(8, '0')

    http_get_url = 'http://' + host + ':' + str(port) + '/flag?teamid=' + str(teamid) + '&flag=' + flag
    print("Request " + http_get_url)
    r = requests.get(http_get_url)
    if r.status_code == 200:
        print("OK")
    elif r.status_code == 400:
        print("FAIL " + r.text + "\n" + flag)
        sys.exit(1)
    elif r.status_code == 403:
        print("FAIL " + flag + " " + r.text)
    else:
        print("Wrong params " + str(r.status_code))
    # time.sleep(0.05)
