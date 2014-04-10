/*
 * headers.c
 */

#include "headers.h"


void init_headers(resp_hdrs *hdrs)
{

    memset(hdrs, 0, sizeof *hdrs);

    hdrs->version = "1.1";
    hdrs->server = "SBGiP/0.1";
    hdrs->content_type = "text/html";

}



char *status_msg(const char *code)
{

    if (strcmp(code, "200") == 0) {
        return "OK";
    }
    else if (strcmp(code, "404") == 0) {
        return "Not Found";
    }

    return "";
}



void datetime(const time_t time, char *buf)
{

    struct tm *tinfo;
    tinfo = localtime(&time);
    strftime(buf, sizeof buf, "%a, %d %b %Y %H:%M:%S %Z", tinfo);

}



