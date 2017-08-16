/*
 * UDP Multicast
 * Multicast OPC messages to other hosts on the local network.
 */


#include "udpmulticast.h"
#include "opc.h"

#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAXBUFSIZE 65536 // Max UDP Packet size is 64 Kbyte

//UDPMulticast::UDPMulticast(char* address, int port)
UDPMulticast::UDPMulticast()
{
    const char* groupAddress = "224.0.51.51"; // Multicast group to join
               int groupPort = 12345;         // Multicast port
                 int matrixW = 64;            // LED matrix size (we can ignore
                 int matrixH = 32;            // pixels beyond these)
                                              // Todo: grab these programmatically
                                              // from FCServer (?)
                 messageSize = (matrixW * matrixH * 3) + 4;

    // Set up UDP socket
    // See http://beej.us/guide/bgnet/output/html/multipage/index.html
	// See http://www.tack.ch/multicast/

    int status, socklen;
    char buffer[MAXBUFSIZE];
    struct in_addr iaddr;
    unsigned char ttl = 3;
    unsigned char one = 1;

    // set content of struct mcastAddress and imreq to zero
    memset(&mcastAddress, 0, sizeof(struct sockaddr_in));
    memset(&iaddr, 0, sizeof(struct in_addr));

    // open a UDP datagram socket
    sock = socket(PF_INET, SOCK_DGRAM, 0);
    if ( sock < 0 )
        perror("Error creating socket"), exit(0);

    // create destination address
    mcastAddress.sin_family = AF_INET;
    mcastAddress.sin_port = htons(0);  // Use the first free port
    mcastAddress.sin_addr.s_addr = htonl(INADDR_ANY); // Bind socket to any interface
    status = bind(sock, (struct sockaddr *)&mcastAddress, sizeof(struct sockaddr_in));
    if ( status < 0 )
        perror("Error binding socket to interface"), exit(0);

    // Set the outgoing interface to DEFAULT
    iaddr.s_addr = INADDR_ANY; // use DEFAULT interface
    setsockopt(sock, IPPROTO_IP, IP_MULTICAST_IF, &iaddr,
              sizeof(struct in_addr));

    // set SO_REUSEADDR on a socket to true (1):
    int optval = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval);
    setsockopt(sock, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof optval);

    // Set multicast packet TTL to 3; default TTL is 1
    setsockopt(sock, IPPROTO_IP, IP_MULTICAST_TTL, &ttl,
              sizeof(unsigned char));

    // send multicast traffic to myself too
    status = setsockopt(sock, IPPROTO_IP, IP_MULTICAST_LOOP,
                       &one, sizeof(unsigned char));

    // set destination multicast address
    mcastAddress.sin_family = AF_INET;
    mcastAddress.sin_addr.s_addr = inet_addr(groupAddress);
    mcastAddress.sin_port = htons(groupPort);    

    std::cout << "Socket bound.\n";
    //char* message = "Sending you some pixel lovin";
    //int sentBytes = sendto(sock, message, sizeof(message), 0, (struct sockaddr *)&mcastAddress, sizeof(struct sockaddr_in));
    //std::cout << "\nSent " << sentBytes << " bytes.";
}

int UDPMulticast::multicastMessage(const OPC::Message &msg)
{
    // Pack OPC message struct into a bytestream:
    //unsigned char* datagram[this->messageSize];
    uint8_t* datagram[this->messageSize];
    memcpy(datagram+0, &msg.channel, 1    );
    memcpy(datagram+1, &msg.command, 1    );
    memcpy(datagram+2, &msg.lenHigh, 1    );
    memcpy(datagram+3, &msg.lenLow,  1    );
    memcpy(datagram+4, &msg.data, (this->messageSize - 4));
    
    // Send over UDP socket
    int bytesSent = sendto(this->sock, datagram, this->messageSize, 0, (struct sockaddr *)&mcastAddress, sizeof(struct sockaddr_in));
    std::cout << "Sent " << bytesSent << " bytes.\n";
    
    // Return number of bytes sent.
    return bytesSent;
}

UDPMulticast::~UDPMulticast(){
    shutdown(sock, 2);
    //close(sock);
}