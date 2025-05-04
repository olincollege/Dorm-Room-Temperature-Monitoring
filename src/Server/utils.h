// utils.h
#pragma once

#include <microhttpd.h>

void add_cors_headers(struct MHD_Response* resp);
const char* infer_mime_type(const char* filepath);
