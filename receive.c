
#include "receive.h"



/*
 * return NULL on error
 */
char *receive(const int fd)
{

    req_hdrs req;
    memset(&req, 0, sizeof req);
    int g;

    while((g = get_hdr(fd, &req)) == 0) {

        if (g == -1) {

            perror("server->get_hdr\n");
            exit(1);

        }

    }

    //char *file = request(&req);
    char *file = "index.html";
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
    ssize_t size = 0;
    char buf[1];
    char field[20];
    char value[20];

    if ((r = read(fd, buf, sizeof(char))) == 0) {

        return 0;

    }

    char *buffer = field;
    buffer[size] = buf[0];

    while ((r = read(fd, &buf, sizeof(char))) != 0) {

        if(r == -1) {

            return -1;

        }

        if (buf[0] == ':') {

            buffer = value;
            size = 0;
            continue;

        }

        else if (buf[0] == '\r') {

            if ((r = read(fd, &buf, sizeof(char))) != 0) {

                if (buf[0] == '\n') {

                    break;

                }

            }

        }

        else {

            ++size;
            if (size >= sizeof(buffer) / sizeof(buffer[0])) {

                char temp[size * 2];
                buffer = &(temp[0]);
                buffer[size] = buf[0];
                continue;

            }

        }

    }

    return eval_hdr(&field, sizeof field, &value, sizeof value, req);

}



/*
 * returns -1 on error
 */
int eval_hdr(const char *field, const ssize_t f_size,
            const char *value, const ssize_t v_size,  const req_hdrs *req)
{

    return 1;

}



/*
 * returns NULL on error
 */
char *request(const req_hdrs *req)
{

    return NULL;

}



