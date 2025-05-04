#include <stdio.h>
#include <stdlib.h>

#include "sensor.h"
#include "database_insert.h"
#include "database_create.h"

int main() {
    create_database("sensor.db");
    //create server
    float temp;
    float humidity;
    // loop (infinitely, or until command interrupt?)
        get_measurement(SHT40_ADDR, &temp, &humidity);
        insert_sensor_reading("sensor.db", temp, humidity, "Sensor 01");

    printf("Hello World!");
    print_measurements(SHT40_ADDR, 10, 1);
    return 0;

}
