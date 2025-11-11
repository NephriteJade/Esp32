#include <WiFi.h>
#include <esp_now.h>

// Địa chỉ MAC của ESP32 Receiver (bạn đã lấy trước đó)
uint8_t receiverAddress[] = {0x88, 0x57, 0x21, 0xB6, 0xB4, 0xB4};

// Cấu trúc dữ liệu gửi
typedef struct struct_message {
  char msg[50];
} struct_message;

struct_message myData;

// Callback khi gửi dữ liệu xong
void OnDataSent(const wifi_tx_info_t *info, esp_now_send_status_t status) {
  Serial.print("📡 Trạng thái gửi: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "✅ Thành công" : "❌ Thất bại");
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Bật WiFi chế độ Station
  WiFi.mode(WIFI_STA);

  Serial.println("\n🔰 ESP32 Sender đang khởi động...");
  Serial.print("📡 MAC của Sender: ");
  Serial.println(WiFi.macAddress());

  // Khởi tạo ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("❌ Lỗi khởi tạo ESP-NOW");
    return;
  }

  // Đăng ký callback gửi xong
  esp_now_register_send_cb(OnDataSent);

  // Thêm peer (Receiver)
  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, receiverAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("❌ Lỗi thêm peer");
    return;
  }

  Serial.println("✅ ESP32 Sender sẵn sàng gửi dữ liệu...");
}

void loop() {
  // Gửi thông điệp thử nghiệm
  strcpy(myData.msg, "Hello từ ESP32 Sender!");
  esp_err_t result = esp_now_send(receiverAddress, (uint8_t *)&myData, sizeof(myData));

  if (result == ESP_OK) {
    Serial.println("📤 Gửi gói tin thành công!");
  } else {
    Serial.println("⚠️ Gửi thất bại!");
  }

  delay(2000); // Gửi mỗi 2 giây
}
