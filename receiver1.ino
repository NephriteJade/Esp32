#include <WiFi.h>
#include <esp_now.h>

uint8_t senderAddress[] = {0x24, 0x6F, 0x28, 0xAB, 0xCD, 0xEF};  // ⚠️ Thay bằng MAC của ESP32 A

typedef struct struct_message {
  float temperature;
  float humidity;
  char msg[32];
} struct_message;

struct_message dataReceived;
struct_message dataToSend;

// ✅ Callback nhận dữ liệu (ESP-NOW v3)
void onDataRecv(const esp_now_recv_info *info, const uint8_t *incomingData, int len) {
  memcpy(&dataReceived, incomingData, sizeof(dataReceived));

  Serial.println("📩 Nhận dữ liệu từ ESP32 A:");
  Serial.printf("🌡️  Nhiệt độ: %.2f °C\n", dataReceived.temperature);
  Serial.printf("💧  Độ ẩm: %.2f %%\n", dataReceived.humidity);

  // Gửi phản hồi lại A
  strcpy(dataToSend.msg, "✅ Đã nhận dữ liệu nhiệt độ & độ ẩm!");
  esp_now_send(info->src_addr, (uint8_t *)&dataToSend, sizeof(dataToSend));
}

// ✅ Callback gửi phản hồi (ESP-NOW v3)
void onDataSent(const wifi_tx_info_t *wifi_tx_info, esp_now_send_status_t status) {
  Serial.print("📤 Phản hồi gửi: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Thành công ✅" : "Thất bại ❌");
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  Serial.print("🔰 ESP32 B (Receiver) MAC: ");
  Serial.println(WiFi.macAddress());

  if (esp_now_init() != ESP_OK) {
    Serial.println("❌ Lỗi khởi tạo ESP-NOW!");
    return;
  }

  esp_now_register_send_cb(onDataSent);
  esp_now_register_recv_cb(onDataRecv);

  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, senderAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  esp_now_add_peer(&peerInfo);

  Serial.println("✅ ESP32 B sẵn sàng nhận dữ liệu từ cảm biến...");
}

void loop() {
  // Không cần làm gì, chỉ chờ dữ liệu
}
