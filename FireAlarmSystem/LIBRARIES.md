# Thư viện cần thiết

Các thư viện Arduino cần thiết để biên dịch và chạy mã nguồn của hệ thống báo cháy tự động.

## Danh sách thư viện

| Tên thư viện | Phiên bản | Mô tả | Link cài đặt |
|--------------|-----------|-------|-------------|
| OneWire | 2.3.7 | Giao tiếp với cảm biến DS18B20 | [Link](https://github.com/PaulStoffregen/OneWire) |
| DallasTemperature | 3.9.0 | Đọc nhiệt độ từ cảm biến DS18B20 | [Link](https://github.com/milesburton/Arduino-Temperature-Control-Library) |
| LiquidCrystal_I2C | 1.1.2 | Điều khiển màn hình LCD qua I2C | [Link](https://github.com/johnrickman/LiquidCrystal_I2C) |
| SoftwareSerial | Built-in | Tạo cổng serial ảo cho module SIM | Có sẵn trong Arduino IDE |
| EEPROM | Built-in | Đọc/ghi dữ liệu vào bộ nhớ EEPROM | Có sẵn trong Arduino IDE |
| Wire | Built-in | Giao tiếp I2C | Có sẵn trong Arduino IDE |

## Cài đặt thư viện

### Sử dụng Arduino Library Manager (Khuyến nghị)

1. Mở Arduino IDE
2. Chọn menu **Sketch > Include Library > Manage Libraries**
3. Trong hộp tìm kiếm, nhập tên thư viện
4. Chọn và cài đặt phiên bản phù hợp

### Cài đặt thủ công

1. Tải thư viện từ link GitHub
2. Giải nén file ZIP
3. Di chuyển thư mục thư viện vào thư mục **libraries** trong thư mục cài đặt Arduino
   - Windows: `Documents/Arduino/libraries/`
   - MacOS: `~/Documents/Arduino/libraries/`
   - Linux: `~/Arduino/libraries/`

## Kiểm tra cài đặt

Sau khi cài đặt các thư viện, bạn có thể kiểm tra bằng cách:

1. Khởi động lại Arduino IDE
2. Chọn menu **File > Examples**
3. Kiểm tra xem các thư viện đã cài đặt có xuất hiện trong danh sách không

## Tương thích

Mã nguồn của dự án đã được kiểm tra và hoạt động tốt với:

- Arduino IDE 1.8.13 hoặc mới hơn
- Arduino Nano (ATmega328) 