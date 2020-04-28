//g++ -std=c++11 -pthread -o ec simpleClient.cpp -I.

#include "simpleServer.h"
#include<map>
#include<fstream>
#include<iostream>
#include<vector>
#include<string>
#include<fstream>
#include <cstdio>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <pthread.h>

#define PORTNUMBER 5091
#define NUMBEROFTHREADS 10
using namespace std;
//For some reason, and I have noe idea why, but i had to declare everything here. It would not take
//a header file as being common.
int labSocket = 0;
struct sockaddr_in addressOfServer;
pthread_mutex_t mutexLock = PTHREAD_MUTEX_INITIALIZER;

//Here I create a function that the client thread will use to communicate,
//which is fairly similar to the workerThread over in simpleServer.cpp
void* writeToServer(void* argument) {
    //NOTE: For this file in general, much of the logic is used to help set things up for
    //the client. This logic was discussed in the textbook, as well as the video uploaded by Dr. Kwatny.

    //This is the word that I'll be sending to the server.
    char *term = "coach";
    //Begin locking
    pthread_mutex_lock(&mutexLock);

    //Error checking. Once again, this was provided by textbook.
    if ((labSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        pthread_mutex_unlock(&mutexLock);
        return nullptr;
    }

    //Here I initialize server address settings Configure settings of the server address.
    //Various data structures to help initialize
    addressOfServer.sin_family = AF_INET;
    addressOfServer.sin_port = htons(PORTNUMBER);

    // Here I am basically converting text to binary, and error checking. Provided code used again.
    if (inet_pton(AF_INET, "127.0.0.1", &addressOfServer.sin_addr) <= 0) {
        printf("\nThe address is invalid. \n");
        //unlock the lock
        pthread_mutex_unlock(&mutexLock);
        return nullptr;
    }

    //Again use built-in functions/provided code
    if (connect(labSocket, (struct sockaddr *) &addressOfServer, sizeof(addressOfServer)) < 0) {
        //unlock the lock
        pthread_mutex_unlock(&mutexLock);
        return nullptr;
    }
    //Here I send the word "coach". Provided code used again.
    send(labSocket, term, strlen(term), 0);
    char buffer[1024] = {0};

    //Originally I had this line, but then my IDE said it was never used. But when I ran it without this line,
    //it didn't work. Provided code used again.
    int returnedValue = read(labSocket, buffer, 1024);
    //print the word which is the buffer
    printf("%s\n", buffer);
    //unlock the lock
    pthread_mutex_unlock(&mutexLock);
}
int main(int argc, char const *argv[]) {
    //thread pool
    pthread_t groupOfThreads[NUMBEROFTHREADS];
    //number of threads to loop through
    int threadClients[NUMBEROFTHREADS];
    printf("The threads are now being initiated.\n");

    //for loop that actually created the 10 threads
    for (int i = 0; i < NUMBEROFTHREADS; i++) {
        //Here I index the i value and make it each thread's indivisual value
        threadClients[i] = i;
        //Create each thread, write to server for that specific thread.
        pthread_create(&groupOfThreads[i], nullptr, &writeToServer, &threadClients[i]);
    }
    //for loop that will wait for the 10 threads
    for (auto & i : groupOfThreads) {
        pthread_join(i, nullptr);
    }
    printf("The threads have been initiated. Requests sent. \n");
    return 0;
}



