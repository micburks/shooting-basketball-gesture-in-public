
#ifndef SBGIP_SEND_H_
#define SBGIP_SEND_H_

#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "headers.h"
#include "file.h"

/*
 * send a particalar message 
 */
int send_msg(int fd, char *msg);



/*
 * will be called for each file 
 * no return -- errors will be sent by subsequent functions 
 */
int respond(const int fd, const char *file);



/*
 * send an entire file 
 */
int send_file(const int fd, const int file_fd, const off_t file_size);



/*
 * load and send headers
 */
int send_headers(const int fd, const resp_hdrs *hdrs);



#endif /* SBGIP_SEND_H_ */

