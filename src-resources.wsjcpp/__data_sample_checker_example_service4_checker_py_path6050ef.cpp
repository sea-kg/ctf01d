// automaticly generated by wsjcpp:v0.5.3

#include "__data_sample_checker_example_service4_checker_py_path6050ef.h"

REGISTRY_WSJCPP_RESOURCE_FILE(RES___data_sample_checker_example_service4_checker_py_path6050ef)

const std::string &RES___data_sample_checker_example_service4_checker_py_path6050ef::getFilename() const {
    static const std::string s = "./data_sample/checker_example_service4/checker.py";
    return s;
}
// ---------------------------------------------------------------------

const std::string &RES___data_sample_checker_example_service4_checker_py_path6050ef::getPackAs() const {
    static const std::string s = "text";
    return s;
}
// ---------------------------------------------------------------------

int RES___data_sample_checker_example_service4_checker_py_path6050ef::getBufferSize() const {
    return 2813;
}

// ---------------------------------------------------------------------

const char *RES___data_sample_checker_example_service4_checker_py_path6050ef::getBuffer() const {
    static const std::string sRet = "" // size: 2813
        "#!/usr/bin/python2\n"
        "import sys\n"
        "import socket\n"
        "import errno\n"
        "\n"
        "# put-get flag to service success\n"
        "def service_up():\n"
        "    print(\"[service is worked] - 101\")\n"
        "    exit(101)\n"
        "\n"
        "# service is available (available tcp connect) but protocol wrong could not put/get flag\n"
        "def service_corrupt():\n"
        "    print(\"[service is corrupt] - 102\")\n"
        "    exit(102)\n"
        "\n"
        "# waited time (for example: 5 sec) but service did not have time to reply\n"
        "def service_mumble():\n"
        "    print(\"[service is mumble] - 103\")\n"
        "    exit(103)\n"
        "\n"
        "# service is not available (maybe blocked port or service is down)\n"
        "def service_down():\n"
        "    print(\"[service is down] - 104\")\n"
        "    exit(104)\n"
        "\n"
        "if len(sys.argv) != 5:\n"
        "    print(\"\\nUsage:\\n\\t\" + sys.argv[0] + \" <host> (put|check) <flag_id> <flag>\\n\")\n"
        "    print(\"Example:\\n\\t\" + sys.argv[0] + \" \\\"127.0.0.1\\\" put \\\"abcdifghr\\\" \\\"123e4567-e89b-12d3-a456-426655440000\\\" \\n\")\n"
        "    print(\"\\n\")\n"
        "    exit(0)\n"
        "\n"
        "host = sys.argv[1]\n"
        "port = 4444\n"
        "command = sys.argv[2]\n"
        "f_id = sys.argv[3]\n"
        "flag = sys.argv[4]\n"
        "\n"
        "# will be mumble (2) - for test jury\n"
        "# while True: time.sleep(10);\n"
        "\n"
        "def put_flag():\n"
        "    global host, port, f_id, flag\n"
        "    # try put\n"
        "    try:\n"
        "        # print(\"try connect \" + host + \":\" + str(port))\n"
        "        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)\n"
        "        s.settimeout(1)\n"
        "        s.connect((host, port))\n"
        "        s.recv(1024)\n"
        "        s.send(\"put\" + \"\\n\")\n"
        "        s.recv(1024)\n"
        "        s.send(f_id + \"\\n\")\n"
        "        s.recv(1024)\n"
        "        s.send(flag + \"\\n\")\n"
        "        s.recv(1024)\n"
        "        s.close()\n"
        "    except socket.timeout:\n"
        "        service_down()\n"
        "    except socket.error as serr:\n"
        "        if serr.errno == errno.ECONNREFUSED:\n"
        "            service_down()\n"
        "        else:\n"
        "            print(serr)\n"
        "            service_corrupt()\n"
        "    except Exception as e:\n"
        "        print(e)\n"
        "        service_corrupt()\n"
        "\n"
        "def check_flag():\n"
        "    global host, port, f_id, flag\n"
        "    # try get\n"
        "    flag2 = \"\"\n"
        "    try:\n"
        "        # print(\"try connect \" + host + \":\" + str(port))\n"
        "        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)\n"
        "        s.settimeout(1)\n"
        "        s.connect((host, port))\n"
        "        s.recv(1024)\n"
        "        s.send(\"get\\n\")\n"
        "        s.recv(1024)\n"
        "        s.send(f_id + \"\\n\")\n"
        "        result = s.recv(1024)\n"
        "        flag2 = result.strip()\n"
        "        flag2 = flag2.split(\"FOUND FLAG: \");\n"
        "        if len(flag2) == 2:\n"
        "            flag2 = flag2[1]\n"
        "        else:\n"
        "            flag2 = ''\n"
        "        s.close()\n"
        "    except socket.timeout:\n"
        "        service_down()\n"
        "    except socket.error as serr:\n"
        "        if serr.errno == errno.ECONNREFUSED:\n"
        "            service_down()\n"
        "        else:\n"
        "            print(serr)\n"
        "            service_corrupt()\n"
        "    except Exception as e:\n"
        "        print(e)\n"
        "        service_corrupt()\n"
        "\n"
        "    if flag != flag2:\n"
        "        service_corrupt()\n"
        "\n"
        "\n"
        "if command == \"put\":\n"
        "    put_flag()\n"
        "    check_flag()\n"
        "    service_up()\n"
        "\n"
        "if command == \"check\":\n"
        "    check_flag()\n"
        "    service_up()"
    ;
    return sRet.c_str();
} //::buffer() 


