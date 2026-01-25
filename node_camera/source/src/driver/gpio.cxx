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

void gpio::set_level(gpio_level_t level){
	char buffer[30];
	snprintf(buffer, sizeof(buffer), "/sys/class/gpio/gpio%d/value", pins);
	int fd = open(buffer, O_WRONLY);
	write(fd, level ? "1" : "0", 1);
	close(fd);
}

bool gpio::get_level(){
	char buffer[30], value;
	snprintf(buffer, sizeof(buffer), "/sys/class/gpio/gpio%d/value", pins);
	int fd = open(buffer, O_RDONLY);
	read(fd, &value, 1);
	close(fd);
	return (value == '1') ? 1 : 0;
}

void gpio::export_gpio(gpio_pin_t pins){
	char buffer[23];
	int fd = open("/sys/class/gpio/export", O_WRONLY);
	snprintf(buffer, sizeof(buffer), "%d", pins);
	write(fd, buffer, strlen(buffer));
	close(fd);
}

void gpio::unexport_gpio(gpio_pin_t pins){
	char buffer[23];
	int fd = open("/sys/class/gpio/unexport", O_WRONLY);
	snprintf(buffer, sizeof(buffer), "%d", pins);
	write(fd, buffer, strlen(buffer));
	close(fd);
}

void gpio::direction_gpio(gpio_pin_t pins, gpio_mode_t mode){
	char buffer[34];
	snprintf(buffer, sizeof(buffer), "/sys/class/gpio/gpio%d/direction", pins);
	int fd = open(buffer, O_WRONLY);
	write(fd, mode ? "out" : "in", mode ? 3 : 2);
	close(fd);	
}