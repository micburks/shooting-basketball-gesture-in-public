
#ifndef SBGIP_RECEIVE_H_
#define SBGIP_RECEIVE_H_

#include "headers.h"

/*
 * return -1 on error
 */
int receive(const int fd, const req_hdrs *req);



/*
 * returns -1 on error
 */
char *parse_request(const char *buf, const req_hdrs *req);



#endif /* SBGIP_RECEIVE_H_ */
