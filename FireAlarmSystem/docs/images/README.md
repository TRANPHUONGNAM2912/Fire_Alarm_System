# Tài liệu và hình ảnh

Thư mục này chứa các hình ảnh và tài liệu về thiết kế mạch của hệ thống báo cháy tự động.

## Nội dung

- `pcb_3d_view.jpg`: Hình ảnh 3D của PCB
- `pcb_schematic.jpg`: Sơ đồ mạch điện

## Các kết nối chính

### Cảm biến

- **MQ2**: Cảm biến khí gas, kết nối với chân A0 của Arduino
- **DS18B20**: Cảm biến nhiệt độ, kết nối với chân 3 của Arduino
- **Flame Sensor**: Cảm biến lửa, kết nối với chân 2 của Arduino

### Thiết bị đầu ra

- **Quạt thông gió**: Kết nối qua transistor TIP122 với chân 5 của Arduino
- **Bơm nước**: Kết nối qua transistor TIP122 với chân 4 của Arduino
- **Relay ngắt nguồn**: Kết nối qua transistor TIP122 với chân 6 của Arduino
- **Còi báo động**: Kết nối với chân 7 của Arduino
- **LED cảnh báo**: Kết nối với chân 12 của Arduino

### Màn hình

- **LCD I2C 16x2**: Kết nối qua I2C (SDA: A4, SCL: A5)

### Module SIM A7680C

Kết nối SoftwareSerial:
- TX: Chân 11 của Arduino
- RX: Chân 10 của Arduino 