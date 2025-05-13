# Thiết kế phần cứng

Thư mục này chứa các tệp thiết kế Altium của mạch điện cho hệ thống báo cháy tự động.

## Linh kiện sử dụng

| STT | Linh kiện | Mô tả | Số lượng |
|-----|-----------|-------|----------|
| 1 | Arduino Nano | Vi điều khiển | 1 |
| 2 | MQ2 | Cảm biến khí gas | 1 |
| 3 | DS18B20 | Cảm biến nhiệt độ | 1 |
| 4 | Flame Sensor | Cảm biến lửa | 1 |
| 5 | LCD 16x2 I2C | Màn hình hiển thị | 1 |
| 6 | Module SIM A7680C | Gửi SMS và gọi điện | 1 |
| 7 | LM2596 | Mạch nguồn | 1 |
| 8 | Relay | Ngắt nguồn | 1 |
| 9 | TIP122 | Transistor điều khiển | 3 |
| 10 | Nút nhấn | Điều khiển thủ công | 2 |
| 11 | LED | Đèn báo trạng thái | 4 |
| 12 | Còi | Còi báo động | 1 |
| 13 | Quạt | Thông gió | 1 |
| 14 | Máy bơm nước | Dập lửa | 1 |
| 15 | Các linh kiện thụ động | Điện trở, tụ, diode | - |

## Kết nối

### Cảm biến

- **MQ2** (Cảm biến khí gas):
  - VCC: 5V
  - GND: GND
  - AO: A0 (Arduino)

- **DS18B20** (Cảm biến nhiệt độ):
  - VCC: 5V
  - GND: GND
  - Data: Pin 3 (Arduino)
  - Điện trở pullup 4.7kΩ giữa VCC và Data

- **Flame Sensor** (Cảm biến lửa):
  - VCC: 5V
  - GND: GND
  - DO: Pin 2 (Arduino)

### Thiết bị đầu ra

- **LCD 16x2 I2C**:
  - VCC: 5V
  - GND: GND
  - SDA: A4 (Arduino)
  - SCL: A5 (Arduino)

- **SIM A7680C**:
  - VCC: 5V
  - GND: GND
  - TX: Pin 10 (Arduino)
  - RX: Pin 11 (Arduino)

- **LED**: Pin 12 (Arduino)
- **Còi báo động**: Pin 7 (Arduino)
- **Quạt thông gió**: Pin 5 (Arduino) qua TIP122
- **Bơm nước**: Pin 4 (Arduino) qua TIP122
- **Relay ngắt nguồn**: Pin 6 (Arduino) qua TIP122

### Nút nhấn

- SW1: Pin 9 (Arduino) - Chế độ tự động/thủ công và điều khiển quạt
- SW2: Pin A3 (Arduino) - Điều khiển bơm nước

## Sơ đồ khối

```
+------------+     +-----------------+     +---------------+
| Cảm biến   |     |                 |     | Đầu ra        |
| - Khí gas  |---->|                 |---->| - Quạt        |
| - Lửa      |     |    Arduino      |     | - Bơm nước    |
| - Nhiệt độ |     |     Nano        |     | - Còi báo động|
+------------+     |                 |     | - Relay       |
                   |                 |---->| - LCD         |
+------------+     |                 |     | - LED         |
| Nút nhấn   |---->|                 |     +---------------+
+------------+     +-----------------+
                          |
                          v
                   +-----------------+
                   | Module SIM      |
                   | - Gửi SMS       |
                   | - Gọi điện      |
                   +-----------------+
```

## Thiết kế PCB

Mạch được thiết kế trên phần mềm Altium Designer với các lưu ý sau:

1. Chia thành các khối chức năng riêng biệt: nguồn, xử lý, cảm biến, đầu ra
2. Tách biệt mạch công suất (quạt, bơm, relay) khỏi mạch điều khiển
3. Bố trí các cổng kết nối ở vị trí thuận tiện cho lắp đặt và bảo trì
4. Đặt mạch nguồn gần với đầu vào nguồn để giảm nhiễu
5. Thiết kế mạch 2 lớp giúp tối ưu không gian và giảm nhiễu

## Tệp thiết kế

Trong thư mục `altium_files` có các tệp thiết kế Altium:

- `CanhBaoChay.pdf`: File PDF chứa sơ đồ mạch chi tiết

Ngoài ra, hình ảnh của thiết kế có thể được tìm thấy trong thư mục `docs/images`. 