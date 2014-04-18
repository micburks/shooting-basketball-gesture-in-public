
#ifndef SBGIP_RECEIVE_H_
#define SBGIP_RECEIVE_H_

#include <stdlib.h>

#include "headers.h"
#include "file.h"


/*
 * return NULL on error
 */
char *receive(const int fd);



/*
 * returns 1 on success, 0 on empty line, -1 on error
 */
int get_request_line(const int fd, req_hdrs *req);



/*
 * returns 1 on success, 0 on empty line, -1 on error
 */
int get_hdr(const int fd, req_hdrs *req);



/*
 * returns 1 on success, 0 on empty line, -1 on error
 */
int read_until(const int fd, const char *c, char *buffer);



/*
 * returns 1 on success, 0 on empty line, -1 on error
 */
int read_until_eol(const int fd, char *buffer);



/*
 * returns -1 on error
 */
int eval_hdr(char *field, char *value, req_hdrs *req);



/*
 * returns NULL on error
 */
int request(req_hdrs *req);



#endif /* SBGIP_RECEIVE_H_ */
