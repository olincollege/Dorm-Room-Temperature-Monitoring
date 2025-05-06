#include <stdio.h>
#include "./Database/database_create.h"

int main() {
    const char *filename = "test_sensor.db";
    int result = create_database(filename);

    if (result == 0) {
        printf("Database '%s' created successfully.\n", filename);
    } else if (result == 1) {
        printf("Error: Failed to open database.\n");
    } else if (result == 2) {
        printf("Error: Failed to create table.\n");
    }

    return result;
}
