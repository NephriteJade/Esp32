#include <WiFi.h>

// Cấu hình Wi-Fi Access Point
const char* ssid = "CHAT GPT";
const char* password = "Chat@12345";

WiFiServer server(80);   // Cổng TCP
const int ledPin = 2;    // LED nối chân D2 hoặc GPIO2

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  // Tạo WiFi AP
  WiFi.softAP(ssid, password);

  Serial.println("WiFi AP đã tạo thành công!");
  Serial.print("Tên WiFi: "); Serial.println(ssid);
  Serial.print("IP: "); Serial.println(WiFi.softAPIP());

  // Bắt đầu server
  server.begin();
  Serial.println("Server TCP đang chạy...");
}

void loop() {
  WiFiClient client = server.available();   // Kiểm tra có client kết nối không

  if (client) {
    Serial.println("Client đã kết nối!");
    String data = "";

    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        if (c == '\n') {
          data.trim();
          Serial.print("Dữ liệu nhận được: ");
          Serial.println(data);

          if (data == "ON") {
            digitalWrite(ledPin, HIGH);
            client.println("LED đã bật");
          } else if (data == "OFF") {
            digitalWrite(ledPin, LOW);
            client.println("LED đã tắt");
          } else {
            client.println("Lệnh không hợp lệ");
          }
          data = "";
        } else {
          data += c;
        }
      }
    }

    client.stop();
    Serial.println("Client ngắt kết nối.");
  }
}
