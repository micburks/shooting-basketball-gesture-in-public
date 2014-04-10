
#ifndef SBGIP_HEADERS_H_
#define SBGIP_HEADERS_H_

#include <string.h>
#include <time.h>
#include <sys/types.h>

typedef struct {

    char *version;
    char *server;
    char *status_code;

    char *content_type;
    off_t content_length;
    time_t last_modified;

} resp_hdrs;



void init_headers(resp_hdrs *hdrs);



char *status_msg(const char *code);



void datetime(const time_t time, char *buf);



#endif /* SBGIP_HEADERS_H_ */

