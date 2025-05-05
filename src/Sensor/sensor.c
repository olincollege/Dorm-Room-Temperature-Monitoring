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

float convert_temp(unsigned char bytes_read[6]){
    uint16_t temp_raw = (bytes_read[0] << 8) | bytes_read[1];
    return -45 + 175 * ((float)temp_raw / 65535.0);
}

float convert_humidity(unsigned char bytes_read[6]){
    uint16_t hum_raw = (bytes_read[3] << 8) | bytes_read[4];
    return 100 * ((float)hum_raw / 65535.0);
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

    *temp = convert_temp(buf);
    *humidity = convert_humidity(buf);

    close(fd);
    return 0;
}

int print_measurements(int addr, size_t measurements, size_t delay){
    float tempc;
    float tempf;
    float humidity;
    for(size_t i = 0; i < measurements; ++i){
        get_measurement(addr, &tempc, &humidity);
        tempf = tempc * 9 / 5 + 32;
        printf("Temperature: %.2f°C, %.2f°F\n", tempc, tempf);
        printf("Humidity: %.2f%%\n", humidity);
        sleep(delay);
    }
    return 0;
}


