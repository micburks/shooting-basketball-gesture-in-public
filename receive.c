
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
    g = get_request_line(fd, &req);
    if (g < 0) {

        perror("server->get_request_line\n");
        exit(1);

    }

    /* get all header lines */
    /* empty while */
    /*
    while((g = get_hdr(fd, &req)) >= 0) {}
    if (g < 0) {

        fprintf(stderr, "server: get_hdr\n");

    }

    print_hdrs(&req);

    if (request(&req) < 0) {

        return NULL;

    }

    return req.resource;
    */
    char *file = "index.html";
    return file;

}



/*
 * returns 1 on success, 0 on empty line, -1 on error
 */
int get_request_line(const int fd, req_hdrs *req)
{

    char blank = ' ';
    char *method = NULL;
    read_until(fd, &blank, &method);
    set_method(method, req);
    read_until(fd, &blank, &req->resource);
    read_until(fd, &blank, &req->version);

    return 0;

}



/*
 * returns 1 on success, 0 on empty line, -1 on error
 */
int get_hdr(const int fd, req_hdrs *req)
{

    char semicolon = ':';
    char *field = NULL;
    char *value = NULL;

    read_until(fd, &semicolon, &field);
    read_until_eol(fd, &value);

    return eval_hdr(field, value, req);

}



/*
 * returns 1 on success, 0 on empty line, -1 on error
 */
int read_until(const int fd, const char *c, char **buffer)
{

    ssize_t r;
    ssize_t size = 0;
    char ch;
    *buffer = malloc(sizeof(char) * 20);

    while ((r = read(fd, &ch, sizeof(char))) != 0) {

        if(r == -1) {

            return -1;

        }

        if (ch == *c) {

            break;

        }

        else if (ch == '\r') {

            if ((r = read(fd, &ch, sizeof(char))) != 0) {

                if (ch == '\n') {

                    break;

                }

            }

        }

        else {

            if (size >= sizeof(*buffer) / sizeof(char)) {

                if ((*buffer = realloc(*buffer, size * 2)) < 0) {

                    exit(0);

                }

            }

            *buffer[size] = ch;
            ++size;

        }

    }

    if(size >= (sizeof(*buffer) / sizeof(char))) {

        if((*buffer = realloc(*buffer, size + 1)) < 0) {

            exit(0);

        }

    }

    *buffer[size] = '\0';

    return 0;

}



/*
 * returns 1 on success, 0 on empty line, -1 on error
 */
int read_until_eol(const int fd, char **buffer)
{

    ssize_t r;
    ssize_t size = 0;
    char ch;
    *buffer = malloc(sizeof(char) * 20);
    strcpy(*buffer, "anyString");
    return 1;

    while ((r = read(fd, &ch, sizeof(char))) != 0) {

        if(r == -1) {

            return -1;

        }

        if (ch == '\r') {

            if ((r = read(fd, &ch, sizeof(char))) != 0) {

                if (ch == '\n') {

                    return 0;

                }

            }

        }

        else {

            if (size >= sizeof(buffer) / sizeof(char)) {

                if ((*buffer = realloc(*buffer, size * 2)) < 0) {

                    exit(0);

                }

            }

            *buffer[size] = ch;
            ++size;

        }

    }

    if(size >= (sizeof(*buffer) / sizeof(char))) {

        if((*buffer = realloc(*buffer, size + 1)) < 0) {

            exit(0);

        }

    }

    *buffer[size] = '\0';

    return 0;

}



/*
 * returns 1 on successfully evaluated, 0 on not found
 */
int eval_hdr(char *field, char *value, req_hdrs *req)
{

    if(field == NULL) {

        return -1;

    }

    int f_size = (int)strlen(field);
    /*printf("%d\n", f_size);*/
    switch (f_size) {

        case 4:
            if (strcmp(field, "Host") == 0) {

                req->host = value;
                return 1;

            }

            else if (strcmp(field, "From") == 0) {

                req->from = value;
                return 1;

            }

            break;

        case 6:

            if (strcmp(field, "Accept") == 0) {

                req->accept = value;
                return 1;

            }

            else if (strcmp(field, "Cookie") == 0) {

                req->cookie = value;
                return 1;

            }

            break;

        case 10:

            if (strcmp(field, "User-Agent") == 0) {

                req->user_agent = value;
                return 1;

            }

            else if (strcmp(field, "Connection") == 0) {

                req->connection = value;
                return 1;

            }

            break;

        case 13:

            if (strcmp(field, "Cache-Control") == 0) {

                req->cache_control = value;
                return 1;

            }

            break;

        case 15:

            if (strcmp(field, "Accept-Encoding") == 0) {

                req->accept_encoding = value;
                return 1;

            }

            else if (strcmp(field, "Accept-Language") == 0) {

                req->accept_language = value;
                return 1;

            }

            break;

        default:

            break;

    }

    return 0;

}



/*
 * check accessibility of file
 * returns 1 on unchanged, 0 on changed to index.html, -1 on error
 */
int request(req_hdrs *req)
{

    if (req->resource == NULL) {

        return -1;

    }

    else if ((req->resource)[0] == '/') {

        char *index = "index.html";
        free(req->resource);
        req->resource = calloc(strlen(index)+1, sizeof(char));
        strcpy(req->resource, index);
        return 0;

    }

    else if(!f_can_read(req->resource)) {

        char *index = "index.html";
        free(req->resource);
        req->resource = calloc(strlen(index)+1, sizeof(char));
        strcpy(req->resource, index);
        return 0;

    }

    else {

        return 1;

    }

    /* not sure when to return error */
    /* probably when requesting a file outside of root server folder */
    return -1;

}



