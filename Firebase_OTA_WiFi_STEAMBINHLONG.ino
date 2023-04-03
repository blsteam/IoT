/*
        OTA WiFi - Auto Reconnect - Firebase Library
        Written by Kay Chip
        Copyright by Steam Binh Long
*/

// Khai báo thư viện
// Thư viện WiFi
#include <WiFi.h>
// Thư viện giao thức - WiFi OTA
#include <WiFiManager.h>
// Thư viện Firebase - ESP32
#include <FirebaseESP32.h>
// Indentity Firebase
#define FIREBASE_HOST "Nhap firebase host"
#define FIREBASE_AUTH "Nhap firebase auth"
// Khởi tạo hàm truy xuất dữ liệu
FirebaseData firebaseData;
FirebaseJson json;
// Khai báo biến chạy thời gian thực
unsigned long currentTime = 0;
void setup() {
  // Bật mode WiFi (Phát WiFi + Nhận WiFi - STA + AP) 
  WiFi.mode(WIFI_STA);
  // Khởi tạo biến kết nối giao thức OTA WiFi
  WiFiManager wm;
  // Khởi tạo tần số in ra màn hình monitor
  Serial.begin(115200);
  // Tạo hàm kiểm tra điều kiện kết nối OTA
  bool res;
  // Tạo vòng lặp while - lặp tới khi thỏa điều kiện
  while(WiFi.status() != WL_CONNECTED)  // Nếu như trạng thái WiFi mà chưa được kết nối thì nhảy vào điều kiện ngắt
  {
    // Hàm timer - thời gian thực
    if((unsigned long)(millis()) - currentTime <= 40000) // Nếu vi xử lí chạy được thời gian thực từ lúc khởi động tới hiện tại dưới 40s thì thực hiện điều kiện
    {
      // Dò và tự động kết nối WiFi đã lưu
      res = wm.autoConnect();
    }
    // Trường hợp thời gian thực lớn hơn 40s mà chưa dò được WiFi
    else if((unsigned long)(millis()) - currentTime > 40000 && (unsigned long)(millis()) - currentTime <= 120000)
    {
      // Reset bộ nhớ ESP - Phát WiFi để kết nối và sử dụng thủ công
      wm.resetSettings();
    }
    // Trường hợp còn lại (khởi động lại ESP nếu quá thời gian chờ kết nối)
    else
    {
      ESP.restart();
    }
    // Nếu chưa dò được WiFi đèn sẽ tắt
    digitalWrite(2, 0);
  }
  // Đèn tín hiệu trên ESP sẽ sáng nếu dò được WiFi
  digitalWrite(2, 1);
  // Kiểm tra điều kiện
  if(!res)    // Nếu biến res không xác định - false
  {
    // In ra màn hình
    Serial.print("Failed to connect your WiFi");
  }
  else      // Nếu biến res xác định - true
  {
    // In ra màn hình
    Serial.print("Connecting Success !");
  }
  // Kết nối với máy chủ firebase
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  // Bật chế độ tự động kết nối lại máy chủ nếu mất WiFi
  Firebase.reconnectWiFi(true);
  // Trả về trạng thái cuối cùng
  Serial.print("Status: OK");
}

void loop() {
  // Khởi tạo biến hứng dữ liệu
  /* Hứng dữ liệu dạng số nguyên: 
    tenbien = Firebase.getInt(firebaseData, "nhap url databse")
    tenbien = firebaseData.intData();
  */
  /* Hứng dữ liệu dạng số thực: 
    tenbien = Firebase.getFloat(firebaseData, "nhap url databse")
    tenbien = firebaseData.floatData();
  */
  /* Hứng dữ liệu dạng chuỗi: 
    tenbien = Firebase.getString(firebaseData, "nhap url databse")
    tenbien = firebaseData.stringData();
  */
  int led = Firebase.getInt(firebaseData, "led/status");
  led = firebaseData.intData();
  if(led == 1)      // Nếu giá trị nhận từ database bằng 1
  {
    // Bật LED Debug
    digitalWrite(2, 1);
  }
  else      // Nếu giá trị nhận từ database bằng 0
  {
    // Tắt LED Debug
    digitalWrite(2, 0);
  }
}