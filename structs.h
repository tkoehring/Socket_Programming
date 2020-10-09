#pragma once
#include "defines.h"

struct message
{
        int type;
        char message[MESSAGE_SIZE];
};

struct job
{
        int fd;
        struct message m;
};

