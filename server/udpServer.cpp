//==================================================== file = udpServer.c =====
//=  A message "server" program to demonstrate sockets programming            =
//=============================================================================
//=  Notes:                                                                   =
//=    1) This program conditionally compiles for Winsock and BSD sockets.    =
//=       Set the initial #define to WIN or BSD as appropriate.               =
//=    2) This program serves a message to program udpClient running on       =
//=       another host.                                                       =
//=    3) The steps #'s correspond to lecture topics.                         =
//=---------------------------------------------------------------------------=
//=  Example execution: (udpServer and udpClient running on host 127.0.0.1)   =
//=    Waiting for recvfrom() to complete...                                  =
//=    IP address of client = 127.0.0.1  port = 55476)                        =
//=    Received from client: Test message from CLIENT to SERVER               =
//=---------------------------------------------------------------------------=
//=  Build: bcc32 udpServer.c or cl udpServer.c wsock32.lib for Winsock       =
//=         gcc udpServer.c -lsocket -lnsl for BSD                            =
//=---------------------------------------------------------------------------=
//=  Execute: udpServer                                                       =
//=---------------------------------------------------------------------------=
//=  Author: Ken Christensen                                                  =
//=          University of South Florida                                      =
//=          WWW: http://www.csee.usf.edu/~christen                           =
//=          Email: christen@csee.usf.edu                                     =
//=---------------------------------------------------------------------------=
//=  History:  KJC (08/02/08) - Genesis (from server1.c)                      =
//=            KJC (09/07/09) - Minor clean-up                                =
//=            KJC (09/22/13) - Minor clean-up to fix warnings                =
//=============================================================================
#define  WIN                // WIN for Winsock and BSD for BSD sockets

//----- Include files --------------------------------------------------------
#include <iostream>                     // std::cout
#include <chrono>                       // Timers.
#include <ctime>
#include <cstdlib>


#include <stdio.h>          // Needed for printf()
#include <string.h>         // Needed for memcpy() and strcpy()
#include <stdlib.h>         // Needed for exit()

  #include <sys/types.h>    // Needed for sockets stuff
  #include <netinet/in.h>   // Needed for sockets stuff
  #include <sys/socket.h>   // Needed for sockets stuff
  #include <arpa/inet.h>    // Needed for sockets stuff
  #include <fcntl.h>        // Needed for sockets stuff
  #include <netdb.h>        // Needed for sockets stuff

#include <unistd.h> // to close in c++ 



//----- Defines --------------------------------------------------------------
#define  PORT_NUM   1050    // Arbitrary port number for the server

//===== Main program =========================================================
int main()
{
    int                  server_s;        // Server socket descriptor
    struct sockaddr_in   server_addr;     // Server Internet address
    struct sockaddr_in   client_addr[2];  // Client Internet address
    struct in_addr       client_ip_addr;  // Client IP address
    socklen_t            addr_len;        // Internet address length
    char                 out_buf[4096];   // Output buffer for data
    char                 in_buf[4096];    // Input buffer for data
    int                  retcode;         // Return code

    // >>> Step #1 <<<
    // Create a socket
    //   - AF_INET is Address Family Internet and SOCK_DGRAM is datagram
    server_s = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_s < 0)
    {
        printf("*** ERROR - socket() failed \n");
        exit(-1);
    }

    // >>> Step #2 <<<
    // Fill-in my socket's address information
    server_addr.sin_family = AF_INET;                 // Address family to use
    server_addr.sin_port = htons(PORT_NUM);           // Port number to use
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);  // Listen on any IP address
    retcode = bind(server_s, (struct sockaddr *)&server_addr,
            sizeof(server_addr));
    if (retcode < 0)
    {
        printf("*** ERROR - bind() failed \n");
        exit(-1);
    }

    int clients_Number = 0;

    while(clients_Number != 2){

        // >>> Step #3 <<<
        // Wait to receive a message from client
        printf("Waiting for recvfrom() to complete... \n");
        addr_len = sizeof(client_addr[clients_Number]);
        retcode = recvfrom(server_s, in_buf, sizeof(in_buf), 0,
                (struct sockaddr *)&client_addr[clients_Number], &addr_len);
        if (retcode < 0)
        {
            printf("*** ERROR - recvfrom() failed \n");
            exit(-1);
        }

        // Copy the four-byte client IP address into an IP address structure
        memcpy(&client_ip_addr, &client_addr[clients_Number].sin_addr.s_addr, 4);

        // Print an informational message of IP address and port of the client
        printf("IP address of client = %s  port = %d) \n", inet_ntoa(client_ip_addr),
                ntohs(client_addr[clients_Number].sin_port));

        // Output the received message
        printf("Received from client: %s \n", in_buf);

        clients_Number++;
    }


    // >>>>>>>>>>>> CALCULATE AND SEND TIME <<<<<<<<<<<<
    std::cout<<"Generating clock signal"<<std::endl;

    std::chrono::system_clock::time_point today = std::chrono::system_clock::now();

    std::time_t tt = std::chrono::system_clock::to_time_t ( today );
    std::cout << "Now is: " << ctime(&tt) << std::endl;

    std::cout << "1 => Today : " << today.time_since_epoch().count() << std::endl;

    today += std::chrono::seconds(5);

    long long int time_to_start = today.time_since_epoch().count(); 

    //auto t2 = std::chrono::time_point_cast<std::chrono::nanoseconds>(today);

    //std::cout << "Today2 : " << t2.time_since_epoch().count() << std::endl;

    std::chrono::nanoseconds time_nanoseconds (time_to_start);

    std::cout << "2 => Today : " << time_nanoseconds.count() << std::endl;

    clients_Number = 0;

    while(clients_Number != 2){

        // >>> Step #4 <<<
        // Send to the client using the server socket
        //strcpy(out_buf, "This is a reply message from SERVER to CLIENT");
        
        sprintf(out_buf, "%llu",time_to_start);

        retcode = sendto(server_s, out_buf, (strlen(out_buf) + 1), 0,
                (struct sockaddr *)&client_addr[clients_Number], sizeof(client_addr[clients_Number]));
        if (retcode < 0)
        {
            printf("*** ERROR - sendto() failed \n");
            exit(-1);
        }

        clients_Number++;
    }

    // >>> Step #5 <<<
    // Close all open sockets

  retcode = close(server_s);
  if (retcode < 0)
  {
    printf("*** ERROR - close() failed \n");
    exit(-1);
  }
    // Return zero and terminate
    return(0);
} 