// tests/test_sensor.c
#include <criterion/criterion.h>
#include "../src/Sensor/sensor.h"

Test(convert, known_values) {
    // SHT40 datasheet example: raw = 0x6666 → T = -45 + 175*(0x6666/65535) = ~30°C
    unsigned char buf1[6] = { 0x66, 0x66, 0x00, 0x00, 0x00, 0x00 };
    float t = convert_temp(buf1);
    cr_expect_float_eq(t, -45 + 175.0f * (0x6666 / 65535.0f), 1e-3,
                       "convert_temp returned %g, expected %g", 
                       t, -45 + 175.0f * (0x6666 / 65535.0f));

    // For humidity: raw = 0x8000 → RH = 100 * (0x8000/65535) = 50%
    unsigned char buf2[6] = { 0, 0, 0, 0x80, 0x00, 0 };
    float h = convert_humidity(buf2);
    cr_expect_float_eq(h, 100.0f * (0x8000 / 65535.0f), 1e-3,
                       "convert_humidity returned %g, expected %g",
                       h, 100.0f * (0x8000 / 65535.0f));
}
