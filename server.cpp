/* Simple Server
 */

#include <iostream>
#include <string>
#include <stdio.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>

#include <sys/uio.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <fstream>

using namespace std;

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        cerr << "Usage: port " << endl;
        exit(0);
    }

    // Port number to serve the port
    int port = atoi(argv[1]);
    // buffer to send and receive messages
    char msg[1500];

    // Setup socket related code and connection tools
    sockaddr_in servAddr;
    bzero((char*)&servAddr, sizeof (servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(port);

    // Start a socket
    int serverSd = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSd < 0)
    {
        cerr << "Error establishing the server socket" << endl;
        exit(0);
    }

    // Bind the socket to it's local address
    int bindStatus = bind(serverSd, (struct sockaddr*) &servAddr, sizeof(servAddr));
    if (bindStatus < 0)
    {
        cerr << "Error binding socket to local address" << endl;
        exit(0);
    }

    cout << "Waiting for client to connect ...." << endl;
    listen(serverSd, 5);

    // Receive a request from client using accept - new address to connect with client
    sockaddr_in newSockAddr;
    socklen_t newSockAddrSize = sizeof (newSockAddr);

    // Accept the connection
    int newSd = accept(serverSd, (sockaddr *)&newSockAddr, &newSockAddrSize);
    if(newSd < 0)
    {
        cerr << "Error accepting request from client" << endl;
        exit(1);
    }

    cout << "Connected with client!" << endl;

    // Tracking session time
    struct timeval start1, end1;
    gettimeofday(&start1, NULL);
    int bytesRead, bytesWritten = 0;

    while(1)
    {
        cout << "Awaiting client response ...." << endl;
        memset(&msg, 0, sizeof (msg)); // Clear the Buffer
        bytesRead += recv(newSd, (char*)&msg, sizeof (msg), 0);

        if(!strcmp(msg, "exit"))
        {
            cout <<"Client has quit the session" << endl;
            break;
        }
        cout << "Client: " << msg << endl;
        cout << ">";
        string data;
        getline(cin, data);
        memset(&msg, 0, sizeof(msg)); //clear the buffer
        strcpy(msg, data.c_str());
        if(data == "exit")
        {
            //send to the client that server has closed the connection
            send(newSd, (char*)&msg, strlen(msg), 0);
            break;
        }
        //send the message to client
        bytesWritten += send(newSd, (char*)&msg, strlen(msg), 0);


    }

    // Finally we need to close the socket descriptors
    close(newSd);
    close(serverSd);
    cout << "********Session********" << endl;
    cout << "Bytes written: " << bytesWritten << " Bytes read: " << bytesRead << endl;
    cout << "Elapsed time: " << (end1.tv_sec - start1.tv_sec)
         << " secs" << endl;
    cout <<"Connection closing.." << endl;
    return 0;



}












