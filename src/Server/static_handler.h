// static_handler.h
#pragma once

#ifndef STATIC_HANDLER_H
#define STATIC_HANDLER_H

#include <microhttpd.h>

enum MHD_Result handle_static_file(const char* url,
                                   struct MHD_Connection* connection);

#endif // STATIC_HANDLER_H
