#ifndef PROJECT3_SIMPLESERVER_H
#define PROJECT3_SIMPLESERVER_H
#include <sys/types.h>
#include <sys/socket.h>
#include <cstring>
#include <string>
#include <netinet/in.h>
#include <cstdio>
#include <unistd.h>
#include <cstdlib>
#define BUF_LEN 512
#include <fstream>
//moved "using namespace std;" to after this header in order to avoid a warning.
#include <iostream>
using namespace std;
#include <vector>
#include <map>
#include <arpa/inet.h>

int open_listenfd(int);

string dictionary = "dictionary.txt";
string logText  = "log.txt";

//Map will be used for easier iterations and vectors will be used as a storage data structure
map <string, int> inputWord;
vector<int> cSockVector;
//Socket mutex
pthread_mutex_t lockForSocket;

//This function is quite simple. It loads the dictionary text file into the program.
//It is then placed into a data structure for further comparison. It will be used for both simpleServer
//and simpleClient.
void initDictionary(void){
    //Bring in file pointer and read it in
    fstream dictFP;
    string localString;
    dictFP.open(dictionary, ios::in);

    //Grab each word line by line and stored in localString
    if(dictFP.is_open()){
        while(getline(dictFP, localString)){
            inputWord[localString] = 1;
        }
    }
    //Close up the dictionary file
    dictFP.close();
}

#endif //PROJECT3_SIMPLESERVER_H
