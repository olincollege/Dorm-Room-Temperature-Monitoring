// http_server.h
#pragma once

#include <microhttpd.h>

enum MHD_Result answer(void* cls, struct MHD_Connection* connection,
                       const char* url, const char* method, const char* version,
                       const char* upload_data, size_t* upload_data_size,
                       void** con_cls);

void add_cors_headers(struct MHD_Response* resp);
