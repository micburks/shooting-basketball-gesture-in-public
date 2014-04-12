
#include "receive.h"

/*
 * return NULL on error
 */
char *receive(const int fd)
{

    req_hdrs req;
    memset(&req, 0, sizeof req);
    int g;

    while(1) {

        g = get_hdr(fd, &req);
        if (g == -1) {

            perror("server->get_hdr%s\n");
            exit(1);

        }
        else if (g == 0) {

            break;

        }

    }

    char *file = request(&req);
    if(!f_can_read(file)) {

        return "index.html";

    }

    return file;

}



/*
 * returns 1 on success, 0 on empty line, -1 on error
 */
int get_hdr(const int fd, const req_hdrs *req)
{

    ssize_t r;
    ssize_t size;
    char buf[20];
    char (*field)[20];
    char (*value)[20];

    if ((r = read(fd, buf, sizeof(char))) == 0) {

        return 0;

    }

    while (r = read(fd, &buf, sizeof(char))) {

        if(r == -1) {

            return -1;

        }

        else if(r == 0) {

            break;

        }

        ++size;

    }

    return eval_hdr(&field, sizeof field, &value, sizeof value, req);

}



/*
 * returns -1 on error
 */
int eval_hdr(const char *field, const ssize_t f_size,
            const char *value, const ssize_t v_size,  const req_hdrs *req)
{

    return -1;

}



/*
 * returns NULL on error
 */
char *request(const req_hdrs *req)
{

    return NULL;

}



