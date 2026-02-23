TOOLCHAIN := tools/linux/toolchain/arm-rockchip830-linux-uclibcgnueabihf/bin

CC  := $(TOOLCHAIN)/arm-rockchip830-linux-uclibcgnueabihf-gcc

CXX := $(TOOLCHAIN)/arm-rockchip830-linux-uclibcgnueabihf-g++

CFLAGS   := -O2 -w 

CXXFLAGS := -O2 -w -DRV1106_1103

INCLUDES := \
	-I source/include \
	-I source/include/driver \
	-I rknn/include \
	-I opencv/include \
	-I 3rdparty/jpeg_turbo/include \
	-I 3rdparty/librga/include \
	-I 3rdparty/stb_image/include \
	-I utils/include \
	-I postprocess/include \
	-I yolov5/include

LIBS := \
	-L opencv/lib \
	-lopencv_highgui \
	-lopencv_imgproc \
	-lopencv_features2d \
	-lopencv_photo \
	-lopencv_video \
	-lopencv_core \
	-L rknn/lib \
	-lrknnmrt \
	3rdparty/jpeg_turbo/lib/libturbojpeg.a \
	-L 3rdparty/librga/lib \
	-lrga \
	-lpthread -ldl -lm

OUT_DIR := source/output
OBJ_DIR := $(OUT_DIR)/obj
TARGET  := $(OUT_DIR)/main

C_SRCS := \
	utils/src/file_utils.c \
	utils/src/image_utils.c

CPP_SRCS := \
	source/src/*.cxx \
	source/src/driver/*.cxx \
	source/src/display/*.cxx \
	source/src/usb/*.cxx \
	postprocess/src/*.cc \
	yolov5/src/*.cc

C_OBJS   := $(patsubst %.c,$(OBJ_DIR)/%.o,$(notdir $(C_SRCS)))
CPP_OBJS := $(patsubst %.cxx,$(OBJ_DIR)/%.o,$(notdir $(wildcard source/src/*.cxx))) \
            $(patsubst %.cxx,$(OBJ_DIR)/%.o,$(notdir $(wildcard source/src/driver/*.cxx))) \
            $(patsubst %.cxx,$(OBJ_DIR)/%.o,$(notdir $(wildcard source/src/display/*.cxx))) \
            $(patsubst %.cxx,$(OBJ_DIR)/%.o,$(notdir $(wildcard source/src/usb/*.cxx))) \
            $(patsubst %.cc,$(OBJ_DIR)/%.o,$(notdir $(wildcard postprocess/src/*.cc))) \
            $(patsubst %.cc,$(OBJ_DIR)/%.o,$(notdir $(wildcard yolov5/src/*.cc)))

OBJS := $(C_OBJS) $(CPP_OBJS)

all: $(TARGET)

$(TARGET): $(OBJS)
	@echo "Linking $@"
	$(CXX) $(OBJS) $(LIBS) -o $@

$(OBJ_DIR)/%.o: utils/src/%.c
	@mkdir -p $(OBJ_DIR)
	@echo "CC  $<"
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR)/%.o: source/src/%.cxx
	@mkdir -p $(OBJ_DIR)
	@echo "CXX $<"
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR)/%.o: source/src/driver/%.cxx
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR)/%.o: source/src/display/%.cxx
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR)/%.o: source/src/usb/%.cxx
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR)/%.o: postprocess/src/%.cc
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR)/%.o: yolov5/src/%.cc
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(OUT_DIR)

.PHONY: all clean