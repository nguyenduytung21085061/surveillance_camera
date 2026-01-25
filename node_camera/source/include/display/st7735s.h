#ifndef _ST7735S_H
#define _ST7735S_H 

#include <driver/spi.h>
#include <driver/gpio.h>

typedef enum {
	_MADCTL_MY  = 0x80,
    _MADCTL_MX  = 0x40,
    _MADCTL_MV  = 0x20,
    _MADCTL_ML  = 0x10,
    _MADCTL_RGB = 0x00,
    _MADCTL_BGR = 0x08,
    _MADCTL_MH  = 0x04,
    _IS_160X128 = 1,
    // _WIDTH      = 128,
    // _HEIGHT     = 160,
    _WIDTH      = 160,
    _HEIGHT    	= 128,
    _XSTART     = 0,
    _YSTART     = 0,
    // _ROTATION   = (_MADCTL_MX | _MADCTL_MY),
    _ROTATION   = (_MADCTL_MY | _MADCTL_MV),
    _NOP        = 0x00,
    _SWRESET    = 0x01,
    _RDDID      = 0x04,
    _RDDST      = 0x09,
    _SLPIN      = 0x10,
    _SLPOUT     = 0x11,
    _PTLON      = 0x12,
    _NORON      = 0x13,
    _INVOFF     = 0x20,
    _INVON      = 0x21,
    _GAMSET     = 0x26,
    _DISPOFF    = 0x28,
    _DISPON     = 0x29,
    _CASET      = 0x2A,
    _RASET      = 0x2B,
    _RAMWR      = 0x2C,
    _RAMRD      = 0x2E,
    _PTLAR      = 0x30,
    _COLMOD     = 0x3A,
    _MADCTL     = 0x36,
    _FRMCTR1    = 0xB1,
    _FRMCTR2    = 0xB2,
    _FRMCTR3    = 0xB3,
    _INVCTR     = 0xB4,
    _DISSET5    = 0xB6,
    _PWCTR1     = 0xC0,
    _PWCTR2     = 0xC1,
    _PWCTR3     = 0xC2,
    _PWCTR4     = 0xC3,
    _PWCTR5     = 0xC4,
    _VMCTR1     = 0xC5,
    _RDID1      = 0xDA,
    _RDID2      = 0xDB,
    _RDID3      = 0xDC,
    _RDID4      = 0xDD,
    _PWCTR6     = 0xFC,
    _GMCTRP1    = 0xE0,
    _GMCTRN1    = 0xE1,
    _BLACK      = 0x0000,
    _BLUE       = 0x001F,
    _RED        = 0xF800,
    _GREEN      = 0x07E0,
    _CYAN       = 0x07FF,
    _MAGENTA    = 0xF81F,
    _YELLOW     = 0xFFE0,
    _WHITE      = 0xFFFF,
    _RGB565     = 0x05,
    GAMMA_10    = 0x01,
    GAMMA_25    = 0x02,
    GAMMA_22    = 0x04,
    GAMMA_18    = 0x08,
    _DELAY      = 0x80,
} st7735s_address_t;

class st7735s {
	public:
		st7735s(spi_config_t spi_cfg, gpio_pin_t dc, gpio_pin_t rst);
		~st7735s();
        void init();
        void pixels(int y, int h);
        void set_dc(gpio_level_t level);
        void transmit(uint8_t *tx);
	private:
		spi spi0;
		gpio gpio_dc, gpio_rst;
		void command(uint8_t c);
		void data(uint8_t c);
};

#endif