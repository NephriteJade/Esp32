#include <WiFi.h>

const char* ssid = "VJU Office";
const char* password = "VJuOffice@2023";

WiFiClient client;
IPAddress server(192,168,225,108); // IP máy tính bạn
const int port = 9999;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.print("Đang kết nối Wi-Fi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\n✅ Đã kết nối Wi-Fi!");
  Serial.print("Địa chỉ IP ESP32: ");
  Serial.println(WiFi.localIP());

  Serial.printf("Kết nối tới server %s:%d ...\n", server.toString().c_str(), port);
  
  if (client.connect(server, port)) {
    Serial.println("✅ Kết nối server thành công!");
    client.println("Xin chào, tôi là ESP32!");
  } else {
    Serial.println("❌ Không thể kết nối server!");
  }
}

void loop() {
  if (client.connected()) {
    if (client.available()) {
      String msg = client.readStringUntil('\n');
      Serial.print("📩 Nhận được từ server: ");
      Serial.println(msg);
      delay(5000);
      client.println("ESP32 gửi lại sau 5 giây!");
    }
  } else {
    Serial.println("⚠️ Mất kết nối server!");
    delay(2000);
  }
}
