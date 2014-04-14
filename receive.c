
#include "receive.h"



/*
 * return NULL on error
 */
char *receive(const int fd)
{

    req_hdrs req;
    memset(&req, 0, sizeof req);
    int g;

    /* get request line first */

    /* get all header lines */
    while((g = get_hdr(fd, &req)) == 0) {

        if (g == -1) {

            perror("server->get_hdr\n");
            exit(1);

        }

    }

    print_hdrs(&req);

    if (request(&req) == -1) {

        return NULL;

    }

    return req.resource;

}



/*
 * returns 1 on success, 0 on empty line, -1 on error
 */
int get_hdr(const int fd, req_hdrs *req)
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

    return eval_hdr(field, sizeof field, value, sizeof value, req);

}



/*
 * returns -1 on error
 */
int eval_hdr(char *field, ssize_t f_size,
            char *value, ssize_t v_size,  req_hdrs *req)
{

    switch (f_size) {

        case 4:
            if (strcmp(field, "Host")) {

                req->host = value;
                return 0;

            }

            else if (strcmp(field, "From")) {

                req->from = value;
                return 0;

            }

            break;

        case 6:

            if (strcmp(field, "Accept")) {

                req->accept = value;
                return 0;

            }

            else if (strcmp(field, "Cookie")) {

                req->cookie = value;
                return 0;

            }

            break;

        case 10:

            if (strcmp(field, "User-Agent")) {

                req->user_agent = value;
                return 0;

            }

            else if (strcmp(field, "Connection")) {

                req->connection = value;
                return 0;

            }

            break;

        case 13:

            if (strcmp(field, "Cache-Control")) {

                req->cache_control = value;
                return 0;

            }

            break;

        case 15:

            if (strcmp(field, "Accept-Encoding")) {

                req->accept_encoding = value;
                return 0;

            }

            else if (strcmp(field, "Accept-Language")) {

                req->accept_language = value;
                return 0;

            }

            break;

        default:

            break;

    }

    return -1;

}



/*
 * check accessibility of file
 * returns -1 on error
 */
int request(req_hdrs *req)
{

    if (*(req->resource) == '/') {

        req->resource = "index.html";
        return 0;

    }

    else if(!f_can_read(req->resource)) {

        req->resource = "index.html";
        return 0;

    }

    else {

        return 1;

    }

    /* not sure when to return error */
    /* probably when requesting a file outside of root server folder */
    return -1;

}



