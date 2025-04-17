#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>

#define I2C_DEV "/dev/i2c-1"
#define SHT40_ADDR 0x44
#define SHT40_MEASURE_HIGH_PREC 0xFD

int main() {
    int fd = open(I2C_DEV, O_RDWR);
    if (fd < 0) {
        perror("Failed to open I2C bus");
        return 1;
    }

    if (ioctl(fd, I2C_SLAVE, SHT40_ADDR) < 0) {
        perror("Failed to select I2C device");
        close(fd);
        return 1;
    }

    // Send measurement command
    unsigned char cmd = SHT40_MEASURE_HIGH_PREC;
    if (write(fd, &cmd, 1) != 1) {
        perror("Failed to send measurement command");
        close(fd);
        return 1;
    }

    usleep(10000);  // wait 10ms for measurement

    // Read 6 bytes (2 temp, 1 CRC, 2 humidity, 1 CRC)
    unsigned char buf[6];
    if (read(fd, buf, 6) != 6) {
        perror("Failed to read data");
        close(fd);
        return 1;
    }

    // Convert temperature
    uint16_t temp_raw = (buf[0] << 8) | buf[1];
    float temp = -45 + 175 * ((float)temp_raw / 65535.0);

    // Convert humidity
    uint16_t hum_raw = (buf[3] << 8) | buf[4];
    float humidity = 100 * ((float)hum_raw / 65535.0);

    printf("Temperature: %.2f°C\n", temp);
    printf("Humidity: %.2f%%\n", humidity);

    close(fd);
    return 0;
}
