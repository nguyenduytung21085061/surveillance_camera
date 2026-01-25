#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/mman.h>
#include <linux/videodev2.h>
#include <rknn_api.h>
#include <pthread.h>
#include "display/st7735s.h"
#include "gpio.h"

struct usb_rapoo_buffer {
	void *address;
	size_t length;
}usb_rapoo_buffer[4];

uint8_t yuv422[160 * 10 * 2];
uint8_t rgb565[160 * 10 * 2];
uint8_t rgb888[160 * 128 * 3];

static inline uint16_t yuv_to_rgb(int y, int u, int v) {
    int c = y - 16;
    int d = u - 128;
    int e = v - 128;
    int r = (298 * c + 409 * e + 128) >> 8;
    int g = (298 * c - 100 * d - 208 * e + 128) >> 8;
    int b = (298 * c + 516 * d + 128) >> 8;
    if (r < 0) r = 0; else if (r > 255) r = 255;
    if (g < 0) g = 0; else if (g > 255) g = 255;
    if (b < 0) b = 0; else if (b > 255) b = 255;
    return (uint16_t)(( (r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3));
}

static inline void yuyv_to_rgb888(uint8_t *yuyv, uint8_t *rgb, int width, int height) {
    int p = 0;
    for (int i = 0; i < width * height * 2; i += 4) {
        int y0 = yuyv[i];
        int u  = yuyv[i + 1];
        int y1 = yuyv[i + 2];
        int v  = yuyv[i + 3];
        int c = y0 - 16;
        int d = u - 128;
        int e = v - 128;
        rgb[p++] = (298 * c + 409 * e + 128) >> 8;
        rgb[p++] = (298 * c - 100 * d - 208 * e + 128) >> 8;
        rgb[p++] = (298 * c + 516 * d + 128) >> 8;
        c = y1 - 16;
        rgb[p++] = (298 * c + 409 * e + 128) >> 8;
        rgb[p++] = (298 * c - 100 * d - 208 * e + 128) >> 8;
        rgb[p++] = (298 * c + 516*d + 128) >> 8;
    }
}


void frames(uint8_t *src, int lines) {
    int p = 0;
    int total_bytes_in = 160 * lines * 2;
    for (int i = 0; i < total_bytes_in; i += 4) {
        int y0 = src[i];
        int u  = src[i+1];
        int y1 = src[i+2];
        int v  = src[i+3];
        uint16_t pixel1 = yuv_to_rgb(y0, u, v);
        rgb565[p++] = (uint8_t)(pixel1 >> 8); 
        rgb565[p++] = (uint8_t)(pixel1 & 0xFF);
        uint16_t pixel2 = yuv_to_rgb(y1, u, v);        
        rgb565[p++] = (uint8_t)(pixel2 >> 8); 
        rgb565[p++] = (uint8_t)(pixel2 & 0xFF);
    }
}

int main(int argc, char const *argv[]){
	/* code */
	spi_config_t spi_cfg = {
		.dev = "/dev/spidev0.0",
		.mode = SPI_MODE_0,
		.bits = 8,
		.speeds = _48MHZ
	};
	st7735s tft(spi_cfg, GPIO4_B0_D, GPIO0_A4_D, GPIO4_B1_D);
    tft.init();
	// init camera usb uvc rapoo
	int fd = open("/dev/video0", O_RDWR);
	struct v4l2_capability cap;
    ioctl(fd, VIDIOC_QUERYCAP, &cap);
    struct v4l2_format fmt = {0};
    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.width = 160;
    fmt.fmt.pix.height = 128;
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
    ioctl(fd, VIDIOC_S_FMT, &fmt);
    struct v4l2_requestbuffers req = {0};
    req.count = 4;
    req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory = V4L2_MEMORY_MMAP;
    ioctl(fd, VIDIOC_REQBUFS, &req);
    for (int i = 0; i < 4; i++) {
        struct v4l2_buffer b = {0};
        b.type = req.type;
        b.memory = V4L2_MEMORY_MMAP;
        b.index = i;
        ioctl(fd, VIDIOC_QUERYBUF, &b);
        usb_rapoo_buffer[i].length = b.length;
        usb_rapoo_buffer[i].address = mmap(NULL, b.length, PROT_READ, MAP_SHARED, fd, b.m.offset);
        ioctl(fd, VIDIOC_QBUF, &b);
    }
    enum v4l2_buf_type t = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    ioctl(fd, VIDIOC_STREAMON, &t);
	// end camera usb uvc rapoo
	while (1) {
		struct v4l2_buffer b = {0};
        b.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        b.memory = V4L2_MEMORY_MMAP;
        ioctl(fd, VIDIOC_DQBUF, &b);
        uint8_t *frame = (uint8_t *)usb_rapoo_buffer[b.index].address;
        for (int y = 0; y < 128; y += 10) {
            int lines = (y + 10 <= 128) ? 10 : (128 - y);
            memcpy(yuv422, frame + y * 160 * 2, 160 * lines * 2);
            frames(yuv422, lines);
            tft.pixels(y, lines);
            tft.set_dc(HIGH);
            tft.transmit(rgb565, 160 * lines * 2);
        }
        ioctl(fd, VIDIOC_QBUF, &b);
	}
	return 0;
}