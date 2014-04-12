
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
int get_hdr(const int fd, const req_hdrs *req);



/*
 * returns -1 on error
 */
int eval_hdr(const char *field, const ssize_t f_size,
            const char *value, const ssize_t v_size,  const req_hdrs *req);



/*
 * returns NULL on error
 */
char *request(const req_hdrs *req);



#endif /* SBGIP_RECEIVE_H_ */
