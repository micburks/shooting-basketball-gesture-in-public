
#ifndef SBGIP_HEADERS_H_
#define SBGIP_HEADERS_H_

#include <string.h>
#include <time.h>
#include <sys/types.h>



/*
 * response headers
 */
typedef struct {

    char *version;
    char *server;
    int status_code;

    char *content_type;
    off_t content_length;
    time_t last_modified;

} resp_hdrs;



/*
 * supported request methods
 */
typedef enum {

    GET,
    POST,
    HEAD

} method;



/*
 * request headers
 */
typedef struct {

    method request_method;
    char *resource;
    char *version;

    char *from;
    char *user_agent;

    char *msg_body;

} req_hdrs;


void init_resp_headers(resp_hdrs *resp);



void init_req_headers(req_hdrs *req);



char *status_msg(const int code);



void datetime(const time_t time, char *buf);



#endif /* SBGIP_HEADERS_H_ */

