#include <stdio.h>
#include <stdlib.h>

#include "sensor.h"

int main() {
    //create database
    //create server
    float* temp;
    float* humidity;
    // loop (infinitely, or until command interrupt?)
        get_measurement(SHT40_ADDR, temp, humidity);
        // insert temp and humidity readings into database


    printf("Hello World!");
    print_measurements(SHT40_ADDR, 10, 1);
    return 0;

}
