#include <stdio.h>
#include <string.h>
#include <event2/event.h>
#include <evhtp.h>

void RequestHandler(evhtp_request_t* request, void* ptr) {
    const char* query = (const char *)request->uri->query_raw;
    if (query)
        printf("Query: %s\n", query);

    const char* result = "<h1>Hello<h1>";
    evbuffer_add(request->buffer_out, result, strlen(result));
    evhtp_headers_add_header(request->headers_out, evhtp_header_new("Content-Type", "text/html; charset=UTF-8", 0, 0));
    evhtp_send_reply(request, EVHTP_RES_OK);
}

int main() {
    evbase_t* base = event_base_new();
    evhtp_t* http = evhtp_new(base, NULL);
    http->parser_flags = EVHTP_PARSE_QUERY_FLAG_LENIENT;
    evhtp_set_cb(http, "/", RequestHandler, NULL);
    if (0 != evhtp_bind_socket(http, "0.0.0.0", 8080, 1024)) {
        printf("Can't bind port 8080\n");
        return -1;
    }
    event_base_loop(base, 0);
    evhtp_free(http);
    event_base_free(base);
    return 0;
}

