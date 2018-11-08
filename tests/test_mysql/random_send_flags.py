#!/usr/bin/env python
# -*- coding: utf-8 -*-

import requests
import time
import uuid
import sys

if len(sys.argv) != 5:
    print("Usage: " + sys.argv[0] + " <jury_host> <jury_port> <teamid> <count>")
    exit(0)

host = sys.argv[1]
port = sys.argv[2]
teamnum = sys.argv[3]
count = int(sys.argv[4])

i = 0
while i < count:
    i = i + 1
    flag = str(uuid.uuid4())
    r = requests.get('http://' + host + ':' + str(port) + '/flag?teamid=' + str(teamnum) + '&flag=' + flag)
    if r.status_code == 200:
        print("OK - " + r.text)
    elif r.status_code == 403:
        print("FAIL - " + r.text)
    else:
        print("Wrong params: " + r.text)