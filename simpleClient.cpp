#include "simpleServer.h"

//Here I create a client thread, which is fairly similar to the workerThread over in simpleServer.cpp
void* clientThread(void * argument){

    //NOTE: For this file in general, much of the logic is used to help set things up for
    //the client. This logic was discussed in the textbook, as well as the video uploaded by Dr. Kwatny.
    socklen_t sizeOfServerAddress;
    int connectionValue,sentValue, receivedValue, socket_desc;
    struct sockaddr_in addressOfServer;

    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == - 1){
        printf("Could not create socket");
    }
    //Here I append the current socket to the socket descriptor queue,
    //with protection of a lock
    pthread_mutex_lock(&lockForSocket);
    cSockVector.push_back(socket_desc);
    pthread_mutex_unlock(&lockForSocket);

    //Here I initialize server address settings Configure settings of the server address.
    //Various data structures to help initialize
    addressOfServer.sin_family = AF_INET;
    addressOfServer.sin_addr.s_addr = inet_addr("127.0.0.1");
    addressOfServer.sin_port = htons(5000);
    memset(addressOfServer.sin_zero, '\0', sizeof addressOfServer.sin_zero);

    //Here, I will link the socket to the server via the address variable addressOfServer
    sizeOfServerAddress = sizeof addressOfServer;
    //Again use built-in funcitons/provided code
    connectionValue = connect(socket_desc, (struct sockaddr *) &addressOfServer, sizeOfServerAddress);
    //Error checking
    if (connectionValue < 0){
        cout << "CONNECTION ERROR" << endl;
        return reinterpret_cast<void *>(1);
    }
    puts("CONNECTED");

    string message;
    map<string, int>::iterator looper;
    char boundedBuffer[BUF_LEN];


    }