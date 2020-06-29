#pragma once

#include "stringbuilder.h"

#include <stdbool.h>

struct THttpRequest {
    char* Method;
    char* Path;
    char* QueryString;
    bool KeepAliveNotice;
};

void THttpRequest_Init(struct THttpRequest* self);
int THttpRequest_Receive(struct THttpRequest* self, int sockfd);
void THttpRequest_Destroy(struct THttpRequest* self);
