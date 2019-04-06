/*
 * headers.c
 */

#include "headers.h"

void init_resp_headers(resp_hdrs *resp) {

  memset(resp, 0, sizeof *resp);

  resp->version = "1.1";
  resp->server = "SBGiP/0.1";
  resp->content_type = "text/html";
}

void init_req_headers(req_hdrs *req) { memset(req, 0, sizeof *req); }

char *status_msg(const int code) {

  if (code == 200) {
    return " 200 OK";
  } else if (code == 404) {
    return " 404 Not Found";
  }

  return " 500 Server Error";
}

void datetime(const time_t time, char *buf) {

  struct tm *tinfo;
  tinfo = localtime(&time);
  strftime(buf, sizeof buf, "%a, %d %b %Y %H:%M:%S %Z", tinfo);
}

int set_method(char *method, req_hdrs *req) {

  if (method == NULL) {

    req->request_method = FAIL;

  }

  else if (strcmp(method, "GET") == 0) {

    req->request_method = GET;
    return 1;

  }

  else if (strcmp(method, "POST") == 0) {

    req->request_method = POST;
    return 1;

  }

  else if (strcmp(method, "HEAD") == 0) {

    req->request_method = HEAD;
    return 1;
  }

  return 0;
}

char *string_method(const req_hdrs *req) {

  if (req->request_method == GET) {

    return "GET";

  } else if (req->request_method == POST) {

    return "POST";

  } else if (req->request_method == HEAD) {

    return "HEAD";
  }

  return NULL;
}

void print_hdrs(const req_hdrs *req) {

  printf("method: %s\n", string_method(req));
  printf("resource: %s\n", req->resource);
  printf("version: %s\n", req->version);
  printf("from: %s\n", req->from);
  printf("user-agent: %s\n", req->user_agent);
  printf("msg-body: %s\n", req->msg_body);
}
