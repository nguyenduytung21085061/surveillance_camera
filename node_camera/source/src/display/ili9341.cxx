#include "display/ili9341.h"
#include <unistd.h>

ili9341::ili9341(spi_config_t spi_cfg, gpio_pin_t dc, gpio_pin_t rst, gpio_pin_t cs) {
    this->spi0.init(spi_cfg);
    this->gpio_dc.init(dc, OUTPUT);
    this->gpio_rst.init(rst, OUTPUT);
    this->gpio_cs.init(cs, OUTPUT);
    usleep(100000);
    this->gpio_dc.export_gpio();
    this->gpio_dc.direction_gpio();
    this->gpio_rst.export_gpio();
    this->gpio_rst.direction_gpio();
    this->gpio_cs.export_gpio();
    this->gpio_cs.direction_gpio();
}

ili9341::~ili9341(){}

void ili9341::init() {
    gpio_cs.set_level(LOW);
    gpio_rst.set_level(LOW);
    usleep(100000);
    gpio_rst.set_level(HIGH);
    usleep(150000);
    command(_SWRESET);
    usleep(150000);
    command(_SLPOUT);
    usleep(150000);
    command(_COLMOD);
    data(_RGB565);
    command(_MADCTL);
    data(_MADCTL_MV | _MADCTL_MX | _MADCTL_BGR);
    command(_DISPON);
    usleep(100000);
}

void ili9341::set_window(uint16_t x, uint16_t y, uint16_t w, uint16_t h) {
    command(_CASET);
    data(x >> 8);
    data(x & 0xFF);
    data((x + w - 1) >> 8);
    data((x + w - 1) & 0xFF);
    command(_PASET);
    data(y >> 8);
    data(y & 0xFF);
    data((y + h - 1) >> 8);
    data((y + h - 1) & 0xFF);
    command(_RAMWR);
}

void ili9341::push_pixels(uint8_t *data_buf, int len) {
    gpio_dc.set_level(HIGH);
    spi0.transmit(data_buf, len);
}

void ili9341::command(uint8_t cmd) {
    gpio_dc.set_level(LOW);
    spi0.transmit(&cmd, 1);
}

void ili9341::data(uint8_t data) {
    gpio_dc.set_level(HIGH);
    spi0.transmit(&data, 1);
}