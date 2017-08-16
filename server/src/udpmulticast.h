/*
 * UDP Multicast
 * Multicast OPC messages to other hosts on the local network.
 */

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "opc.h"


class UDPMulticast
{
	struct sockaddr_in mcastAddress;
	int sock;

public:
    UDPMulticast();
    ~UDPMulticast();
    //UDPMulticast(char* address, int port);
    //void multicastMessage(const OPC::Message &msg);
    //void multicastMessage(const char* &message);
    //int multicastMessage(char* message);
    int multicastMessage(const OPC::Message &msg);
};