#include <stdio.h>
#include <stdlib.h>

#include "sensor.h"

int main() {
    
    print_measurements(I2C_DEV, SHT40_ADDR, SHT40_MEASURE_HIGH_PREC, 10, 1);
    printf("Hello World!");
    return 0;

}
