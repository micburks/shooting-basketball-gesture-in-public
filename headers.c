/*
 * headers.c
 */

#include "headers.h"


void init_resp_headers(resp_hdrs *resp)
{

    memset(resp, 0, sizeof *resp);

    resp->version = "1.1";
    resp->server = "SBGiP/0.1";
    resp->content_type = "text/html";

}



void init_req_headers(req_hdrs *req)
{

    memset(req, 0, sizeof *req);

    /*
    resp->version = "1.1";
    resp->server = "SBGiP/0.1";
    resp->content_type = "text/html";
    */

}



char *status_msg(const int code)
{

    if (code == 200) {
        return " 200 OK";
    }
    else if (code == 404) {
        return " 404 Not Found";
    }

    return " 500 Server Error";

}



void datetime(const time_t time, char *buf)
{

    struct tm *tinfo;
    tinfo = localtime(&time);
    strftime(buf, sizeof buf, "%a, %d %b %Y %H:%M:%S %Z", tinfo);

}



