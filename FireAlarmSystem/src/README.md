# Mã nguồn hệ thống báo cháy tự động

Thư mục này chứa mã nguồn Arduino cho hệ thống báo cháy tự động với module SIM A7680C.

## Cấu trúc mã nguồn

- `FireAlarmSystem.ino`: File chương trình chính
- `main.h`: File header chứa các hàm xử lý module SIM và các định nghĩa

## Các thư viện sử dụng

```cpp
#include <OneWire.h>           // Giao tiếp với cảm biến DS18B20
#include <DallasTemperature.h> // Đọc dữ liệu từ DS18B20
#include <Wire.h>              // Giao tiếp I2C
#include <LiquidCrystal_I2C.h> // Điều khiển LCD
#include <SoftwareSerial.h>    // Giao tiếp với module SIM
#include <EEPROM.h>            // Lưu trữ số điện thoại
```

## Khai báo chân kết nối

```cpp
int smokeSensorPin = A0;     // Cảm biến khí gas MQ2
int flameSensorPin = 2;      // Cảm biến lửa
int buzzerPin = 7;           // Còi báo động
int ledPin = 12;             // Đèn LED báo động
int fanPin = 5;              // Quạt thông gió
int pumpPin = 4;             // Bơm nước
int relayPin = 6;            // Relay ngắt nguồn
const int sw1 = 9;           // Nút nhấn 1 (chế độ & quạt)
const int sw2 = A3;          // Nút nhấn 2 (bơm)
#define ONE_WIRE_BUS 3       // Cảm biến nhiệt độ DS18B20
#define SERIAL_SIM_TX 11     // TX module SIM
#define SERIAL_SIM_RX 10     // RX module SIM
```

## Cách hoạt động

### Khởi tạo

1. **setup()**: Khởi tạo các chân I/O, màn hình LCD, module SIM và cảm biến
   - Cấu hình chân vào/ra
   - Lấy số điện thoại đã lưu trong EEPROM
   - Khởi động module SIM
   - Gửi SMS thông báo hệ thống đã khởi động

### Vòng lặp chính

2. **loop()**: Vòng lặp chính của chương trình
   - Kiểm tra thông báo từ module SIM
   - Đọc dữ liệu từ các cảm biến
   - Kiểm tra trạng thái nút nhấn để thay đổi chế độ
   - Xử lý theo chế độ tự động hoặc thủ công

### Chế độ tự động

3. **Quy trình xử lý**:
   - Nếu phát hiện khí gas (MQ2):
     * Bật quạt thông gió
     * Kích hoạt còi báo động và LED nhấp nháy
     * Hiển thị cảnh báo trên LCD
     * Gửi SMS thông báo
   
   - Nếu phát hiện lửa và nhiệt độ cao:
     * Bật bơm nước
     * Ngắt relay nguồn
     * Kích hoạt còi báo động và LED nhấp nháy
     * Hiển thị cảnh báo trên LCD
     * Gửi SMS thông báo
     * Thực hiện cuộc gọi sau một khoảng thời gian
   
   - Khi hết sự cố:
     * Tắt các thiết bị sau một khoảng thời gian

### Chế độ thủ công

4. **Điều khiển thủ công**:
   - Nút SW1: Bật/tắt quạt thông gió
   - Nút SW2: Bật/tắt bơm nước
   - Hiển thị trạng thái thiết bị trên LCD

### Chức năng module SIM

5. **Xử lý SMS và cuộc gọi**:
   - Nhận SMS để cấu hình số điện thoại (cú pháp: `SDT0123456789`)
   - Gửi SMS thông báo khi phát hiện sự cố
   - Thực hiện cuộc gọi khi có sự cố nghiêm trọng

## Lưu trữ dữ liệu

- Số điện thoại nhận thông báo được lưu trong EEPROM
- Ngưỡng cảnh báo nhiệt độ và khí gas được cấu hình trong mã nguồn:
  ```cpp
  int smokeThreshold = 100; // Ngưỡng khí gas
  int setteamp = 27;       // Ngưỡng nhiệt độ (°C)
  ```

## Cải tiến tiềm năng

Một số ý tưởng cải tiến cho mã nguồn:

1. Thêm khả năng cấu hình ngưỡng nhiệt độ và khí gas qua SMS
2. Lưu trữ lịch sử sự cố
3. Thêm giao diện web để theo dõi từ xa
4. Tối ưu hóa tiêu thụ năng lượng 