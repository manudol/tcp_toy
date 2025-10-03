#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <json-c/json.h>
#include "json.h"

char* dynamic_file_reader(char* file_name, int chunk_size)
{
    FILE* file_ptr;

    int chk = chunk_size;

    char* buffer = (char*)malloc(chk);
    
    file_ptr = fopen(file_name, "r");
    if (file_ptr == NULL) {
        perror("Opening file");
        exit(1);
    }
    char c;
    int counter = 0;
    // chars get added in 'chunk_size' num of bytes chunks to buffer - recommended 1024 bytes chunk_size
    while ((c = fgetc(file_ptr)) != EOF) {
        if (counter % chk == 0 && counter != 0) {
            char* tmp_buffer = (char*)realloc(buffer, sizeof(buffer));
            buffer = tmp_buffer;
        }             
        buffer[counter] = c;
        counter++;
    }
    return buffer;
}


char* get_json_string(req_p *req)
{
    char* json_string_buffer = dynamic_file_reader("json/req.json", 1024);

    struct json_object* parsed_json = json_tokener_parse(json_string_buffer);

    struct json_object* ip_addr_obj;
    struct json_object* msg_body_obj;

    json_object_object_get_ex(parsed_json, "addr", &ip_addr_obj);
    json_object_object_get_ex(parsed_json, "body", &msg_body_obj);

    json_object_set_string(ip_addr_obj, req->ip_addr);
    json_object_set_string(msg_body_obj, req->msg);
    
    char* modified_json = (char*)json_object_to_json_string(parsed_json);
 
    free(json_string_buffer);
    return modified_json;
}




req_p* parse_json_string(char *json_string)
{ 
    struct json_object* parsed_json = json_tokener_parse(json_string);

    struct json_object* ip_addr_obj;
    struct json_object* msg_body_obj;

    json_object_object_get_ex(parsed_json, "addr", &ip_addr_obj);
    json_object_object_get_ex(parsed_json, "body", &msg_body_obj);
    
    char* ip_addr = (char*)json_object_get_string(ip_addr_obj);
    char* msg = (char*)json_object_get_string(msg_body_obj);

    req_p* req = malloc(sizeof(req_p));
    req->ip_addr = ip_addr;
    req->msg = msg;

    return req;
}


// for unit testing
void assert_JSON()
{
    req_p* req = NULL;
    req->ip_addr = "127.0.0.1";
    req->msg = "hello, friend\n";

    char* payload1 = get_json_string(req);
    if (!payload1) {
        printf("Payload NULL\n");
        exit(1);
    }

    char* test_p = "{ \"addr\":\"127.0.0.1\", \"body\": \"hello, friend\n\" }";
    int assert_len = strlen(test_p);

    req_p* req2 = parse_json_string(test_p);
    if (!req2) {
        printf("Error\n");
        exit(1);
    }

    char* payload2 = get_json_string(req2);
    if (!payload2) {
        printf("Payload2 NULL\n");
        exit(1);
    }
    
    if (strncmp(payload1, payload2, assert_len) == 0) {
        printf("Unit test successful for assert_JSON()\n");
    } else {
        printf("Assertion errro in code for assert_JSON()\n");
    }
}


