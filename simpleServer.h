using namespace std;

#ifndef PROJECT3_SIMPLESERVER_H
#define PROJECT3_SIMPLESERVER_H
#include <sys/types.h>
#include <sys/socket.h>
#include <cstring>
#include <netinet/in.h>
#include <cstdio>
#include <unistd.h>
#include <cstdlib>
#define BUF_LEN 512

#include<string>
#include<fstream>
#include<iostream>
#include<vector>
#include<map>
#include <arpa/inet.h>
#include<map>

int open_listenfd(int);

string logText  = "log.txt";
map <string, int> inputWord;
vector<int> cSockVector;
pthread_mutex_t lockForSocket;

#endif //PROJECT3_SIMPLESERVER_H
