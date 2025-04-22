#include <stdio.h>
#include <stdlib.h>

#include "sensor.h"

int main() {
    printf("Hello World!");
    print_measurements(SHT40_ADDR, 10, 1);
    return 0;

}
