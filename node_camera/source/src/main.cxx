#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/videodev2.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "display/st7735s.h"

struct cam_buf {
    void*  addr;
    size_t len;
};

static cam_buf buffers[4];

static inline uint16_t bgr_to_rgb565(uint8_t b, uint8_t g, uint8_t r){
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

int main(){
    spi_config_t spi_cfg = {
        .dev    = "/dev/spidev0.0",
        .mode   = SPI_MODE_0,
        .bits   = 8,
        .speeds = _48MHZ
    };
    st7735s tft(spi_cfg, GPIO4_B0_D, GPIO0_A4_D, GPIO4_B1_D);
    tft.init();
    int fd = open("/dev/video0", O_RDWR);
    if (fd < 0) {
        perror("open video");
        return -1;
    }
    struct v4l2_format fmt{};
    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.width       = 640;
    fmt.fmt.pix.height      = 480;
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_MJPEG;
    fmt.fmt.pix.field       = V4L2_FIELD_ANY;
    if (ioctl(fd, VIDIOC_S_FMT, &fmt) < 0) {
        perror("VIDIOC_S_FMT");
        return -1;
    }
    struct v4l2_requestbuffers req{};
    req.count  = 4;
    req.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory = V4L2_MEMORY_MMAP;
    ioctl(fd, VIDIOC_REQBUFS, &req);
    for (int i = 0; i < 4; i++) {
        struct v4l2_buffer buf{};
        buf.type   = req.type;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index  = i;
        ioctl(fd, VIDIOC_QUERYBUF, &buf);
        buffers[i].len  = buf.length;
        buffers[i].addr = mmap(NULL, buf.length, PROT_READ, MAP_SHARED, fd, buf.m.offset);
        ioctl(fd, VIDIOC_QBUF, &buf);
    }
    enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    ioctl(fd, VIDIOC_STREAMON, &type);
    cv::Mat bgr, resized;
    static uint8_t rgb565[160 * 128 * 2];
    while (1) {
        struct v4l2_buffer buf{};
        buf.type   = type;
        buf.memory = V4L2_MEMORY_MMAP;
        ioctl(fd, VIDIOC_DQBUF, &buf);
        cv::Mat mjpeg(1, buf.bytesused, CV_8UC1, buffers[buf.index].addr);
        bgr = cv::imdecode(mjpeg, cv::IMREAD_COLOR);
        if (!bgr.empty()) {
            cv::resize(bgr, resized, cv::Size(160, 128));
            int p = 0;
            for (int y = 0; y < 128; y++) {
                cv::Vec3b* row = resized.ptr<cv::Vec3b>(y);
                for (int x = 0; x < 160; x++) {
                    uint16_t c = bgr_to_rgb565( row[x][0], row[x][1], row[x][2]);
                    rgb565[p++] = c >> 8;
                    rgb565[p++] = c & 0xFF;
                }
            }
            for (int y = 0; y < 128; y += 10) {
                int lines = (y + 10 <= 128) ? 10 : (128 - y);
                tft.pixels(y, lines);
                tft.set_dc(HIGH);
                tft.transmit(&rgb565[y * 160 * 2], 160 * lines * 2);
            }
        }
        ioctl(fd, VIDIOC_QBUF, &buf);
    }
    return 0;
}