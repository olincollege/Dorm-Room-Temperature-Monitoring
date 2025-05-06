#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define DB_PATH "sensor.db"
#define SENSOR_ID "sensor1"
// Integer constants as scoped, typed values
enum {
  INTERVAL_SECONDS = 2,    // how often to insert
  TEMP_RANDOM_MAX = 1000,  // max random value for temperature
  HUM_RANDOM_MAX = 3000    // max random value for humidity
};

// Float constants as compile-time constants
static const float TEMP_BASE = 20.0F;        // base temp for generation
static const float HUM_BASE = 40.0F;         // base humidity for generation
static const float HUM_MAGIC = 40.0F;        // used in humidity calc
static const float HUM_RANDOM_BASE = 40.0F;  // base value for random humidity
static const float HUM_OFFSET = 40.0F;       // offset for humidity

int insert_sensor_reading(float temperature, float humidity,
                          const char* sensor_id) {
  sqlite3* database = NULL;
  sqlite3_stmt* stmt = NULL;
  const char* sql =
      "INSERT INTO sensor_readings (temperature, humidity, sensor_id) VALUES "
      "(?, ?, ?);";

  if (sqlite3_open(DB_PATH, &database) != SQLITE_OK) {
    (void)fprintf(stderr, "Failed to open DB: %s\n", sqlite3_errmsg(database));
    return 1;
  }

  if (sqlite3_prepare_v2(database, sql, -1, &stmt, NULL) != SQLITE_OK) {
    (void)fprintf(stderr, "Failed to prepare SQL: %s\n",
                  sqlite3_errmsg(database));
    sqlite3_close(database);
    return 1;
  }

  sqlite3_bind_double(stmt, 1, temperature);
  sqlite3_bind_double(stmt, 2, humidity);
  sqlite3_bind_text(stmt, 3, sensor_id, -1, SQLITE_STATIC);

  if (sqlite3_step(stmt) != SQLITE_DONE) {
    (void)fprintf(stderr, "Failed to insert: %s\n", sqlite3_errmsg(database));
    sqlite3_finalize(stmt);
    sqlite3_close(database);
    return 1;
  }

  sqlite3_finalize(stmt);
  sqlite3_close(database);
  return 0;
}

int main() {
  unsigned int seed = 0;
  FILE* urandom = fopen("/dev/urandom", "re");
  if (urandom && fread(&seed, sizeof(seed), 1, urandom) == 1) {
    srandom(seed);
    if (fclose(urandom) != 0) {
      (void)fprintf(stderr, "Failed to close /dev/urandom\n");
      return 1;
    }
  } else {
    (void)fprintf(stderr, "Failed to seed random number generator\n");
    if (urandom) {
      if (fclose(urandom) != 0) {
        (void)fprintf(stderr, "Failed to close /dev/urandom\n");
        return 1;
      }
    }
    return 1;
  }
  float temp = TEMP_BASE +
               (((float)(random() % TEMP_RANDOM_MAX)) / 100.0F);  // 20.0 – 30.0
  float hum = HUM_OFFSET +
              (((float)(random() % HUM_RANDOM_MAX)) / 100.0F);  // 40.0 – 70.0

  if (insert_sensor_reading(temp, hum, SENSOR_ID) == 0) {
    printf("Inserted: temp=%.2f, hum=%.2f\n", temp, hum);
  } else {
    (void)fprintf(stderr, "Failed to insert reading\n");
  }

  struct timespec timeSpec;
  timeSpec.tv_sec = INTERVAL_SECONDS;
  timeSpec.tv_nsec = 0;
  nanosleep(&timeSpec, NULL);
  return 0;
}
