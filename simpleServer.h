
#ifndef FINALSPELL_SIMPLESERVER_H
#define FINALSPELL_SIMPLESERVER_H
#include <sys/types.h>
#include <sys/socket.h>
#include <cstring>
#include <netinet/in.h>
#include <cstdio>
#include <unistd.h>
#include <cstdlib>
#define BUF_LEN 512
int open_listenfd(int);
#endif //FINALSPELL_SIMPLESERVER_H
