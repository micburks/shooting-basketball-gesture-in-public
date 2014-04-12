
#ifndef SBGIP_FILE_H_
#define SBGIP_FILE_H_

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <unistd.h>

/*
 * return size of file if opened -- -1 on unopened
 */
int f_open(const char *file);



/*
 * return the size of a file -- -1 on error 
 */
off_t f_size(const int file_fd);



/*
 * return the time of last modification -- -1 on error 
 */
time_t f_last_mod(const int file_fd);



/*
 * return 1 on accessible, 0 on not
 */
int f_can_read(const int file_fd);



#endif /* SBGIP_FILE_H_ */

