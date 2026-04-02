#ifndef _ILI9341_H_
#define _ILI9341_H_

#include <driver/spi.h>
#include <driver/gpio.h>

typedef enum {
    _WIDTH  = 320,
    _HEIGHT = 240,
    _NOP     = 0x00,
    _SWRESET = 0x01,
    _SLPOUT  = 0x11,
    _DISPON  = 0x29,
    _CASET   = 0x2A,
    _PASET   = 0x2B,
    _RAMWR   = 0x2C,
    _MADCTL  = 0x36,
    _COLMOD  = 0x3A,
    _MADCTL_MY  = 0x80,
    _MADCTL_MX  = 0x40,
    _MADCTL_MV  = 0x20,
    _MADCTL_BGR = 0x08,
    _RGB565 = 0x55,
} ili9341_cmd_t;

class ili9341 {
public:
    ili9341(spi_config_t spi_cfg, gpio_pin_t dc, gpio_pin_t rst, gpio_pin_t cs);
    ~ili9341();
    void init();
    void set_window(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
    void push_pixels(uint8_t *data, int len);
private:
    spi spi0;
    gpio gpio_dc, gpio_rst, gpio_cs;
    void command(uint8_t cmd);
    void data(uint8_t data);
};

#endif