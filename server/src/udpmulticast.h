/*
 * UDP Multicast
 * Multicast OPC messages to other hosts on the local network.
 */

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <cstring>
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
    int multicastMessage(const OPC::Message &msg);

private:
	int messageSize;
};