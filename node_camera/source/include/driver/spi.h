#ifndef _SPI_H_
#define _SPI_H_ 

#include <stdint.h>
#include <stdlib.h>
#include <cstdio>
#include <fcntl.h>
#include <unistd.h>
#include <linux/spi/spidev.h>
#include <sys/ioctl.h>

typedef enum {
    _10KHZ  = 10000,
    _50KHZ  = 50000,
    _100KHZ = 100000,
    _200KHZ = 200000,
    _250KHZ = 250000,
    _400KHZ = 400000,
    _500KHZ = 500000,
    _1MHZ   = 1000000,
    _2MHZ   = 2000000,
    _4MHZ   = 4000000,
    _8MHZ   = 8000000,
    _10MHZ  = 10000000,
    _12MHZ  = 12000000,
    _16MHZ  = 16000000,
    _20MHZ  = 20000000,
    _24MHZ  = 24000000,
    _25MHZ  = 25000000,
    _30MHZ  = 30000000,
    _40MHZ  = 40000000,
    _48MHZ  = 48000000
} spi_speed_t;


typedef struct {
	char dev[15];
	uint8_t mode;
	uint8_t bits;
    spi_speed_t speeds;
} spi_config_t;

class spi {
    public:
        spi(spi_config_t config);
        ~spi();
        void transmit(uint8_t *tx);
        uint8_t receive(void);
        uint8_t transfer(uint8_t *tx);
    private:
        spi_config_t config;
        int fd;
};

#endif