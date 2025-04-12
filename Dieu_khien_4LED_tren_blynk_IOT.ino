// 2 dòng đầu là link của phần Blynk app của bạn.
#define BLYNK_TEMPLATE_ID "TMPL6UNj686bn"
#define BLYNK_TEMPLATE_NAME "Bat Tat Thiet Bj Tu Xa"
#define BLYNK_FIRMWARE_VERSION "0.1.0"

// Inlude thư viện.
#define BLYNK_PRINT Serial
#include "BlynkEdgent.h"

// Định nghĩa các chân kết nối.
#define led1 5 // D1
#define led2 0 // D2
#define button1 14 // D3
#define button2 12 // D4

unsigned long times = millis();
unsigned long timesDelay = millis();

// Tương đương nút reset để kết nối Wifi.
WidgetLED led_connect(V0);

// Trạng thái cập nhật nút bấm.
boolean updateState1 = 0;
boolean updateState2 = 0;

void setup()
{
  Serial.begin(115200);
  BlynkEdgent.begin();
  
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);

  // Sử dụng ngắt để điều khiển các nút bấm.
  attachInterrupt(digitalPinToInterrupt(button1), handleInterrupt1, FALLING);
  attachInterrupt(digitalPinToInterrupt(button2), handleInterrupt2, FALLING);
}

void loop() {
  BlynkEdgent.run();

  // Kiểm tra kết nối và viết giá trị lên Blynk app.
  if(millis() - times > 1000) {
    Blynk.virtualWrite(V5, millis() / 1000);
    if (led_connect.getValue()) {
      led_connect.off();
    } else {
      led_connect.on();
    }
    times = millis();
  }

  // Cập nhật trạng thái các nút bấm.
  if(updateState1 == 1) {
    Blynk.virtualWrite(V1, digitalRead(led1));
    updateState1 = 0;
  }
  
  if(updateState2 == 1) {
    Blynk.virtualWrite(V2, digitalRead(led2));
    updateState2 = 0;
  }
}

// Đồng bộ trạng thái khi kết nối với Blynk.
BLYNK_CONNECTED() {
  Blynk.syncAll();
}

// Điều khiển led từ Blynk app.
BLYNK_WRITE(V1) {
  int value = param.asInt();
  digitalWrite(led1, value);
}

BLYNK_WRITE(V2) {
  int value = param.asInt();
  digitalWrite(led2, value);
}

// Xử lý ngắt cho nút 1.
ICACHE_RAM_ATTR void handleInterrupt1() {
  if(millis() - timesDelay > 200) {
    digitalWrite(led1, !digitalRead(led1));
    updateState1 = 1;
    timesDelay = millis();
  }
}

// Xử lý ngắt cho nút 2.
ICACHE_RAM_ATTR void handleInterrupt2() {
  if(millis() - timesDelay > 200) {
    digitalWrite(led2, !digitalRead(led2));
    updateState2 = 1;
    timesDelay = millis();
  }
}
