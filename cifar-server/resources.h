#pragma once

#include "http_response.h"
#include <stdbool.h>

void CreateErrorPage(struct THttpResponse* response, enum EHttpCode code);
void CreateIndexPage(struct THttpResponse* response, int page);
void SendCifarBitmap(struct THttpResponse* response, int number);
void SendStaticFile(struct THttpResponse* response, const char* path);
bool TrySendDirContent(struct THttpResponse* response, const char* path);
