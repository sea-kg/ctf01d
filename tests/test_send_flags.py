#!/usr/bin/env python
# -*- coding: utf-8 -*-

import requests
import time
import uuid

host = "localhost"
port = 8080
teamnum = 1

i = 0
while i < 1000:
    i = i + 1
    flag = str(uuid.uuid4())
    r = requests.get('http://' + host + ':' + str(port) + '/flag?teamid=' + str(teamnum) + '&flag=' + flag)
    if r.status_code == 200:
        print("OK")
    elif r.status_code == 403:
        print("FAIL")
    else:
        print("Wrong params")
    # time.sleep(0.05)
