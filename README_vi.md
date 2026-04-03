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

## 📅 Các bước đã thực hiện trong dự án

| STT | Công việc | Trạng thái | Ngày hoàn thành |
|----|-----------|------------|----------------|
| 1 | Đọc USB WiFi | ✅ Xong | 24/01/2026 |
| 2 | Giao tiếp màn hình TFT | ✅ Xong | 25/01/2026 |
| 3 | Đọc camera USB | ✅ Xong | 26/01/2026 |
| 4 | Stream camera USB lên TFT | ✅ Xong | 28/01/2026 |
| 5 | Thêm Mobile OpenCV trên Luckfox Pico | ✅ Xong | 29/01/2026 |
| 6 | Resize stream từ 160x120 → 160x128 trên TFT | ✅ Xong | 29/01/2026 |
| 7 | Chạy RKNN trên Luckfox Pico | ✅ Xong | 05/02/2026 |
| 8 | Load model YOLOv5 nhận diện người | ✅ Xong | 07/02/2026 |
| 9 | Thêm confidence score hiển thị trên TFT | ✅ Xong | 08/01/2026 |
|10 | Nghiên cứu mở rộng USB hub cho Luckfox Pico | ✅ Xong | 11/01/2026 |
|11 | Vẽ sơ đồ nguyên lý USB hub cho node Camera | ⏳ Chưa | — |
|12 | Vẽ PCB node Camera | ⏳ Chưa | — |
|13 | Kiểm thử mạch STM32F103RCT6 | ✅ Xong | 15/01/2026 |
|14 | Kiểm thử truyền thông HC-12 giữa Luckfox Pico và STM32F103RCT6 | ✅ Xong | 17/01/2026 |
|15 | Phát triển I2S với mạch khuếch đại và loa | ✅ Xong| 22/02/2026 |
|16 | Test full nhận diện người → node Notice → phát loa | ⏳ Chưa | — |
|17 | Nghiên cứu sơ đồ nguyên lý node Notice | ⏳ Chưa | — |
|18 | Đánh giá mô hình hệ thống | ⏳ Chưa | — |
|19 | Nâng cấp màn hình lớn hơn cho node Camera | ✅ Xong | 03/04/2026 |
|20 | Nghiên cứu face embedding cho Luckfox Pico | ⏳ Chưa | — |
|21 | Phát triển node Camera thành sản phẩm độc lập | ⏳ Chưa | — |
|22 | Nghiên cứu truyền thông giám sát cho app mobile | ⏳ Chưa | — |

---

## 👨‍💻 Tác giả

Nguyễn Duy Tùng  
Embedded Systems & IoT Developer  

---

⭐ Nếu bạn thấy project hay, hãy star repo nhé!