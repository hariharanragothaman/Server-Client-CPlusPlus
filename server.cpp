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



    // Finally we need to close the socket descriptors
    close(newSd);
    close(serverSd);

    cout <<"Connection closing.." << endl;
    return 0;



}












