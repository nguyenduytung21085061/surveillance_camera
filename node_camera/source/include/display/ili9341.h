#ifndef _ILI9341_H_
#define _ILI9341_H_

#include <driver/spi.h>
#include <driver/gpio.h>
#include <stdint.h>

class ili9341 {
public:
    ili9341(spi_config_t spi_cfg, gpio_pin_t dc, gpio_pin_t rst, gpio_pin_t cs);
    ~ili9341();

    void init();
    void pixels(int y, int h);
    void set_dc(gpio_level_t level);
    void transmit(uint8_t *tx, int length);

private:
    spi spi0;
    gpio gpio_dc, gpio_rst, gpio_cs;

    void command(uint8_t c);
    void data(uint8_t c);
};

#endif