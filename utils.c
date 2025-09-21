// Not in use atm

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "utils.h"

uint32_t len_of(char* msg, uint32_t msg_len)
{
    msg_len = htonl(strlen(msg));
    return msg_len;
}

