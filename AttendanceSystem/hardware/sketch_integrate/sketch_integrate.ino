#include <SPI.h>
#include <MFRC522.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <string.h>

//设置你的wifi接入信息
#define STASSID "mamamiya"
#define STAPSK  "Mamamiya2021"
#define SERVER_IP "http://192.168.0.163:8000/api/attendance"

//定义RFID的引脚 esp8266
#define SS_PIN 2  //D4
#define RST_PIN 0 //D3

//定义rfid
MFRC522 mfrc522(SS_PIN, RST_PIN); // Instance of the class
MFRC522::MIFARE_Key key; 
byte nuidPICC[4];

void setup() { 
  //初始化串口设置
  Serial.begin(115200);
  //初始化RFID 模块
  SPI.begin(); // Init SPI bus
  mfrc522.PCD_Init(); // Init MFRC522 

  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }

  //初始化wifi模块
  //开始连接wifi
  WiFi.begin(STASSID, STAPSK);
 
  //等待WiFi连接,连接成功打印IP
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  //打印设备状态
  Serial.println("");
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());
}
 
void loop() {
 // Prepare key - all keys are set to FFFFFFFFFFFFh at chip delivery from the factory.
  MFRC522::MIFARE_Key key;
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;

  //some variables we need
  byte block;
  byte len = 18;
  MFRC522::StatusCode status;

  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
  if ( ! mfrc522.PICC_IsNewCardPresent())
    return;

  // Verify if the NUID has been readed
  if ( ! mfrc522.PICC_ReadCardSerial())
    return;

  Serial.print(F("**Card Detected ---- PICC type: "));
  MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
  Serial.println(mfrc522.PICC_GetTypeName(piccType));
  mfrc522.PICC_DumpDetailsToSerial(&(mfrc522.uid)); //dump some details about the card

 //---------------------------------------- GET ID NUMBER

  byte buffer2[16];
  block = 1;

  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 1, &key, &(mfrc522.uid)); //line 834
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Authentication failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
  //读取的时候len=16会报错“A buffer is not big enough.”
  status = mfrc522.MIFARE_Read(block, buffer2, &len);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Reading failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }

  //PRINT LAST NAME
  for (uint8_t i = 0; i < 16; i++) {
    Serial.write(buffer2[i] );
  }


  //----------------------------------------

    // Store NUID into nuidPICC array
    for (byte i = 0; i < 4; i++) {
      nuidPICC[i] = mfrc522.uid.uidByte[i];
    }

    //打印读取到的卡片信息
    Serial.println(F("The NUID tag is:"));
    Serial.print(F("In hex: "));
    printHex(mfrc522.uid.uidByte, mfrc522.uid.size);
    Serial.println();
    Serial.print(F("In dec: "));
    printDec(mfrc522.uid.uidByte, mfrc522.uid.size);
    Serial.println();

    //发送http请求，并接受返回
    if ((WiFi.status() == WL_CONNECTED)) {
      //执行发送操作开始------------------------------------------------
      //发送post请求
      http_post(buffer2);
      //执行发送操作结束------------------------------------------------
      //PRINT ID NUMBER
      for (uint8_t i = 0; i < 16; i++) {
        Serial.write(buffer2[i] );
      }
    }
    
  // Halt PICC
  mfrc522.PICC_HaltA();

  // Stop encryption on PCD
  mfrc522.PCD_StopCrypto1();

  delay(1000);
}


void printHex(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}

void printDec(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], DEC);
  }
}

void http_post(byte *buffer) {
 
  //创建 WiFiClient 实例化对象
  WiFiClient client;
  //创建http对象
  HTTPClient http;
 
  //配置请求地址
  http.begin(client, SERVER_IP); //HTTP请求
  Serial.print("[HTTP] begin...\n");
 
  //启动连接并发送HTTP报头和报文
  int httpCode = http.POST((char *)buffer);
  Serial.print("[HTTP] POST...\n");
//  Serial.print("[HTTP] body:%c\n",(unsigned char*)buffer);
 
  //如果连接成功
  if (httpCode > 0) {
 
    //将服务器响应头打印到串口
    Serial.printf("[HTTP] POST... code: %d\n", httpCode);
 
    //将从服务器获取的数据打印到串口
    if (httpCode == HTTP_CODE_OK) {
      const String& payload = http.getString();
      Serial.println("received payload:\n<<");
      Serial.println(payload);
      Serial.println(">>");
    }
  } 
  //连接失败时 httpCode时为负
  else {
    Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }
  //关闭http连接
  http.end();
}
