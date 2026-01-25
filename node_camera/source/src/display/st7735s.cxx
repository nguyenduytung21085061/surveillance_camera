#include "display/st7735s.h"

st7735s::st7735s(spi_config_t spi_cfg, gpio_pin_t dc, gpio_pin_t rst, gpio_pin_t cs) {
	this->spi0.init(spi_cfg);
	this->gpio_dc.init(dc, OUTPUT);
	this->gpio_rst.init(rst, OUTPUT);
	this->gpio_cs.init(cs, OUTPUT);
	usleep(100000);
	this->gpio_dc.export_gpio();
	usleep(100000);
	this->gpio_dc.direction_gpio();
	usleep(100000);
	this->gpio_rst.export_gpio();
	usleep(100000);
	this->gpio_rst.direction_gpio();
	usleep(100000);
	this->gpio_cs.export_gpio();
	usleep(100000);
	this->gpio_cs.direction_gpio();
}

st7735s::~st7735s(){

}

void st7735s::init(){
	this->gpio_cs.set_level(LOW);
	usleep(100000);
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

void st7735s::pixels(int y, int h){
	this->command(_CASET);
	this->data(_NOP);
	this->data(_NOP);
	this->data(_NOP);
	this->data(_WIDTH - 1);
	this->command(_RASET);
	this->data(_NOP);
	this->data(y);
	this->data(_NOP);
	this->data(y + h - 1);
	this->command(_RAMWR);
}

void st7735s::set_dc(gpio_level_t level){
	this->gpio_dc.set_level(level);
}

void st7735s::transmit(uint8_t *tx, int length){
	this->spi0.transmit(tx, length);
}

void st7735s::command(uint8_t c){
	this->gpio_dc.set_level(LOW);
	this->spi0.transmit(&c, 1);
}

void st7735s::data(uint8_t c){
	this->gpio_dc.set_level(HIGH);
	this->spi0.transmit(&c, 1);
}	