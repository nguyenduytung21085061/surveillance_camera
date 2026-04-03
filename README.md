# 📷 Smart Home Surveillance Camera System

🇺🇸 English | 🇻🇳 [Vietnamese](README_vi.md)

---

## 📌 Project Status

This project is currently in the **research and development stage**.  
The goal is to develop a real product for **home use**, testing system stability, performance, and usability in real-world conditions.

---

## 🎯 Project Goal

Build a **low-cost smart home surveillance system** that can:

- Detect when someone stands in front of the house gate
- Notify family members inside the house in real time
- Run AI detection on an embedded device

In addition, the Camera Node is designed so it can be developed into an **independent smart door camera product**, which can:

- Be mounted in front of an apartment or house door
- Monitor visitors in real time
- Recognize whether the person is a family member or just detect unknown people
- Work as a standalone smart security device

---

## 🧱 System Architecture

The system is divided into **two main nodes**:

![System Overview](photo/overview_system.png)

---

## 1️⃣ Camera Node

Main task: Monitor the front gate and detect people.

### Hardware

- Embedded board: Luckfox Pico (RV1103G1)
- USB Camera
- USB WiFi dongle (mainly for SSH development)
- Real-time display screen

### Why Luckfox Pico?

- Powerful enough to run person detection models
- Small size, low power
- Suitable for edge AI applications

### Functions

- Capture video from USB camera
- Run AI model to detect people
- Display real-time video
- Send notification signal to Notice Node

---

## 2️⃣ Notice Node

Main task: Notify people inside the house.

### Hardware

- MCU: STM32F103RCT6
- Audio amplifier (I2S output)
- Speaker module
- Wireless communication module: HC-12

### Functions

- Receive signal from Camera Node
- Play voice/audio alert
- Can be placed in kitchen, bedroom, etc.

---

## 📡 Communication

Camera Node communicates with Notice Node using:

- HC-12 wireless serial module
- Low cost
- Long range
- Simple UART interface

---

## 🛠 Technologies Used

- Embedded Linux on Luckfox Pico
- STM32 HAL firmware (C language)
- UART wireless communication
- AI person detection model (YOLO / RKNN planned)

---

## 📅 Project Progress

| No | Task | Status | Completion Date |
|----|------|--------|----------------|
| 1 | Read USB WiFi | ✅ Done | 24/01/2026 |
| 2 | Interface TFT display | ✅ Done | 25/01/2026 |
| 3 | Read USB camera | ✅ Done | 26/01/2026 |
| 4 | Stream USB camera on TFT | ✅ Done | 28/01/2026 |
| 5 | Add Mobile OpenCV on Luckfox Pico | ✅ Done | 29/01/2026 |
| 6 | Fit stream size from 160x120 → 160x128 on TFT | ✅ Done | 29/01/2026 |
| 7 | Run RKNN on Luckfox Pico | ✅ Done | 05/02/2026 |
| 8 | Load YOLOv5 model to detect person | ✅ Done | 07/02/2026 |
| 9 | Add confidence score on TFT | ✅ Done | 08/01/2026 |
|10 | Research USB hub expansion for Luckfox Pico | ✅ Done | 11/01/2026 |
|11 | Draw USB hub schematic for Camera Node | ⏳ Pending | — |
|12 | Design PCB for Camera Node | ⏳ Pending | — |
|13 | Test STM32F103RCT6 board | ✅ Done | 15/01/2026 |
|14 | Test HC-12 communication between Luckfox Pico and STM32F103RCT6 | ✅ Done | 17/01/2026 |
|15 | Develop I2S audio with amplifier & speaker | ✅ Done| 22/02/2026 |
|16 | Full test: Person detection → Notice Node → Speaker alert | ⏳ Pending | — |
|17 | Research schematic for Notice Node | ⏳ Pending | — |
|18 | System evaluation testing | ⏳ Pending | — |
|19 | Upgrade TFT screen to larger display | ✅ Done | 03/04/2026 |
|20 | Research face embedding on Luckfox Pico | ⏳ Pending | — |
|21 | Develop standalone Camera Node product | ⏳ Pending | — |
|22 | Research mobile monitoring communication system | ⏳ Pending | — |

---

## 👨‍💻 Author

Nguyễn Duy Tùng  
Embedded Systems & IoT Developer  

---

⭐ If you like this project, please give it a star!