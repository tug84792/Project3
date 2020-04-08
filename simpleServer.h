//
// Created by Harith Siddiqui on 4/8/20.
//

#ifndef PROJECT3_SIMPLESERVER_H
#define PROJECT3_SIMPLESERVER_H
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#define BUF_LEN 512
int open_listenfd(int);

#endif //PROJECT3_SIMPLESERVER_H
