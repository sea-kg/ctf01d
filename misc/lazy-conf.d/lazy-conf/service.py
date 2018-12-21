#!/usr/bin/env python2
# -*- coding: utf-8 -*-
 
import socket
import threading
import sys
import math
import re
import os
import errno
import json

# include custom library
sys.path.insert(0, r'/usr/share/hackwars/lazy-conf')
import team

host = ""
port = 17345
thrs = []

print(team.name)

conf_response = {
    'lazy-conf': 'v1',
    'team_name': team.name,
}

mutex = threading.Lock()

class Connect(threading.Thread):
    def __init__(self, sock, addr):
        self.sock = sock
        self.addr = addr
        self.bKill = False
        threading.Thread.__init__(self)
    def run (self):
        global mutex
        ip_address = self.addr[0]

        self.sock.send(json.dumps(conf_response).encode())
        buf = self.sock.recv(1024)
        print(buf)
        buf = str(buf).strip()
        if buf != '':
            params = json.loads(buf)
            print(json.dumps(params))
            mutex.acquire()
            try:
                print("Recieve jury new configurations: ")
                print("Got host = " + params["host"])
                print("Got port = " + str(params["port"]))
                print("Got teamnum = " + str(params["teamnum"]))
                print("Got host_by_conn = " + ip_address)
                print("Save to file " + os.getcwd() + "/jury/__init__.py")
                
                if ip_address != params["host"]:
                    print("Warn: network is masked or wronf ip address!")

                with open("jury/__init__.py", "w") as f:
                    f.write("#!/usr/bin/env python2\n")
                    f.write("# -*- coding: utf-8 -*-\n")
                    f.write("\n")
                    f.write("host = \"" + params["host"] + "\"\n")
                    f.write("port = " + str(params["port"]) + "\n")
                    f.write("teamnum = " + str(params["teamnum"]) + "\n")
                    f.write("host_by_conn = \"" + ip_address + "\"\n")
                    
            finally:
                mutex.release()
        else:
            print(ip_address + " - stranger...")

        self.bKill = True
        self.sock.close()
        thrs.remove(self)
    def kill(self):
        if self.bKill == True:
            return
        self.bKill = True
        self.sock.close()
        # thrs.remove(self)
 
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
s.bind((host, port))
s.listen(10)
        
try:
    while True:
        sock, addr = s.accept()
        thr = Connect(sock, addr)
        thrs.append(thr)
        thr.start()
except KeyboardInterrupt:
    print('Bye! Write me letters!')
    s.close()
    for thr in thrs:
        thr.kill()
    
