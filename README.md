# 👁️ Assistive Device for Visually Impaired People

> **Thiết bị thông minh hỗ trợ người khiếm thị nhận diện môi trường xung quanh, cảnh báo vật cản và tương tác qua giọng nói.**

## 📖 Giới thiệu
Dự án được xây dựng nhằm cung cấp giải pháp trợ lý cá nhân dạng đeo cho người khiếm thị. Hệ thống kết hợp giữa phần cứng đo đạc khoảng cách (Arduino, cảm biến siêu âm) và phần mềm thị giác máy tính / nhận diện giọng nói (Python, Node.js) để chuyển đổi thông tin môi trường thành tín hiệu âm thanh trực quan.

## ✨ Tính năng chính
- **Cảnh báo va chạm thời gian thực**: Cảm biến siêu âm phát hiện vật cản ở cự ly gần và phản hồi qua còi/rung.
- **Tương tác qua giọng nói (Live Speech)**: Module `live-speech.py` lắng nghe khẩu lệnh và đọc thông tin trợ giúp qua Text-to-Speech.
- **Server điều khiển trung gian**: Cầu nối Node.js (`server.js`) đồng bộ dữ liệu giữa phần cứng Arduino và client.
- **Module kiểm thử tích hợp**: Bộ kịch bản test delay, test khởi tạo và kiểm thử luồng điều khiển phần cứng.

## 🛠️ Ngăn xếp công nghệ
- **Hardware**: Arduino Uno/Nano, Cảm biến siêu âm HC-SR04, module rung & loa
- **Phần mềm & Thư viện**:
  - Python: `pyaudio`, `speech_recognition`, `pyttsx3`, `pyserial`
  - Backend: Node.js (Express, Socket/Serial)
  - Frontend: Giao diện web theo dõi trạng thái thiết bị (`index.html`)

## 🚀 Hướng dẫn cài đặt & Khởi chạy

```bash
# 1. Cài đặt các gói Node.js
npm install

# 2. Cài đặt thư viện Python
pip install speechrecognition pyaudio pyttsx3 pyserial

# 3. Nạp code cho Arduino (trong thư mục arduino-program/)
# Sử dụng Arduino IDE để upload lên board

# 4. Khởi chạy hệ thống
python live-speech.py
node server.js
```

## 📄 Bản quyền & Tác giả
Phát triển bởi [@baolam](https://github.com/baolam).
