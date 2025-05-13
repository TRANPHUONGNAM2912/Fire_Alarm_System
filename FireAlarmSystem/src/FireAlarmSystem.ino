#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "main.h"
LiquidCrystal_I2C lcd(0x27, 16, 2);  
int smokeSensorPin = A0;
int flameSensorPin = 2; 
int buzzerPin = 7;
int ledPin = 12;
int fanPin = 5;
int pumpPin = 4;
int relayPin = 6;
int ledState = LOW;   
const int sw1 = 9;
const int sw2 = A3;
bool prevSw1State = HIGH;
bool prevSw2State = HIGH;
bool sw1State = HIGH;  // Trạng thái của nút sw1
unsigned long sw1PressStartTime = 0;  // Thời điểm bắt đầu giữ nút sw1
const unsigned long holdDuration = 3000;  // Thời gian giữ nút (5 giây)
bool isAutoMode = true;  // Trạng thái chế độ (auto: true, manual: false)

#define ONE_WIRE_BUS 3
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

static bool isNormalScreen = true; 
float temperatureC = 0;
int smokeThreshold = 100; //cài đặt ngưỡng khí gas
int setteamp = 27;//cài đặt thái nhiệt độ
unsigned long previousMillis = 0;
unsigned long previousMillis1 = 0;
unsigned long previousMillis2 = 0;
const long interval = 100;//thời gian led bật tắt
const long duration = 10000;//thời gian các thiết bị hoạt động thêm khi sự cố
bool check = false;//biến kiểm tra xem có sự cố xảy ra không
int check1 = 0;
int check2 = 0;
bool fanState = LOW;
bool pumpState = LOW;
bool fanStatus = false;
bool pumStatus = false;
bool flag=false;
String mumber = "";

void start_up(void)
{
  sim_init();

  sent_sms(mumber, "KHOI DONG HE THONG ");

}
void Task_Sim(void);
void setup() {
  Serial.begin(115200);
  pinMode(sw1, INPUT_PULLUP);
  pinMode(sw2, INPUT_PULLUP);
  pinMode(smokeSensorPin, INPUT);
  pinMode(flameSensorPin, INPUT);
  pinMode(buzzerPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(fanPin, OUTPUT);
  pinMode(pumpPin, OUTPUT);
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, HIGH);
  lcd.init(); 
  lcd.backlight(); 
  lcd.setCursor(1,0); 
  lcd.print("DANG KHOI DONG"); 
  mumber = "";
  mumber = EEPROM_get(ADD_MUM);
  
  Serial.print("So dien thoai:");
  Serial.println(mumber);

  sim.begin(SIM_BAUD);


  start_up();
  // delay(1000); 
  
  sensors.begin();
}

void loop() {
    Task_Sim();
  int smokeValue = analogRead(smokeSensorPin);//đọc cảm biến khí gas
  int flameValue = digitalRead(flameSensorPin);//đọc cảm biến lửa
  sensors.requestTemperatures();
  int temperatureC = sensors.getTempCByIndex(0);//đọc cảm biến nhiệt độ
// kiểm tra nếu giá trị cảm biến gas đo được lớn hơn giá trị đặt
  // Serial.println(isAutoMode);
  if (digitalRead(sw1) == LOW) {
    if (sw1State == HIGH) {
      // Nếu nút sw1 vừa được nhấn, lưu thời điểm bắt đầu giữ nút
      sw1PressStartTime = millis();
      sw1State = LOW;
    }

    // Kiểm tra thời gian giữ nút
    if (millis() - sw1PressStartTime >= holdDuration) {
      lcd.clear();
      lcd.setCursor(2,0);
      lcd.print("CHANGE MODE ");
      // Nếu nút đã được giữ trong thời gian đủ lâu (5 giây)
      // Kiểm tra chế độ và chuyển đổi
      if (isAutoMode) {
        // Đang ở chế độ auto, chuyển qua chế độ manual
        isAutoMode = false;
        delay(2000);
        
        // Thêm code thực hiện công việc cần thiết khi chuyển sang chế độ manual
      } else {
        // Đang ở chế độ manual, chuyển qua chế độ auto
        isAutoMode = true;
        delay(2000);
        digitalWrite(fanPin, LOW);
        digitalWrite(pumpPin, LOW);
        // Thêm code thực hiện công việc cần thiết khi chuyển sang chế độ auto
      }

      // Reset trạng thái nút sw1 để tránh lặp lại việc chuyển đổi
      sw1State = HIGH;
    }
  } else {
    // Nút sw1 không được nhấn, reset trạng thái nút
    sw1State = HIGH;
  }
  if(isAutoMode==true){
if (smokeValue < smokeThreshold){
  check1=0;
  }
  if (temperatureC < setteamp){
  check2=0;
  }
  
  if (smokeValue > smokeThreshold) {

// đổi trạng thái lcd mỗi 2s
    unsigned long currentMillis1 = millis();
    if (currentMillis1 - previousMillis1 >= 2000) {
      previousMillis1 = currentMillis1; 
      if (isNormalScreen) {
        lcd.clear();
        lcd.setCursor(2,0); 
        lcd.print("TRANG THAI:");
        lcd.setCursor(1,1); 
        lcd.print("PHAT HIEN GAS");
        isNormalScreen = false;
      } else {
        lcd.clear();
        lcd.setCursor(2,0);
        lcd.print("FLAME: ");
        lcd.setCursor(8,0);
        lcd.print("KHONG");
        lcd.setCursor(0,1);
        lcd.print("GAS:");
        lcd.setCursor(4,1);
        lcd.print(smokeValue);
        lcd.setCursor(8,1);
        lcd.print("T: ");
        lcd.setCursor(10,1);
        lcd.print(temperatureC);
        lcd.setCursor(12,1);
        lcd.print("/");
        lcd.setCursor(13,1);
        lcd.print(setteamp);
        isNormalScreen = true;
      }
    }
         check1++;
       if(check1==1){
        sent_sms(mumber, "PHAT HIEN GAS ");
          
      }
      
      
    // bật quạt,buzzer và đổi biến check thành true
    digitalWrite(fanPin, HIGH);
    digitalWrite(buzzerPin, HIGH);
    ledBlink();
    check = true;
  } 
  // kiểm tra xem có lửa và nhiệt độ trên ngưỡng cài đặt không
  else if (flameValue == 0 && temperatureC > setteamp) { 
// chuyển đổi màn hình lcd mỗi 2s
    unsigned long currentMillis1 = millis();
    if (currentMillis1 - previousMillis1 >= 2000) {
      previousMillis1 = currentMillis1; 
      if (isNormalScreen) {
        lcd.clear();
        lcd.setCursor(2,0); 
        lcd.print("TRANG THAI:");
        lcd.setCursor(1,1); 
        lcd.print("PHAT HIEN CHAY");
        isNormalScreen = false;
      } else {
        lcd.clear();
        lcd.setCursor(2,0);
        lcd.print("FLAME: ");
        lcd.setCursor(8,0);
        lcd.print("CO");
        lcd.setCursor(0,1);
        lcd.print("GAS:");
        lcd.setCursor(4,1);
        lcd.print(smokeValue);
        lcd.setCursor(8,1);
        lcd.print("T: ");
        lcd.setCursor(10,1);
        lcd.print(temperatureC);
        lcd.setCursor(12,1);
        lcd.print("/");
        lcd.setCursor(13,1);
        lcd.print(setteamp);
        isNormalScreen = true;
      }
    } 
    // bật bơm,buzzer,relay,đổi biến check thành true
        check2++;
       if(check2==1){
        sent_sms(mumber, "PHAT HIEN CHAY ");
          
      }
       
    digitalWrite(pumpPin, HIGH);
    digitalWrite(buzzerPin, HIGH);
    digitalWrite(relayPin, LOW);
    ledBlink();
    check = true;
  } 
  
  //kiểm tra xem có lửa và nhiệt độ trên ngưỡng cài đặt hoặc khí gas trên ngưỡng cài đặt
  else if (flameValue == 0&& temperatureC < setteamp ) { 
// chuyển màn hình lcd mỗi 2s
    unsigned long currentMillis1 = millis();
    if (currentMillis1 - previousMillis1 >= 2000) {
      previousMillis1 = currentMillis1; 
      if (isNormalScreen) {
        lcd.clear();
        lcd.setCursor(1,0); 
        lcd.print("TRANG THAI");
        lcd.setCursor(1,1); 
        lcd.print("PHAT HIEN LUA");
        isNormalScreen = false;
      } else {
        lcd.clear();
        lcd.setCursor(2,0);
        lcd.print("FLAME: ");
        lcd.setCursor(8,0);
        lcd.print("CO");
        lcd.setCursor(0,1);
        lcd.print("GAS:");
        lcd.setCursor(4,1);
        lcd.print(smokeValue);
        lcd.setCursor(8,1);
        lcd.print("T: ");
        lcd.setCursor(10,1);
        lcd.print(temperatureC);
        lcd.setCursor(12,1);
        lcd.print("/");
        lcd.setCursor(13,1);
        lcd.print(setteamp);
        isNormalScreen = true;
      }
    } 
    // digitalWrite(fanPin, HIGH);
    // digitalWrite(pumpPin, HIGH);
    // digitalWrite(buzzerPin, HIGH);
    // digitalWrite(relayPin, LOW);
    // ledBlink();
    // check = true;
  } else {
    unsigned long currentMillis1 = millis();
    if (currentMillis1 - previousMillis1 >= 2000) {
      previousMillis1 = currentMillis1; 
      if (isNormalScreen) {
        lcd.clear();
        lcd.setCursor(2,0); 
        lcd.print("STATE:");
        lcd.setCursor(9,0); 
        lcd.print("Normal");
        lcd.setCursor(2,1); 
        lcd.print("MODE:");
        lcd.setCursor(8,1); 
        lcd.print("AUTO");
        isNormalScreen = false;
      } else {
        lcd.clear();
        lcd.setCursor(2,0);
        lcd.print("FLAME: ");
        lcd.setCursor(8,0);
        lcd.print("KHONG");
        lcd.setCursor(0,1);
        lcd.print("GAS:");
        lcd.setCursor(4,1);
        lcd.print(smokeValue);
        lcd.setCursor(8,1);
        lcd.print("T: ");
        lcd.setCursor(10,1);
        lcd.print(temperatureC);
        lcd.setCursor(12,1);
        lcd.print("/");
        lcd.setCursor(13,1);
        lcd.print(setteamp);
        isNormalScreen = true;
      } 
    }
    ledOff();
    // bơm,quạt,buzzer bật,relay ngắt thêm 1 khoảng thời gian khi có sự cố
    unsigned long currentMillis2 = millis();
    if (check==true) {
        
      if (currentMillis2 - previousMillis2 >= duration) {
        sim_call(mumber);
        digitalWrite(fanPin, LOW);
        digitalWrite(pumpPin, LOW);
        digitalWrite(buzzerPin, LOW);
        digitalWrite(relayPin, HIGH);
        
        previousMillis2 = currentMillis2;
        check = false; 
    }   
    }
    else{
      previousMillis2 = currentMillis2;
    }
  }

  }
  else{
    
    lcd.setCursor(2,0);
    lcd.print("MODE: ");
    lcd.setCursor(8,0);
    lcd.print("Manual");
    lcd.setCursor(14,0);
    lcd.print(" ");

    lcd.setCursor(0,1); 
    lcd.print("QUAT:");
    lcd.setCursor(5, 1); 
    lcd.print(fanStatus ? "ON " : "OFF");
    lcd.setCursor(8,1); 
    lcd.print(" ");
    lcd.setCursor(9,1); 
    lcd.print("BOM:");
    lcd.setCursor(13, 1); 
    lcd.print(pumStatus ? "ON " : "OFF");
  bool currentSw1State = digitalRead(sw1);
  if (currentSw1State == LOW && prevSw1State == HIGH) {
    // Nếu nút sw1 được nhấn sau khi được nhả ra
    if (fanState == HIGH) {
      // Nếu fanPin đang bật, tắt nó
      digitalWrite(fanPin, LOW);
      fanState = LOW;
      fanStatus = false;
    } else {
      // Nếu fanPin đang tắt, bật nó
      digitalWrite(fanPin, HIGH);
      fanState = HIGH;
      fanStatus = true;
    }
  }
  prevSw1State = currentSw1State; // Lưu trạng thái hiện tại của nút sw1

  // Kiểm tra nút sw2
  bool currentSw2State = digitalRead(sw2);
  if (currentSw2State == LOW && prevSw2State == HIGH) {
    // Nếu nút sw2 được nhấn sau khi được nhả ra
    if (pumpState == HIGH) {
      // Nếu pumpPin đang bật, tắt nó
      digitalWrite(pumpPin, LOW);
      pumpState = LOW;
      pumStatus = false;
    } else {
      // Nếu pumpPin đang tắt, bật nó
      digitalWrite(pumpPin, HIGH);
      pumpState = HIGH;
      pumStatus = true;
    }
  }
  prevSw2State = currentSw2State; // Lưu trạng thái hiện tại của nút sw2
  }
}
// led nhấp nháy mỗi 0.2s
void ledBlink() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }
    digitalWrite(ledPin, ledState);
  }
}
// tắt led
void ledOff() {
  digitalWrite(ledPin, LOW);
}

void Task_Sim(void)
{
  if (sim.available())
  {
    String sim_data = sim.readString();
    String messenger = "";
    String telephone_mumber = "";

    if (sim_data[4] == 'M')
    {
      for (int8_t i = 50; i < sim_data.length(); i++)
      {
        messenger += sim_data[i];
      }

      for (uint8_t i = 12; i < sim_data.length() && sim_data[i] != '"'; i++)
      {
        telephone_mumber += sim_data[i];
      }
      Serial.println(sim_data);
      Serial.print("Noi dung tin :");
      Serial.println(messenger);
      Serial.print("So dien thoai den :");
      Serial.println(telephone_mumber);
    }

    if (messenger[0] == 'S' && messenger[1] == 'D' && messenger[2] == 'T')
    {
      mumber = "";
      for (int i = 3; i <= 12; i++)
      {
        mumber += (String)messenger[i];
      }
      Serial.print("So dien thoai cai : ");
      Serial.println(mumber);
      if (mumber.length() == 10)
      {
        EEPROM_put(ADD_MUM, mumber);
        sent_sms("0" + telephone_mumber, "Da luu : " + mumber);
      }
      else sent_sms("0" + telephone_mumber, "Sai cu phap");
    }
    // if (messenger[0] == 'T' && messenger[1] == 'O' && messenger[2] == 'A' && messenger[3] == 'D' && messenger[4] == 'O' )
    // {
    //   read_gps();
    // }
  }
}
