#include <WiFi.h>

// Cấu hình Wi-Fi Access Point
const char* SSID = "MyESP32";
const char* PASSWORD = "Juan123";

void setup() {
  Serial.begin(115200);
  delay(500);  // Đảm bảo Serial sẵn sàng

  // Cấu hình IP tĩnh cho Access Point
  IPAddress apIP(192, 168, 4, 1);
  IPAddress subnet(255, 255, 255, 0);

  // Khởi tạo Access Point
  WiFi.softAPConfig(apIP, apIP, subnet);
  WiFi.softAP(SSID, PASSWORD);

  // Hiển thị thông tin
  Serial.println("\n===== ESP32 Access Point Started =====");
  Serial.printf("SSID: %s\n", SSID);
  Serial.printf("Password: %s\n", PASSWORD);
  Serial.printf("AP IP Address: %s\n", WiFi.softAPIP().toString().c_str());
  Serial.println("======================================");
}

void loop() {
  static int lastClientCount = -1;  // Lưu số client lần trước
  int currentClientCount = WiFi.softAPgetStationNum();

  // Chỉ in khi có thay đổi
  if (currentClientCount != lastClientCount) {
    Serial.printf("📶 Connected devices: %d\n", currentClientCount);
    lastClientCount = currentClientCount;
  }

  delay(2000);
}

