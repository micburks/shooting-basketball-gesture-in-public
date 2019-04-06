
#ifndef SBGIP_HEADERS_H_
#define SBGIP_HEADERS_H_

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>

/*
 * response headers
 */
typedef struct {
  char* version;
  char* server;
  int status_code;

  char* content_type;
  off_t content_length;
  time_t last_modified;

} resp_hdrs;

/*
 * supported request methods
 */
typedef enum {

  GET,
  POST,
  HEAD,
  FAIL

} method;

/*
 * request headers
 */
typedef struct {
  /* line 1 */
  method request_method;
  char* resource;
  char* version;

  char* host;
  char* from;
  char* user_agent;

  char* accept;
  /*
  char *accept_charset;
  */
  char* accept_encoding;
  char* accept_language;
  /*
  char *accept_ranges;
  */

  /*
  char *age;
  char *allow;
  char *authorization;
  */
  char* cache_control;
  /*
  char *cache_extension;
  */

  char* cookie;
  char* connection;
  /*
  char *content_encoding;
  char *content_language;
  char *content_length;
  char *content_range;
  char *content_type;

  char *date;
  char *expect;
  char *expires;

  char *if_match;
  char *if_modified_since;
  char *if_none_match;
  char *if_range;
  char *if_unmodified_since;
  char *last_modified;
  char *location;
  char *max_forwards;
  char *proxy_authenticate;
  */

  char* msg_body;

} req_hdrs;

void init_resp_headers(resp_hdrs* resp);

void init_req_headers(req_hdrs* req);

char* status_msg(const int code);

void datetime(const time_t time, char* buf);

int set_method(char* method, req_hdrs* req);

char* string_method(const req_hdrs* req);

void print_hdrs(const req_hdrs* req);

#endif /* SBGIP_HEADERS_H_ */
