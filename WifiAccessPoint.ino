#include <WiFi.h>
#include <WebServer.h>

// Thông tin Wi-Fi mà ESP32 sẽ KẾT NỐI VÀO (Station mode)
const char* ssid_STA = "VJU Office";
const char* pass_STA = "VJuoffice@2023";

// Thông tin Wi-Fi mà ESP32 sẽ PHÁT RA (Access Point mode)
const char* ssid_AP = "ThamkhaoChatGPT";
const char* pass_AP = "Chat@12345";

// Tạo web server (chạy trên cả 2 chế độ)
WebServer server(80);

void handleRoot() {
  String html = "<h2>Xin chào từ ESP32!</h2>";
  html += "<p>ESP32 đang chạy chế độ kép (STA + AP)</p>";
  html += "<p>IP Wi-Fi chính (STA): " + WiFi.localIP().toString() + "</p>";
  html += "<p>IP Wi-Fi riêng (AP): " + WiFi.softAPIP().toString() + "</p>";
  server.send(200, "text/html", html);
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("\n=== Bắt đầu chế độ kép Wi-Fi (STA + AP) ===");

  // 1️⃣ Kết nối vào Wi-Fi có sẵn
  WiFi.mode(WIFI_AP_STA);  // Bật cả 2 chế độ
  WiFi.begin(ssid_STA, pass_STA);
  Serial.print("🔄 Đang kết nối tới Wi-Fi: ");
  Serial.println(ssid_STA);

  int retry = 0;
  while (WiFi.status() != WL_CONNECTED && retry < 20) {
    delay(500);
    Serial.print(".");
    retry++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n✅ Đã kết nối Wi-Fi thành công!");
    Serial.print("🌐 IP STA: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\n⚠️ Không thể kết nối Wi-Fi STA!");
  }

  // 2️⃣ Phát Wi-Fi riêng (Access Point)
  bool ap_ok = WiFi.softAP(ssid_AP, pass_AP);
  if (ap_ok) {
    Serial.println("✅ Tạo Wi-Fi Access Point thành công!");
    Serial.print("📶 Tên Wi-Fi AP: ");
    Serial.println(ssid_AP);
    Serial.print("🌐 IP AP: ");
    Serial.println(WiFi.softAPIP());
  } else {
    Serial.println("❌ Không thể tạo Wi-Fi Access Point!");
  }

  // 3️⃣ Cấu hình web server
  server.on("/", handleRoot);
  server.begin();
  Serial.println("🚀 Web server đã khởi động!");
}

void loop() {
  server.handleClient();
}
