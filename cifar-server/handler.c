#include "handler.h"

#include "http_request.h"
#include "http_response.h"
#include "resources.h"
#include "stringutils.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#ifdef DEBUG
static const char* SafeStr(const char* value) {
    return (value != NULL) ? value : "<null>";
}
#endif

static void Handle(const struct THttpRequest* request, struct THttpResponse* response) {
    #ifdef DEBUG
    fprintf(
        stderr, "method: '%s'; path: '%s'; qs: '%s'\n",
        SafeStr(request->Method), SafeStr(request->Path), SafeStr(request->QueryString)
    );
    #endif

    if (strcasecmp(request->Method, "GET") != 0) { // case-insensitive compare
        CreateErrorPage(response, HTTP_METHOD_NOT_ALLOWED);
        return;
    }

    char* path = malloc(strlen(request->Path) + 1);
    UrlToString(path, request->Path);

    if (strcmp(path, "/") == 0) {
        int page = request->QueryString ? GetIntParam(request->QueryString, "page") : 0;
        CreateIndexPage(response, page);
        free(path);
        return;
    }
    if (StartsWith(path, "/images/")) {
        int n;
        if (sscanf(path, "/images/%d.bmp", &n) == 1) {
            SendCifarBitmap(response, n);
            free(path);
            return;
        }
    }
    if (StartsWith(path, "/static/") || strcmp(path, "/static") == 0) {
        if (!TrySendDirContent(response, path)) { // keep slash
            SendStaticFile(response, path + 1);
        }
        free(path);
        return;
    }

    CreateErrorPage(response, HTTP_NOT_FOUND);
    free(path);
}

void ServeClient(int sockfd) {
    struct THttpRequest req;
    THttpRequest_Init(&req);
    struct THttpResponse resp;
    THttpResponse_Init(&resp);

    if (THttpRequest_Receive(&req, sockfd)) {
        Handle(&req, &resp);
    } else {
        CreateErrorPage(&resp, HTTP_BAD_REQUEST);
    }

    THttpResponse_Send(&resp, sockfd);
    THttpResponse_Destroy(&resp);
    THttpRequest_Destroy(&req);

    close(sockfd);
}
