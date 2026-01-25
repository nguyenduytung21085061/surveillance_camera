#include "driver/gpio.h"

gpio::gpio(){
	
}

gpio::~gpio(){
	// this->unexport_gpio(this->pins);
}

void gpio::init(gpio_pin_t pins, gpio_mode_t mode){
	this->pins = pins;
	this->mode = mode;
	printf("GPIO init pins - mode: %d - %d\r\n", this->pins, this->mode);
}

void gpio::set_level(gpio_level_t level){
	char buffer[30];
	snprintf(buffer, sizeof(buffer), "/sys/class/gpio/gpio%d/value", this->pins);
	int fd = open(buffer, O_WRONLY);
	write(fd, level ? "1" : "0", 1);
	close(fd);
}

bool gpio::get_level(){
	char buffer[30], value;
	snprintf(buffer, sizeof(buffer), "/sys/class/gpio/gpio%d/value", this->pins);
	int fd = open(buffer, O_RDONLY);
	read(fd, &value, 1);
	close(fd);
	return (value == '1') ? 1 : 0;
}

void gpio::export_gpio(){
	char buffer[5];
	int fd = open("/sys/class/gpio/export", O_WRONLY);
	if (fd < 0) {
	    perror("open export");
	    return;
	}
	snprintf(buffer, sizeof(buffer), "%d", this->pins);
	write(fd, buffer, strlen(buffer));
	close(fd);
}

void gpio::unexport_gpio(){
	char buffer[5];
	int fd = open("/sys/class/gpio/unexport", O_WRONLY);
	snprintf(buffer, sizeof(buffer), "%d", this->pins);
	write(fd, buffer, strlen(buffer));
	close(fd);
}

void gpio::direction_gpio(){
	char buffer[35];
	snprintf(buffer, sizeof(buffer), "/sys/class/gpio/gpio%d/direction", this->pins);
	int fd = open(buffer, O_WRONLY);
	if (fd < 0) {
	    perror("open direction");
	    return;
	}
	write(fd, (this->mode == OUTPUT) ? "out" : "in", (this->mode == OUTPUT) ? 3 : 2);
	close(fd);	
}