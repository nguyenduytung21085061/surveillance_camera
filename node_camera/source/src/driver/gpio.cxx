#include "driver/gpio.h"

gpio::gpio(gpio_pin_t pins, gpio_mode_t mode){
	this->pins = pins;
	this->mode = mode;
	this->export_gpio(this->pins);
	this->direction_gpio(this->pins, this->mode);
}

gpio::~gpio(){
	this->unexport_gpio(this->pins);
}

void gpio::export_gpio(gpio_pin_t pins){
	char buffer[23];
	int file_gpio = open("/sys/class/gpio/export", O_WRONLY);
	snprintf(buffer, sizeof(buffer), "%d", pins);
	write(file_gpio, buffer, strlen(buffer));
	close(file_gpio);
}

void gpio::unexport_gpio(gpio_pin_t pins){
	char buffer[23];
	int file_gpio = open("/sys/class/gpio/unexport", O_WRONLY);
	snprintf(buffer, sizeof(buffer), "%d", pins);
	write(file_gpio, buffer, strlen(buffer));
	close(file_gpio);
}

void gpio::direction_gpio(gpio_pin_t pins, gpio_mode_t mode){
	char buffer[30];
	snprintf(buffer, sizeof(buffer), "/sys/class/gpio/gpio%d/value", pins);
	int file_gpio = open(buffer, O_WRONLY);
	write(file_gpio, OUTPUT ? "out" : "in", OUTPUT ? 3 : 2);
	close(file_gpio);	
}