#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/videodev2.h>
#include <time.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <yolov5.h>
#include <image_utils.h>
#include <file_utils.h>
#include <image_drawing.h>
#include <display/st7735s.h>

#define MODEL_W         640
#define MODEL_H         640
#define CAM_W           640
#define CAM_H           480
#define TFT_W           160
#define TFT_H           128
#define YOLO_SKIP       2 
#define TARGET_FPS      10
#define PERSON_ID       0
#define CONF_THRESH     0.45
#define COCO_CLASS_NUM  80

static const char* coco_labels[] = {
    "person","bicycle","car","motorcycle","airplane","bus","train","truck","boat",
    "traffic light","fire hydrant","stop sign","parking meter","bench","bird","cat",
    "dog","horse","sheep","cow","elephant","bear","zebra","giraffe","backpack",
    "umbrella","handbag","tie","suitcase","frisbee","skis","snowboard","sports ball",
    "kite","baseball bat","baseball glove","skateboard","surfboard","tennis racket",
    "bottle","wine glass","cup","fork","knife","spoon","bowl","banana","apple",
    "sandwich","orange","broccoli","carrot","hot dog","pizza","donut","cake","chair",
    "couch","potted plant","bed","dining table","toilet","tv","laptop","mouse",
    "remote","keyboard","cell phone","microwave","oven","toaster","sink",
    "refrigerator","book","clock","vase","scissors","teddy bear","hair drier","toothbrush"
};

const char* coco_cls_to_names(int cls_id){
    if (cls_id < 0 || cls_id >= COCO_CLASS_NUM)
        return "unknown";
    return coco_labels[cls_id];
}

struct cam_buf {
    void*  addr;
    size_t len;
};

static cam_buf buffers[4];

static inline uint16_t bgr_to_rgb565(uint8_t b, uint8_t g, uint8_t r){
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

static inline void mapCoordinates(const cv::Mat& input, const cv::Mat& output, int *x, int *y){
    float scaleX = (float)output.cols / (float)input.cols;
    float scaleY = (float)output.rows / (float)input.rows;
    *x = (int)((float)(*x) / scaleX);
    *y = (int)((float)(*y) / scaleY);
}

int main(int argc, char **argv){
    if (argc != 2) {
        printf("Usage: %s yolov5.rknn\n", argv[0]);
        return -1;
    }
    spi_config_t spi_cfg = {
        .dev    = "/dev/spidev0.0",
        .mode   = SPI_MODE_0,
        .bits   = 8,
        .speeds = _48MHZ
    };
    st7735s tft(spi_cfg, GPIO4_B0_D, GPIO0_A4_D, GPIO4_B1_D);
    tft.init();
    static uint8_t rgb565[TFT_W * TFT_H * 2];
    int fd = open("/dev/video0", O_RDWR);
    if (fd < 0) {
        perror("open video");
        return -1;
    }
    struct v4l2_format fmt{};
    fmt.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.width       = CAM_W;
    fmt.fmt.pix.height      = CAM_H;
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_MJPEG;
    fmt.fmt.pix.field       = V4L2_FIELD_ANY;
    ioctl(fd, VIDIOC_S_FMT, &fmt);
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
    rknn_app_context_t rknn_app_ctx;
    object_detect_result_list od_results;
    memset(&rknn_app_ctx, 0, sizeof(rknn_app_ctx));
    init_yolov5_model(argv[1], &rknn_app_ctx);
    // init_post_process();
    cv::Mat model_input(MODEL_H, MODEL_W, CV_8UC3, rknn_app_ctx.input_mems[0]->virt_addr);
    cv::Mat bgr, resized;
    int frame_id = 0;
    while (1) {
        struct v4l2_buffer buf{};
        buf.type   = type;
        buf.memory = V4L2_MEMORY_MMAP;
        ioctl(fd, VIDIOC_DQBUF, &buf);
        cv::Mat mjpeg(1, buf.bytesused, CV_8UC1, buffers[buf.index].addr
        );
        bgr = cv::imdecode(mjpeg, cv::IMREAD_COLOR);
        if (!bgr.empty()) {
            if (frame_id % YOLO_SKIP == 0) {
                cv::resize(bgr, model_input, cv::Size(MODEL_W, MODEL_H));
                inference_yolov5_model(&rknn_app_ctx, &od_results);
                for (int i = 0; i < od_results.count; i++) {
                    auto *det = &od_results.results[i];
                    if (det->cls_id != PERSON_ID)
                        continue;
                    if (det->prop < CONF_THRESH)
                        continue;
                    mapCoordinates(bgr, model_input, &det->box.left, &det->box.top);
                    mapCoordinates(bgr, model_input, &det->box.right, &det->box.bottom);
                    printf("%s %.2f\n", coco_cls_to_names(det->cls_id), det->prop);
                    cv::rectangle(bgr, cv::Point(det->box.left, det->box.top), cv::Point(det->box.right, det->box.bottom), cv::Scalar(0, 255, 0), 2);
                }
            }
            cv::resize(bgr, resized, cv::Size(TFT_W, TFT_H));
            int p = 0;
            for (int y = 0; y < TFT_H; y++) {
                cv::Vec3b* row = resized.ptr<cv::Vec3b>(y);
                for (int x = 0; x < TFT_W; x++) {
                    uint16_t c = bgr_to_rgb565(row[x][0], row[x][1], row[x][2]);
                    rgb565[p++] = c >> 8;
                    rgb565[p++] = c & 0xFF;
                }
            }
            for (int y = 0; y < TFT_H; y += 10) {
                int lines = (y + 10 <= TFT_H) ? 10 : (TFT_H - y);
                tft.pixels(y, lines);
                tft.set_dc(HIGH);
                tft.transmit(&rgb565[y * TFT_W * 2], TFT_W * lines * 2);
            }
        }
        ioctl(fd, VIDIOC_QBUF, &buf);
        frame_id++;
    }
    deinit_post_process();
    release_yolov5_model(&rknn_app_ctx);
    return 0;
}