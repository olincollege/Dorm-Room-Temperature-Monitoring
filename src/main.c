#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "./Sensor/sensor.h"
#include "./Database/database_create.h"
#include "./Database/database_insert.h"

int main(void) {
    if (create_database("sensor.db") != 0) {
        fprintf(stderr, "Failed to create database\n");
        return 1;
    }

    float temp, humidity;
    while (1) {
        if (get_measurement(SHT40_ADDR, &temp, &humidity) != 0) {
            fprintf(stderr, "Sensor read failed\n");
            break;
        }
        if (insert_sensor_reading("sensor.db", temp, humidity, "Sensor01") != 0) {
            fprintf(stderr, "DB insert failed\n");
            break;
        }

        printf("Recorded: %.2f°C, %.2f%%\n", tempf, humidity);
        sleep(1);
    }
    return 0;
}
