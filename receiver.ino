#include <WiFi.h>
#include <esp_now.h>

// Cấu trúc dữ liệu nhận
typedef struct struct_message {
  char msg[50];
} struct_message;

struct_message incomingData;

// Callback khi nhận dữ liệu
void OnDataRecv(const esp_now_recv_info *info, const uint8_t *data, int data_len) {
  memcpy(&incomingData, data, sizeof(incomingData));

  Serial.print("📩 Nhận từ MAC (Sender): ");
  for (int i = 0; i < 6; i++) {
    Serial.printf("%02X", info->src_addr[i]);
    if (i < 5) Serial.print(":");
  }
  Serial.print(" | Dữ liệu: ");
  Serial.println(incomingData.msg);
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Station mode bắt buộc cho ESP-NOW
  WiFi.mode(WIFI_STA);

  // Khởi tạo ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("❌ Lỗi khởi tạo ESP-NOW");
    return;
  }

  // Đăng ký callback nhận dữ liệu
  esp_now_register_recv_cb(OnDataRecv);

  Serial.println("✅ ESP32 Receiver sẵn sàng nhận dữ liệu...");
}

void loop() {
  // Không cần làm gì thêm
}
