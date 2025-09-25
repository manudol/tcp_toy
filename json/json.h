#ifndef JSON_H
#define JSON_H

typedef struct Reqest_payload {
    char *ip_addr;
    char *msg;
} req_p;

req_p *parse_json_string(char *json_string);
char *get_json_string(req_p *req);
void assert_JSON();

#endif

