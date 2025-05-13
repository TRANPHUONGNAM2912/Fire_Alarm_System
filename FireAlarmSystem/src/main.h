#ifndef MAIN_H
#define MAIN_H

#include <EEPROM.h>

#include <SoftwareSerial.h>
// #include <TinyGPS++.h>
// TinyGPSPlus gps;
#define SERIAL_SIM_TX 11
#define SERIAL_SIM_RX 10
SoftwareSerial sim(SERIAL_SIM_RX, SERIAL_SIM_TX); // RX, TX


// #define SERIAL_GPS_TX 3
// #define SERIAL_GPS_RX 4
// SoftwareSerial Serial_gps(SERIAL_GPS_RX, SERIAL_GPS_TX); // RX, TX

#define SIM_BAUD    115200

// #define SENSOR 2
// #define BUZZER 12 // sua lai 12
// #define BUTTON 7
// #define LED 9

// #define BUZZER_ON  digitalWrite(BUZZER,HIGH)
// #define BUZZER_OFF  digitalWrite(BUZZER,LOW)

// #define LED_ON    digitalWrite(LED,HIGH)
// #define LED_OFF    digitalWrite(LED,LOW)

#define ADD_MUM 0
#define ADD_SYS 15

// #define LEVEL_RING1   30000
// #define LEVEL_RING_OFF  100

#define ON    1
#define OFF   0

/***** luu du lieu tu EEPROM ********/
void EEPROM_put(int add, String Data)
{
  for (int i = 0; i < 10 ; i++)
  {
    EEPROM.write(i + add, Data[i]);
  }
}
/******** lay du lieu tu eeprom *********/
String EEPROM_get(int add)
{
  String sdt = "";
  for (int i = add; i < add + 10; i++)
  {
    char c  = EEPROM.read(i);
    sdt += (String)c;
  }
  return sdt;
}

/**** ham thu vien sim ******/
void sim_at_wait()
{
  delay(100);
  while (sim.available())
  {
    Serial.write(sim.read());
  }
}
bool sim_at_cmd(String cmd)
{
  sim.println(cmd);
  delay(500);
  sim_at_wait();
}
bool sim_at_send(char c) {
  sim.write(c);
}


void sent_sms(String mumber, String mess)
{
  String num = (String)mumber[1] + (String)mumber[2] + (String)mumber[3]
               + (String)mumber[4] + (String)mumber[5] + (String)mumber[6] + (String)mumber[7]
               + (String)mumber[8] + (String)mumber[9];
  Serial.println(num);
  String com = "+84" + (String)num;
  Serial.println(com);
  sim_at_cmd("AT+CMGF=1");
  String temp = "AT+CMGS=\"";
  temp += (String)com;
  temp += "\"";
  sim_at_cmd(temp);
  sim_at_cmd(mess);
  sim_at_send(0x1A);
  delay(1000);
}

void sim_init(void)
{
  sim_at_cmd("AT");
  delay(500);
  sim_at_cmd("ATI");
  delay(500);
  sim_at_cmd("AT+CPIN?");
  delay(500);
  sim_at_cmd("AT+CSQ");
  delay(500);
  sim_at_cmd("AT+CIMI");
  delay(500);
  sim_at_cmd("AT+CMGF=1");
  delay(500);

  sim_at_cmd("AT+CLIP=1");
  delay(500);

  sim_at_cmd("AT+CNMI=1,2,0,0,0");
  delay(500);
  sim_at_cmd("AT&W");

  delay(1000);
}

void sim_call(String mumber) {
  String temp = "ATD+84";
  String num = (String)mumber[1] + (String)mumber[2] + (String)mumber[3]
               + (String)mumber[4] + (String)mumber[5] + (String)mumber[6] + (String)mumber[7]
               + (String)mumber[8] + (String)mumber[9];
  temp += num;
  temp += ";";

  // Gửi lệnh gọi điện
  sim_at_cmd(temp);

  // Bắt đầu đo thời gian gọi điện
  unsigned long callStartTime = millis();
  while (millis() - callStartTime < 10000) {
    // Vòng lặp chờ trong 15 giây nhưng không chặn chương trình.
    // Có thể thêm các tác vụ khác tại đây nếu cần.
  }

  // Ngắt cuộc gọi sau 15 giây
  sim_at_cmd("ATH");

  // Chờ thêm 1 giây trước khi tiếp tục
  unsigned long endWaitTime = millis();
  while (millis() - endWaitTime < 1000) {
    // Chờ 1 giây
  }
}



#endif
