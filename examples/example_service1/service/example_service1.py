#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import socket
import threading
import sys
import math
import re
import os
import errno
import sys
import psutil

host = ""
port = 4101
thrs = []
parent_dir = os.path.dirname(os.path.abspath(__file__))
dir_flags = parent_dir + '/flags/'

if len(sys.argv) != 2:
    print("Usage " + sys.argv[0] + " [start/clean/stop/run]")
    exit(-1)

arg1 = sys.argv[1]


if arg1 == "clean":
    lst_flags = [f for f in os.listdir(dir_flags) if os.path.isfile(os.path.join(dir_flags, f))]
    for fname in lst_flags:
        print('Remove file ' + dir_flags + '/' + fname)
        os.remove(dir_flags + '/' + fname)
    exit(0)
elif arg1 == "stop":
    # find and kill service
    for proc in psutil.process_iter():
        # check whether the process name matches
        command = ' '.join(proc.cmdline())
        if proc.name() == 'python3' and 'service1_test.py run' in command:
            proc.kill()
    exit(0)
elif arg1 == "start":
    os.system('cd ' + parent_dir + ' && python3 ./service1_test.py run > /dev/null &')
    exit(0)
elif arg1 == "status":
    status = 'Not running'
    for proc in psutil.process_iter():
        # check whether the process name matches
        command = ' '.join(proc.cmdline())
        if proc.name() == 'python3' and 'service1_test.py run' in command:
            print(proc)
            status = 'Service running pid: ' + str(proc.pid)
    print(status)
    exit(0)
elif arg1 == "run":
    print("Running service. Listen " + str(port))
else:
    print("Usage " + sys.argv[0] + " [start/clean/stop/run]")
    exit(-1)


class Connect(threading.Thread):
    def __init__(self, sock, addr):
        self.sock = sock
        self.addr = addr
        self.bKill = False
        threading.Thread.__init__(self)
    def run (self):
        help_s = """
Welcome to service1!!!
Commands: put, get, delete, list, close
> """
        # ptrn = re.compile(r""".*(?P<name>\w*?).*""", re.VERBOSE)
        self.sock.send(help_s.encode())
        # self.sock.send(help_s)
        while True:
            if self.bKill == True:
                break
            buf = self.sock.recv(1024).decode("utf-8").strip()
            if buf == "":
                break
            # print(buf)
            command = re.search( r"\w*", buf).group()
            if command == "close":
                resp = "\nBye-bye\n\n"
                self.sock.send(resp.encode())
                break
            elif command == "list":
                files_len = 0
                for f in os.listdir(dir_flags):
                    resp = "file: " + f + "\n"
                    files_len += 1
                    self.sock.send(resp.encode())
                if files_len == 0:
                    self.sock.send("no files".encode())
                break
            elif command == "put":
                resp = "flag_id = "
                self.sock.send(resp.encode())
                f_id = self.sock.recv(1024).decode("utf-8").strip()
                if f_id == "":
                    break
                orig_flag_id = f_id
                f_id = re.search( r"\w*", f_id).group()
                if f_id == "" or f_id != orig_flag_id:
                    resp = "\nFAIL: Incorrect flag_id\n"
                    self.sock.send(resp.encode())
                    break
                resp = "flag = "
                self.sock.send(resp.encode())
                f_text = self.sock.recv(1024).decode("utf-8").strip()
                f_text = f_text
                if f_text == "":
                    break
                f = open(dir_flags + f_id, 'w')
                f.write(f_text)
                self.sock.send("OK".encode())
                f.close()
                break
            elif command == "get":
                resp = "flag_id = "
                self.sock.send(resp.encode())
                f_id = self.sock.recv(1024).decode("utf-8").strip()
                if f_id == "":
                    break
                orig_flag_id = f_id
                f_id = re.search( r"\w*", f_id).group()
                if f_id == "" or f_id != orig_flag_id:
                    resp = "\nFAIL: Incorrect flag_id\n"
                    self.sock.send(resp.encode())
                    break
                if os.path.exists(dir_flags + f_id):
                    f = open(dir_flags + f_id, 'r')
                    line = f.readline()
                    f.close()
                    resp = "FOUND FLAG: " + line + ""
                    self.sock.send(resp.encode())
                else:
                    resp = "\nFAIL: flag_id not found\n"
                    self.sock.send(resp.encode())
                break
            elif command == "delete":
                resp = "flag_id = "
                self.sock.send(resp.encode())
                f_id = self.sock.recv(1024).decode("utf-8").strip()
                if f_id == "":
                    break
                orig_flag_id = f_id
                f_id = re.search( r"\w*", f_id).group()
                if f_id == "" or f_id != orig_flag_id:
                    resp = "\nFAIL: Incorrect flag_id\n"
                    self.sock.send(resp.encode())
                    break
                if os.path.exists(dir_flags + f_id):
                    os.remove(dir_flags + f_id)
                    resp = "REMOVED"
                    self.sock.send(resp.encode())
                else:
                    resp = "\nFAIL: flag_id not found\n"
                    self.sock.send(resp.encode())
                break
            elif command:
                resp = "\n ["+ command + "] unknown command\n\n"
                self.sock.send(resp.encode())
                break
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

if not os.path.exists(dir_flags):
    os.makedirs(dir_flags)
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
