#include <WiFi.h>

// Thông tin WiFi của ESP32 Server
const char* ssid = "CHAT GPT";
const char* password = "Chat@12345";

const char* host = "192.168.4.1"; // IP mặc định của ESP32 ở chế độ AP
const uint16_t port = 80;

WiFiClient client;

void setup() {
  Serial.begin(115200);
  delay(500);

  Serial.println("Kết nối đến WiFi Server...");
  WiFi.begin(ssid, password);

  // Chờ kết nối WiFi
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nĐã kết nối WiFi!");
  Serial.print("Địa chỉ IP của Client: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  if (!client.connected()) {
    Serial.println("Kết nối tới Server...");
    if (client.connect(host, port)) {
      Serial.println("Đã kết nối server thành công!");
    } else {
      Serial.println("Kết nối thất bại, thử lại...");
      delay(2000);
      return;
    }
  }

  // Gửi lệnh ON/OFF (thử thay đổi)
  Serial.println("Gửi lệnh ON...");
  client.println("ON");
  delay(3000);

  Serial.println("Gửi lệnh OFF...");
  client.println("OFF");
  delay(3000);

  // Đọc phản hồi từ server
  while (client.available()) {
    String response = client.readStringUntil('\n');
    Serial.print("Server trả về: ");
    Serial.println(response);
  }
}
