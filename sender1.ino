#include <WiFi.h>
#include <esp_now.h>
#include "DHT.h"

// ---------- Cấu hình DHT ----------
#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// ---------- Cấu hình ESP-NOW ----------
uint8_t receiverAddress[] = {0x88, 0x57, 0x21, 0xB6, 0xB4, 0xB4};  // ⚠️ Thay bằng MAC của ESP32 B

typedef struct struct_message {
  float temperature;
  float humidity;
  char msg[32];
} struct_message;

struct_message dataToSend;
struct_message dataReceived;

// ✅ Callback mới cho ESP32 core v3+
void onDataSent(const wifi_tx_info_t *wifi_tx_info, esp_now_send_status_t status) {
  Serial.print("📤 Gửi dữ liệu: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Thành công ✅" : "Thất bại ❌");
}

// ✅ Callback nhận phản hồi
void onDataRecv(const esp_now_recv_info *info, const uint8_t *incomingData, int len) {
  memcpy(&dataReceived, incomingData, sizeof(dataReceived));
  Serial.print("📩 Phản hồi từ ESP32 B: ");
  Serial.println(dataReceived.msg);
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  Serial.print("🔰 ESP32 A (Sender) MAC: ");
  Serial.println(WiFi.macAddress());

  dht.begin();

  if (esp_now_init() != ESP_OK) {
    Serial.println("❌ Lỗi khởi tạo ESP-NOW!");
    return;
  }

  esp_now_register_send_cb(onDataSent);
  esp_now_register_recv_cb(onDataRecv);

  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, receiverAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  esp_now_add_peer(&peerInfo);

  Serial.println("✅ ESP32 A sẵn sàng gửi dữ liệu nhiệt độ & độ ẩm...");
}

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("⚠️ Lỗi đọc DHT!");
    delay(2000);
    return;
  }

  dataToSend.temperature = t;
  dataToSend.humidity = h;
  strcpy(dataToSend.msg, "Dữ liệu cảm biến gửi đi!");

  esp_err_t result = esp_now_send(receiverAddress, (uint8_t *)&dataToSend, sizeof(dataToSend));

  if (result == ESP_OK) {
    Serial.printf("📤 Gửi T=%.2f°C, H=%.2f%%\n", t, h);
  } else {
    Serial.println("⚠️ Gửi lỗi!");
  }

  delay(5000);
}
