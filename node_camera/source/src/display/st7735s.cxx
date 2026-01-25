#include "display/st7735s.h"

st7735s::st7735s(spi_config_t spi_cfg, gpio_pin_t dc, gpio_pin_t rst) {
	this->spi0.init(spi_cfg);
	this->gpio_dc.init(dc, OUTPUT);
	this->gpio_rst.init(rst, OUTPUT);
}

st7735s::~st7735s(){

}

void st7735s::init(){
	this->gpio_rst.set_level(LOW);
	usleep(100000);
	this->gpio_rst.set_level(HIGH);
	usleep(100000);
	this->command(_SLPOUT);
	usleep(150000);
	this->command(_MADCTL);
	this->data(_ROTATION);
	this->command(_COLMOD);
	this->data(_RGB565);
	this->command(_INVOFF);
	this->command(_DISPON);
}

void st7735s::command(uint8_t c){
	this->gpio_dc.set_level(LOW);
	this->spi0.transmit(&c);
}

void st7735s::data(uint8_t c){
	this->gpio_dc.set_level(HIGH);
	this->spi0.transmit(&c);
}	