#include <iostream>
#include "udpmulticast.h"

int main()
{
    UDPMulticast multicaster;
    char* msg = "Sending some pixel lovin\n";
    multicaster.multicastMessage(msg);

    return 0;
}
