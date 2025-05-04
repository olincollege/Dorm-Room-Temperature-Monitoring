// api_handler.h
#pragma once

#include <microhttpd.h>

enum MHD_Result handle_api_sensor(struct MHD_Connection* connection);
