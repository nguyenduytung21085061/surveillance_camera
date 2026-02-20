# 📷 Hệ Thống Camera Giám Sát Thông Minh Cho Gia Đình

🇺🇸 [Tiếng Anh](README.md) | 🇻🇳 Tiếng Việt

---

## 📌 Trạng thái dự án

Dự án hiện đang trong giai đoạn **nghiên cứu và phát triển**.  
Mục tiêu là xây dựng một sản phẩm sử dụng trong **gia đình**, để đánh giá độ ổn định và hiệu quả trong môi trường thực tế.

---

## 🎯 Mục tiêu

Xây dựng hệ thống camera thông minh giá rẻ có thể:

- Nhận diện có người đứng trước cổng nhà
- Thông báo cho người trong nhà ngay lập tức
- Chạy AI trực tiếp trên thiết bị nhúng

Ngoài ra, node Camera được thiết kế để có thể phát triển thành một **sản phẩm camera thông minh độc lập**, có thể:

- Gắn trước cửa nhà hoặc cửa chung cư
- Quan sát người đến trước cửa theo thời gian thực
- Nhận diện người trong gia đình hoặc phát hiện người lạ
- Hoạt động như một thiết bị an ninh thông minh độc lập

---

## 🧱 Kiến trúc hệ thống

Hệ thống chia thành **2 node chính**:

![System Overview](photo/overview_system.png)

---

## 1️⃣ Node Camera

Nhiệm vụ: Giám sát trước cổng nhà.

### Phần cứng

- Board nhúng: Luckfox Pico (RV1103G1)
- Camera USB
- USB WiFi (chủ yếu để SSH code)
- Màn hình hiển thị realtime

### Chức năng

- Lấy video từ camera
- Chạy model nhận diện người
- Hiển thị realtime
- Gửi tín hiệu sang node Notice

---

## 2️⃣ Node Notice

Nhiệm vụ: Thông báo cho người trong nhà.

### Phần cứng

- MCU: STM32F103RCT6
- Mạch khuếch đại âm thanh I2S
- Loa
- Module không dây: HC-12

### Chức năng

- Nhận tín hiệu từ node Camera
- Phát âm thanh thông báo
- Có thể đặt ở bếp, phòng ngủ,…

---

## 📡 Giao tiếp

Node Camera → Node Notice sử dụng:

- Module HC-12 (UART wireless)
- Giá rẻ, nhỏ gọn
- Khoảng cách truyền xa

---

## 🛠 Công nghệ sử dụng

- Embedded Linux trên Luckfox Pico
- Firmware STM32 HAL (C language)
- UART wireless communication
- Model AI nhận diện người (YOLO / RKNN dự kiến)

---

## 🔮 Hướng phát triển

- Nhận diện khuôn mặt
- Thông báo qua điện thoại
- Theo dõi qua Internet
- Hệ thống nhiều camera
- Tích hợp Smart Home

---

## 👨‍💻 Tác giả

Nguyễn Duy Tùng  
Embedded Systems & IoT Developer  

---

⭐ Nếu bạn thấy project hay, hãy star repo nhé!