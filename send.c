/*
 * send.c
 */

#include <string.h>
#include <sys/sendfile.h>

#include "send.h"

/*
 * send a particalar message 
 */
int send_msg(int fd, char *msg)
{

    int m_len = strlen(msg);
    if (send(fd, msg, m_len, 0) == -1) {

        fprintf(stderr, "server: send failed");

    }

    return 0;

}



/*
 * will be called for each file 
 * no return -- errors will be sent by subsequent functions 
 */
int respond(const int fd, const char *file)
{

    resp_hdrs hdrs;
    init_resp_headers(&hdrs);

    int file_fd = f_open(file);
    if (file_fd == -1) {

        fprintf(stderr, "file not found\n");
        hdrs.status_code = 404;

    }
    else {

        hdrs.status_code = 200;

    }

    off_t file_size = f_size(file_fd);
    if (file_size == -1) {

        fprintf(stderr, "file size error\n");
        hdrs.status_code = 404;

    }

    hdrs.content_length = file_size;
    hdrs.last_modified = f_last_mod(file_fd);
    send_headers(fd, &hdrs);

    if (hdrs.status_code != 404) {

        send_file(fd, file_fd, file_size);

    }
    else {

        char *msg404 =
            "<html><head><title>404 Not Found</head></title><body><p>404 Not Found: The requested resource could not be found!</p></body></html>";
        send_msg(fd, msg404);

    }

    return 0;

}



/*
 * send an entire file 
 */
int send_file(const int fd, const int file_fd, const off_t file_size)
{

    size_t total_bytes_sent = 0;
    ssize_t bytes_sent;
    while (total_bytes_sent < file_size) {

    if ((bytes_sent =
        sendfile(fd, file_fd, 0, file_size - total_bytes_sent)) <= 0) {

        if (errno == EINTR || errno == EAGAIN) {

            continue;

        }

        perror("server->sendfile");
        return -1;

    }

    total_bytes_sent += bytes_sent;

    }

    /*
     * never reached
     * perror will say success and return -1 
     */
    return 0;

}



/*
 * load and send headers
 */
int send_headers(const int fd, const resp_hdrs *hdrs)
{

    /* HTTP/1.1 200 OK  */
    char hbuf[100] = "HTTP/";
    strcat(hbuf, hdrs->version);
    strcat(hbuf, status_msg(hdrs->status_code));
    strcat(hbuf, "\r\n");
    send_msg(fd, hbuf);
    /********************/

    /* Server: SBGiP/0.1    */
    char rbuf[100] = "Server: ";
    strcat(rbuf, hdrs->server);
    strcat(rbuf, "\r\n");
    send_msg(fd, rbuf);
    /************************/

    /* Content-Type: text/html  */
    char ibuf[100] = "Content-Type: ";
    strcat(ibuf, hdrs->content_type);
    strcat(ibuf, "\r\n");
    send_msg(fd, ibuf);
    /****************************/

    /* Content-Length: 100  */
    char obuf[100] = "Content-Type: ";

    char len[20];
    sprintf(len, "%d", (int)hdrs->content_length);
    strcat(obuf, len);

    strcat(obuf, "\r\n");
    send_msg(fd, obuf);
    /************************/

    /* Last-Modified: Fri, 31 Dec 1999 23:59:59 GMT */
    char pbuf[100] = "Last-Modified: ";
    char tbuf[30];
    datetime(hdrs->last_modified, tbuf);
    strcat(pbuf, tbuf);
    strcat(pbuf, "\r\n\r\n");
    send_msg(fd, pbuf);
    /************************************************/

    return 0;

}



