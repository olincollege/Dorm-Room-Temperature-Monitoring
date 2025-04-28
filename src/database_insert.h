#pragma once

// Inserts a sensor reading into the database.
// Returns:
//   0 on success,
//   1 on database open error,
//   2 on statement preparation error,
//   3 on insert execution error.
int insert_sensor_reading(const char *db_filename, float temperature, float humidity, const char *sensor_id);
