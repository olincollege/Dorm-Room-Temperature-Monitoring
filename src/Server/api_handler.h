// api_handler.h
#pragma once

#include <microhttpd.h>

/**
 * Handles GET requests for "/api/sensor".
 *
 * This function queries the latest temperature and humidity reading
 * from the sensor_readings SQLite database table and returns it
 * in JSON format.
 *
 * @param connection  The libmicrohttpd connection object representing the
 * request.
 * @return            MHD_YES on success, MHD_NO on failure.
 */
enum MHD_Result handle_api_sensor(struct MHD_Connection* connection);
