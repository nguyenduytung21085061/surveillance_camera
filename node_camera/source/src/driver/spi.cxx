#include "driver/spi.h"

spi::spi(){
	
}

spi::~spi(){
	close(this->fd);
}

void spi::init(spi_config_t config){
    this->config = config;
    this->fd = open(this->config.dev, O_RDWR);
    ioctl(this->fd, SPI_IOC_WR_MODE, &this->config.mode);
    ioctl(this->fd, SPI_IOC_WR_BITS_PER_WORD, &this->config.bits);
    ioctl(this->fd, SPI_IOC_WR_MAX_SPEED_HZ, &this->config.speeds);
}

void spi::transmit(uint8_t *tx){
	struct spi_ioc_transfer transfer = {
        .tx_buf = (unsigned long)tx,
        .len = sizeof(tx),
        .speed_hz = this->config.speeds,
        .delay_usecs = 0,
        .bits_per_word = 8,
    };
    if (ioctl(this->fd, SPI_IOC_MESSAGE(1), &transfer) < 0) {
        perror("Failed to perform SPI transfer");
    }
}

uint8_t spi::receive(void){
	uint8_t rx;	
	struct spi_ioc_transfer transfer = {
        .rx_buf = (unsigned long)&rx,
        .len = sizeof(rx),
        .speed_hz = this->config.speeds,
        .delay_usecs = 0,
        .bits_per_word = 8,
    };
    if (ioctl(this->fd, SPI_IOC_MESSAGE(1), &transfer) < 0) {
        perror("Failed to perform SPI transfer");
    }
    return rx;
}

uint8_t spi::transfer(uint8_t *tx){
	uint8_t rx;	
	struct spi_ioc_transfer transfer = {
		.tx_buf = (unsigned long)tx,
        .rx_buf = (unsigned long)&rx,
        .len = sizeof(tx),
        .speed_hz = this->config.speeds,
        .delay_usecs = 0,
        .bits_per_word = 8,
    };
    if (ioctl(this->fd, SPI_IOC_MESSAGE(1), &transfer) < 0) {
        perror("Failed to perform SPI transfer");
    }
    return rx;
}
