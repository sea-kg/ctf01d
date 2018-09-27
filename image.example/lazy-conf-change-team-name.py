#!/usr/bin/env python2
# -*- coding: utf-8 -*-

import sys
import os.path

lct_dir = "/usr/share/hackwars/lazy-conf/team"

if not os.path.exists(lct_dir):
    print("Folder '" + lct_dir + "' - did not found please create it self")
    print("usefull command: 'sudo mkdir -p " + lct_dir + "'")
    print("usefull command: 'sudo chmod 777 " + lct_dir + "'")
    exit(-1)

lct_filename = '/usr/share/hackwars/lazy-conf/team/__init__.py'

if not os.path.exists(lct_filename):
    print("File '" + lct_filename + "' - did not exists")
    print("usefull command: 'sudo touch " + lct_filename + "'")
    print("usefull command: 'sudo chmod 777 " + lct_filename + "'")
    exit(-1)


sys.path.insert(0, r'/usr/share/hackwars/lazy-conf')
import team

if 'name' in team.__dict__.keys():
    print("Current Team Name: '" + team.name + "'")
else:
    print("Current Team Name: ''")

team_name = raw_input("New Team Name: ")

f = open(lct_filename, "w")
f.write("#!/usr/bin/env python2\n")
f.write("# -*- coding: utf-8 -*-\n")
f.write("\n")
f.write("name = '" + team_name + "'\n")
f.close();


from subprocess import call
call(["service", "lazy-conf", "restart"])