#include "sensor.h"

#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>


int open_and_connect(char* dev, int addr){
    int fd = open(dev, O_RDWR);
    if (fd < 0) {
        perror("Failed to open I2C bus");
        return 1;
    }

    if (ioctl(fd, I2C_SLAVE, addr) < 0) {
        perror("Failed to select I2C device");
        close(fd);
        return 1;
    }
    return fd;
}

int get_measurement(int addr, float* temp, float* humidity){
    int fd = open_and_connect(I2C_DEV, addr);
    
    // Send measurement command
    unsigned char cmd = SHT40_MEASURE_HIGH_PREC;
    if (write(fd, &cmd, 1) != 1) {
        perror("Failed to send measurement command");
        close(fd);
        return 1;
    }
    // wait 10ms for measurement
    usleep(10000);

    // Read 6 bytes (2 temp, 1 CRC, 2 humidity, 1 CRC)
    unsigned char buf[6];
    if (read(fd, buf, 6) != 6) {
        perror("Failed to read data");
        close(fd);
        return 1;
    }

    // Convert temperature
    uint16_t temp_raw = (buf[0] << 8) | buf[1];
    *temp = -45 + 175 * ((float)temp_raw / 65535.0);

    // Convert humidity
    uint16_t hum_raw = (buf[3] << 8) | buf[4];
    *humidity = 100 * ((float)hum_raw / 65535.0);

    close(fd);
    return 0;
}

int print_measurements(int addr, size_t measurements, size_t delay){
    float temp;
    float humidity;
    for(size_t i = 0; i < measurements; ++i){
        get_measurement(addr, &temp, &humidity);
        printf("Temperature: %.2f°C\n", temp);
        printf("Humidity: %.2f%%\n", humidity);
        sleep(delay);
    }
    return 0;
}


