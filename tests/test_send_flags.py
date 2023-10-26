#!/usr/bin/env python
# -*- coding: utf-8 -*-

import requests
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
    r = requests.get('http://' + host + ':' + str(port) + '/flag?teamid=' + str(teamid) + '&flag=' + flag)
    if r.status_code == 200:
        print("OK")
    elif r.status_code == 403:
        print("FAIL " + flag)
    else:
        print("Wrong params")
    # time.sleep(0.05)
