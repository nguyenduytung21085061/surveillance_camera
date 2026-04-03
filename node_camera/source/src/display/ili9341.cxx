#include "display/ili9341.h"
#include <unistd.h>

#define WIDTH   320
#define HEIGHT  240
#define SPI_CHUNK 4096
#define _SLPOUT   0x11
#define _DISPON   0x29
#define _CASET    0x2A
#define _RASET    0x2B
#define _RAMWR    0x2C
#define _MADCTL   0x36
#define _COLMOD   0x3A
#define _MADCTL_MV  0x20
#define _MADCTL_MX  0x40
#define _MADCTL_MY  0x80
#define _MADCTL_BGR 0x08
#define _RGB565     0x55

ili9341::ili9341(spi_config_t spi_cfg, gpio_pin_t dc, gpio_pin_t rst, gpio_pin_t cs) {
    spi0.init(spi_cfg);
    gpio_dc.init(dc, OUTPUT);
    gpio_rst.init(rst, OUTPUT);
    gpio_cs.init(cs, OUTPUT);
    usleep(100000);
    gpio_dc.export_gpio();
    gpio_dc.direction_gpio();
    gpio_rst.export_gpio();
    gpio_rst.direction_gpio();
    gpio_cs.export_gpio();
    gpio_cs.direction_gpio();
    gpio_cs.set_level(HIGH);
}

ili9341::~ili9341() {}

void ili9341::init() {
    gpio_cs.set_level(HIGH);
    gpio_rst.set_level(LOW);
    usleep(100000);
    gpio_rst.set_level(HIGH);
    usleep(150000);
    command(_SLPOUT);
    usleep(120000);
    command(_COLMOD);
    data(_RGB565);
    command(_MADCTL);
    data(_MADCTL_MV | _MADCTL_MY | _MADCTL_BGR);
    command(_DISPON);
}

void ili9341::pixels(int y, int h) {
    command(_CASET);
    data(0 >> 8);
    data(0 & 0xFF);
    data((WIDTH - 1) >> 8);
    data((WIDTH - 1) & 0xFF);
    command(_RASET);
    data(y >> 8);
    data(y & 0xFF);
    data((y + h - 1) >> 8);
    data((y + h - 1) & 0xFF);
    command(_RAMWR);
}

void ili9341::set_dc(gpio_level_t level) {
    gpio_dc.set_level(level);
}

void ili9341::transmit(uint8_t *tx, int length) {
    gpio_cs.set_level(LOW);
    while (length > 0) {
        int chunk = (length > SPI_CHUNK) ? SPI_CHUNK : length;
        spi0.transmit(tx, chunk);
        tx += chunk;
        length -= chunk;
    }
    gpio_cs.set_level(HIGH);
}

void ili9341::command(uint8_t c) {
    gpio_dc.set_level(LOW);
    gpio_cs.set_level(LOW);
    spi0.transmit(&c, 1);
    gpio_cs.set_level(HIGH);
}

void ili9341::data(uint8_t c) {
    gpio_dc.set_level(HIGH);
    gpio_cs.set_level(LOW);
    spi0.transmit(&c, 1);
    gpio_cs.set_level(HIGH);
}