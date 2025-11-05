#include <WiFi.h>

// --- Cấu hình Station (kết nối Wi-Fi có sẵn) ---
const char* sta_ssid = "VJU Guest";
const char* sta_password = "Vjuguest@2024";

// --- Cấu hình Access Point (phát Wi-Fi riêng) ---
const char* ap_ssid = "ThamKhaoChat";
const char* ap_password = "Chat@12345";

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println("=== ESP32 ở chế độ AP + STA (Dual Mode) ===");

  // Bắt đầu Wi-Fi Station (kết nối đến Wi-Fi router)
  WiFi.mode(WIFI_AP_STA);

  // Cấu hình Access Point
  bool ap_ok = WiFi.softAP(ap_ssid, ap_password);
  if (ap_ok) {
    Serial.println("✅ Đã bật chế độ Access Point!");
    Serial.print("Tên Wi-Fi: ");
    Serial.println(ap_ssid);
    Serial.print("Mật khẩu: ");
    Serial.println(ap_password);
    Serial.print("Địa chỉ IP AP: ");
    Serial.println(WiFi.softAPIP());
  } else {
    Serial.println("❌ Không thể bật AP!");
  }

  // Kết nối đến Wi-Fi STA
  Serial.println();
  Serial.print("🔗 Đang kết nối đến Wi-Fi: ");
  Serial.println(sta_ssid);
  WiFi.begin(sta_ssid, sta_password);

  int retry = 0;
  while (WiFi.status() != WL_CONNECTED && retry < 20) {
    delay(500);
    Serial.print(".");
    retry++;
  }

  Serial.println();
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("✅ Đã kết nối Wi-Fi (STA) thành công!");
    Serial.print("Địa chỉ IP STA: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("❌ Không thể kết nối đến Wi-Fi STA!");
  }

  Serial.println("\n=== Kết quả tổng hợp ===");
  Serial.print("AP IP:  ");
  Serial.println(WiFi.softAPIP());
  Serial.print("STA IP: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // In lại IP mỗi 10 giây để kiểm tra
  static unsigned long lastPrint = 0;
  if (millis() - lastPrint > 10000) {
    lastPrint = millis();
    Serial.println();
    Serial.println("🔄 Cập nhật IP:");
    Serial.print("AP IP:  ");
    Serial.println(WiFi.softAPIP());
    Serial.print("STA IP: ");
    Serial.println(WiFi.localIP());
  }
}
