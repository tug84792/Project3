#include "simpleServer.h"
//An extremely simple server that connects to a given port.
//Once the server is connected to the port, it will listen on that port
//for a user connection.
//A user will connect through telnet, the user also needs to know the port number.
//If the user connects successfully, a socket descriptor will be created.
//The socket descriptor works exactly like a file descriptor, allowing us to read/write on it.
//The server will then use the socket descriptor to communicate with the user, sending and
//receiving messages.

#include<string>
#include<fstream>
#include<iostream>
#include<vector>
#include<map>

using namespace std;

vector<int> cSockVector;
vector<string> loggerVector;

#define DEFAULT_PORT 5000
#define NUM_WORKERS 1

string logText  = "log.txt";
string dictionary = "dictionary.txt";

map <string, int> inputWord;

pthread_mutex_t lockForSocket, lockForLogFile;

// This function basically makes up the sentence/indivisual line that will be printed
// to the log file.
string logLine(string inputWord, string outcome){
    string localString = "";
    localString += "CLIENT: " + inputWord + " is " + outcome;
    return localString;
}

// This worker thread function works in the same logic as presented during labs
void* workerThread(void * argument) {
    int cliSock = *((int *) argument), bytesReturned;;

    char *correct = "OK\n";
    char *incorrect = "MISSPELLED\n";
    char recvBuffer[BUF_LEN];
    recvBuffer[0] = '\0';

    pthread_mutex_lock(&lockForSocket);
    cSockVector.push_back(cliSock);
    pthread_mutex_unlock(&lockForSocket);

    while (true) {
        bytesReturned = recv(cliSock, recvBuffer, BUF_LEN, 0);
        if (bytesReturned <= 0)
            break;
    }
}

//In the main function, a lot of the code is the same as given on Canvas. I only added a few lines
//of code that basically establish locks and create threads.
int main(int argc, char** argv) {

    if(argc == 1){
        printf("No port number entered.\n");
        return -1;
    }
    //sockaddr_in holds information about the user connection.
    //We don't need it, but it needs to be passed into accept().
    struct sockaddr_in client;
    int clientLen = sizeof(client);
    int connectionPort = atoi(argv[1]);
    int connectionSocket, clientSocket, bytesReturned;
    char recvBuffer[BUF_LEN];
    recvBuffer[0] = '\0';

    connectionPort = atoi(argv[1]);

    //We can't use ports below 1024 and ports above 65535 don't exist.
    if(connectionPort < 1024 || connectionPort > 65535){
        printf("Port number is either too low(below 1024), or too high(above 65535).\n");
        return -1;
    }

    //Does all the hard work for us.
    connectionSocket = open_listenfd(connectionPort);
    if(connectionSocket == -1){
        printf("Could not connect to %s, maybe try another port number?\n", argv[1]);
        return -1;
    }

    //accept() waits until a user connects to the server, writing information about that server
    //into the sockaddr_in client.
    //If the connection is successful, we obtain A SECOND socket descriptor.
    //There are two socket descriptors being used now:
    //One by the server to listen for incoming connections.
    //The second that was just created that will be used to communicate with
    //the connected user.
    printf("Enter ctrl + c to end the program\n");
    while(true){

        if((clientSocket = accept(connectionSocket, (struct sockaddr*)&client,
                                  reinterpret_cast<socklen_t *>(&clientLen))) == -1){
            printf("Error connecting to client.\n");
            return -1;
        }

        printf("Connection success!\n");
        char* clientMessage = "Hello! I hope you can see this.\n";
        char* msgRequest = "Send me some text and I'll respond with something interesting!\nSend the escape key to close the connection.\n";
        char* msgResponse = "I actually don't have anything interesting to say...but I know you sent ";
        char* msgPrompt = ">>>";
        char* msgError = "I didn't get your message. ):\n";
        char* msgClose = "Goodbye!\n";

        //send()...sends a message.
        //We specify the socket we want to send, the message and it's length, the
        //last parameter are flags.
        send(clientSocket, clientMessage, strlen(clientMessage), 0);
        send(clientSocket, msgRequest, strlen(msgRequest), 0);


        //Begin sending and receiving messages.
        while(1){
            send(clientSocket, msgPrompt, strlen(msgPrompt), 0);
            //recv() will store the message from the user in the buffer, returning
            //how many bytes we received.
            bytesReturned = recv(clientSocket, recvBuffer, BUF_LEN, 0);

            //Check if we got a message, send a message back or quit if the
            //user specified it.
            if(bytesReturned == -1){
                send(clientSocket, msgError, strlen(msgError), 0);
            }
                //'27' is the escape key.
            else if(recvBuffer[0] == 27){
                send(clientSocket, msgClose, strlen(msgClose), 0);
                close(clientSocket);
                break;
            }
            else{
                send(clientSocket, msgResponse, strlen(msgResponse), 0);
                send(clientSocket, recvBuffer, bytesReturned, 0);
                //This line will send it back to the server, it also clears the old buffer
                //fflush(recvBuffer);
                write(1, recvBuffer, bytesReturned);
            }
        }
    }
}
