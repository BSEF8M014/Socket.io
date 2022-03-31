#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
int main()
{
    using namespace std;
    // Create a socket
        /*
        domain = 2  AF_INET is ipv4   and we are using this, if we have to use ipv6 we will use AF_INET6
           #define AF_INET 2
           #define AF_INET6 30
        type = 1
            #define SOCK_STREAM 1
        protocol = 0 so that it can determine the automatically
        */
    int listening_socket= socket(AF_INET,SOCK_STREAM,0 );
        //Checking if listening socket
        if(listening_socket == -1)  // something bad happened
        {
            cerr<<"Socket not created!\n";
            return -1;
        }         

    // Bind the socket to a IP / port
    sockaddr_in hint;
    hint.sin_family=AF_INET;
    hint.sin_port= htons(54000);
    inet_pton(AF_INET,"0.0.0.0",&hint.sin_addr);

        // Checking binding worked or not
        if(bind( listening_socket , (sockaddr *)&hint , sizeof(hint) )==-1)   // binding not successfull
        {
            cerr<<"IP/Port Binding Not Worked!\n";
            return -2;
        }

    // Mark the socket for listening in
    if(listen(listening_socket,SOMAXCONN) == -1)    // not Listening to socket
    {
        cerr<<"Not Listening\n";
        return -3;
    }
    // Accept a call
    sockaddr_in client;
    socklen_t clientSize = sizeof(client);
    char host[NI_MAXHOST];
    char svc[NI_MAXSERV];

    int clientSocket = accept(listening_socket,
                            (sockaddr *)&client,
                            &clientSize);
        //Checking Client Socket accepting or not
        if(clientSocket==-1)    // not accepting
        {
            cerr<<"Problem wit client connecting!\n";
            return -4;    
        }
    
    // Close the listening socket
    close(listening_socket);
    
    memset(host,0,NI_MAXHOST);
    memset(svc,0,NI_MAXSERV);

    int result= getnameinfo((sockaddr*)&client,
                            clientSize,
                            host,
                            NI_MAXHOST,
                            svc,
                            NI_MAXSERV,
                            0);
    if(result)  // Good result is not equal zero
    {
        cout<<host<<" connected on "<<svc<<endl;
    }
    else    //bad result
    {
        inet_ntop(AF_INET,&client.sin_addr,host,NI_MAXHOST);
        cout<<host<<" connected on "<<ntohs(client.sin_port)<<endl;
    }
    // While receiving - display message._echo message
    char buf[4096];
    while(true)
    {
        //clear buffer
        memset(buf,0,4096);
        //wait for a message
        int bytesRecv= recv(clientSocket,buf,4096,0);
            //Checking Connection
            if(bytesRecv==-1)   // connection issue
            {
                cerr<<"Connection Issue!\n";
                break;
            }
            else if(bytesRecv==0) // client disconnected
            {
                cout<<"Client Disconnected\n";
                break;
            }
            else if(bytesRecv==1)
            {
                cout<<"Connection Disconnected!\n";
                break;
            }
        //Display Message
        cout<<"Recieved: "<<string(buf,0,bytesRecv)<<endl;
        //Resend Message
        send(clientSocket,buf,bytesRecv+1,0);
    }
    // Close Socket
    close(clientSocket);
    return 0;
}

/*
    for futher information use
    man socket
    man getnameinfo
    man memset
    man inet
    man bind

*/