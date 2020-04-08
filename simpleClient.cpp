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

    //This for loop has same logic as in the worker thread. Keeps going until it has reached the end.
    for(looper = inputWord.begin(); looper != inputWord.end(); looper++){

        message = (*looper).first;
        //keep adding new line
        message += "\n";

        //Again use built-in funcitons/provided code
        sentValue = send(socket_desc, message.c_str(), message.length(), 0);
        //Error checking
        if (sentValue < 0){
            puts("SEND FAILED");
            return reinterpret_cast<void *>(1);
        }
        puts("DATA SENT");

        receivedValue = recv(socket_desc, boundedBuffer, BUF_LEN, 0);
        if (receivedValue < 0){
            puts("RECV FAILED");
            return reinterpret_cast<void *>(1);
        }
        puts("REPLY RECEIVED\n");
        puts(boundedBuffer);
    }

    //Here I will iterate through the whole queue of sockets, protecting this data structure using a mutex
    pthread_mutex_lock(&lockForSocket);
    //Iterator named positionC, C for this being the client file
    auto positionC = cSockVector.end();
    //looperH for this file's looper specifically
    for(auto looperH = cSockVector.begin(); looperH != cSockVector.end(); looperH++){
        //If at the end
        if(socket_desc == *looperH){
            positionC = looperH;
            break;
        }
    }
    //Get rid of last element, and release the lock.
    cSockVector.erase(positionC);
    pthread_mutex_unlock(&lockForSocket);

    //Close out the client and end the thread
    close(socket_desc);
    pthread_exit(nullptr);
}

