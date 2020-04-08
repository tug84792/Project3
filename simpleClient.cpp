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

   }