#!/usr/bin/env python2
# -*- coding: utf-8 -*-

import os
import socket
import netifaces
import json

ip_addr = ""
interfaces = netifaces.interfaces()
for i in interfaces:
    if i == 'lo':
        continue
    iface = netifaces.ifaddresses(i).get(netifaces.AF_INET)
    if iface != None:
        for j in iface:
            if j['netmask'] == '255.255.255.0':
                ip_addr = j['addr']
            else:
                print(j)

print("Current ip_addr: " + ip_addr)

ip_addr_tmpl = ip_addr.split(".")

min_a = 1
max_a = 254

i = min_a

conf_msg = {
    'port': 8080,
    'host': ip_addr,
    'teamnum': 1,
}

while i < max_a:
    ip_addr_tmpl[3] = str(i)
    ip_addr_team = ".".join(ip_addr_tmpl)
    try:
        client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        client.settimeout(0.3)
        client.connect((ip_addr_team, 17345))
        result = client.recv(1024)
        result = json.loads(result)

        if "lazy-conf" not in result:
            client.close()
            print(ip_addr_team + " - FAIL (not found 'lazy-conf' attribute in json response)")
            continue

        if result['lazy-conf'] != "v1":
            client.close()
            print(ip_addr_team + " - FAIL (unsupport 'lazy-conf' version)")
            continue

        if "team_name" not in result:
            client.close()
            print(ip_addr_team + " - FAIL (not found 'team_name' attribute in json response)")
            continue

        team_name = result["team_name"]
        print(ip_addr_team + " - OK  (team" + str(conf_msg['teamnum']) + ": '" + team_name + "') ")

        client.send(json.dumps(conf_msg))
        client.close()
	conf_msg['teamnum'] = conf_msg['teamnum'] + 1

    except Exception as e:
        # print(e)
        print(ip_addr_team + " - FAIL")
    i = i + 1
