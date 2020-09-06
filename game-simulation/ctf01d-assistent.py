#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import docker
import sys
import os

# https://docker-py.readthedocs.io/en/latest/
client = docker.from_env()

progname = sys.argv[0]

# TODO read from data/config.yml

teams = [
    {
        "name": "team1",
        "ip_prefix": "10.0.11"
    }, {
        "name": "team2",
        "ip_prefix": "10.0.12"
    }, {
        "name": "team3",
        "ip_prefix": "10.0.13"
    }
]
jury_net_prefix = "10.0.100"
ntwrk_name_prefix = "ctf01d_net_"

services_list = ["service1_py", "service2_go", "service3_php", "service4_cpp"]
img_name_prefix = "ctf01d-game-simulation/"

def printHelp():
    print("\n"
        + progname + " - assistent for manupulate game-simulation\n"
        "commands:\n"
        "\t clean - remove all containers, images, networks and etc...\n"
        "\t start - prepare networks, images, containers and etc \n"
        "\n"
    )

if len(sys.argv) < 2:
    printHelp()
    exit(1)

command = sys.argv[1]

if command == "clean":
    ntwrks = client.networks.list()
    print(ntwrks)
    for n in ntwrks:
        if n.name.startswith(ntwrk_name_prefix):
            n.remove()

    # find and remove images
    imgs = client.images.list()
    for i in imgs:
        tag = ""
        if len(i.tags) > 0:
            tag = i.tags[0]
        if tag.startswith(img_name_prefix):
            client.images.remove(image=tag)
    exit(0)

def createNetwork(network_list, network_name, ip_prefix):
    for n in network_list:
        if n.name == network_name:
            print("Network already exists " + network_name)
            return 
    print("Creating network " + network_name)
    ret = client.networks.create(network_name,
        driver="bridge",
        check_duplicate=True,
        options={
            "gateway": ip_prefix + ".1",
            "subnet": ip_prefix + ".0/24",
            "ip-range": ip_prefix + ".3/24",
        }
    )
    print(" -> Done." + str(ret))

def createTeamAndJuryNetworks():
    print("\n ===> Create networks")
    ntwrks = client.networks.list()
    createNetwork(ntwrks, ntwrk_name_prefix + "jury", jury_net_prefix)
    for t in teams:
        # print(t)
        network_name_team = ntwrk_name_prefix + t['name']
        createNetwork(ntwrks, network_name_team, t['ip_prefix'])

def buildImage(images_list, image_tag, pathWithDockerfile):
    if not os.path.isdir(pathWithDockerfile):
        print(pathWithDockerfile + " - not exists")
        return

    path_dockerfile = pathWithDockerfile + "/Dockerfile"
    if not os.path.isfile(path_dockerfile):
        print(path_dockerfile + " - not exists")
        return

    for i in images_list:
        tag = ""
        if len(i.tags) > 0:
            tag = i.tags[0]
        if tag == image_tag:
            print(image_tag + "Skip. Image already exists.")
            return
    # TODO redesing to command line build
    print("Building image " + image_tag + "...")
    ret = client.images.build(
        path=pathWithDockerfile,
        tag=image_tag,
        forcerm=True # Always remove intermediate containers, even after unsuccessful builds
    )
    print(" -> Done." + str(ret))

def buildJuryAndServiceImages():
    print("\n ===> Docker images for Services")
    imgs = client.images.list()
    # basic image with jury
    buildImage(imgs, "sea5kg/ctf01d:latest", "..")
    for service_name in services_list:
        img_tag = img_name_prefix + service_name + ":latest"
        buildImage(imgs, img_tag, "./vulnbox/" + service_name)

if command == "start":
    createTeamAndJuryNetworks()
    buildJuryAndServiceImages()


        # if n.name.startswith(ntwrk_name_prefix):
        #     n.remove()
# print(client.containers.list())


# ctf01d-game-simulation/service4_cpp:latest

# print(client.images.list())