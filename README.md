# C-curriculum-design

## About

Project: Hust C programming language curriculum design 2017 summer.

Name: Logistics Distribution Information Management System

License: `GNU GENERAL PUBLIC LICENSE V3.0`

Developing language: C, Javascript

GUI: HTML page

Communication protocol: websocket

Platform: Linux

Dependencies: openssl

## Usage

```shell
cd bin
sh ./install.sh
sudo ./wsserver
[Your Internet browser] index.html
```

If you have used this system before, you can skip the second step.

You can do `sh /bin/Clear.sh` to clear the database. Of course, you can also do it in GUI.

## Features

This system uses HTML5 GUI, relying on websocket protocol with a javascript client.

The backstage is totally writen by C language, includes a websocket server and three linklist algorithm librarys.

## About GUI

Be sure to do `sh ./install.sh` if you are fresh to this system.

Be sure to launch wsserver before opening the GUI.

Please open index.html.

Don't move Clear.sh.

The GUI is in Chinese !

## About Backstage

All message sent by client or server can be seen in the console.

Error message will appear when something wrong happens.

If the socket was closed by some error, both wsserver and GUI should be restart.

## Here are some explanations

* linklist.h : Head file of part: 'Linklist algorithm library'
* wsserver.h : Head file of part: 'Backstage and server'
* basic.c : Basic functions like insert, modify, delete or locate
* statistics.c : Get informations and calculation functions
* search.c : Comparation and Searching functions
* wsserver.c : Backstage control and websocket server
* Clear.sh : To clear the database

## Other commonts

This project is homework of HUST-CS, for reference only. I do not wish anyone to copy it totally.

Copyright (C) 2017, Pan Yue, zxc479773533@gmail.com

School of Computer Science & Technology, HUST
