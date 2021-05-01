/**
 * Demo：
 *    演示Http请求天气接口信息
 * @author 单片机菜鸟
 * @date 2019/09/09
 */
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include <Servo.h>
Servo myservo;
 
//以下三个定义为调试定义
#define DebugBegin(baud_rate)    Serial.begin(baud_rate)
#define DebugPrintln(message)    Serial.println(message)
#define DebugPrint(message)    Serial.print(message)
 
const char* AP_SSID     = "mamamiya";         // XXXXXX -- 使用时请修改为当前你的 wifi ssid
const char* AP_PSK = "liyongjin1998";         // XXXXXX -- 使用时请修改为当前你的 wifi 密码
//const char* HOST = "http://api.seniverse.com";
//const char* APIKEY = "wcmquevztdy1jpca";        //API KEY
//const char* CITY = "guangzhou";
//const char* LANGUAGE = "zh-Hans";//zh-Hans 简体中文  会显示乱码
  
const unsigned long BAUD_RATE = 115200;                   // serial connection speed
const unsigned long HTTP_TIMEOUT = 5000;               // max respone time from server
 
// 我们要从此网页中提取的数据的类型
//struct WeatherData {
//  char city[16];//城市名称
//  char weather[32];//天气介绍（多云...）
//  char temp[16];//温度
//  char udate[32];//更新时间
//};

struct ResponseData{
  bool key1;
  int key2;
  String key3;
  };
 
HTTPClient http;
String GetUrl;
String response;
ResponseData responsedata;
//WeatherData weatherData;
 
void setup() {
  myservo.attach(5);
  pinMode(LED_BUILTIN, OUTPUT);
  // put your setup code here, to run once:
  WiFi.mode(WIFI_STA);     //设置esp8266 工作模式
  DebugBegin(BAUD_RATE);
  DebugPrint("Connecting to ");//写几句提示，哈哈
  DebugPrintln(AP_SSID);
  WiFi.begin(AP_SSID, AP_PSK);   //连接wifi
  WiFi.setAutoConnect(true);
  while (WiFi.status() != WL_CONNECTED) {
    //这个函数是wifi连接状态，返回wifi链接状态
    delay(500);
    DebugPrint(".");
  }
  DebugPrintln("");
  DebugPrintln("WiFi connected");
  DebugPrintln("IP address: " + WiFi.localIP());
 
  //拼接get请求url  博哥后面考虑看看是否可以封装一个方法来用用 不需要自己一个个拼装这个url
//  GetUrl = String(HOST) + "/v3/weather/now.json?key=";
//  GetUrl += APIKEY;
//  GetUrl += "&location=";
//  GetUrl += CITY;
//  GetUrl += "&language=";
//  GetUrl += LANGUAGE;
  GetUrl = "http://www.futureseed.top/locking/";
  //设置超时
  http.setTimeout(HTTP_TIMEOUT);
  //设置请求url
  http.begin(GetUrl);
  //以下为设置一些头  其实没什么用 最重要是后端服务器支持
//  http.setUserAgent("esp8266");//用户代理版本
//  http.setAuthorization("esp8266","boge");//用户校验信息
}
 
void loop() {
  //心知天气  发送http  get请求
  int httpCode = http.GET();
  if (httpCode > 0) {
      Serial.printf("[HTTP] GET... code: %d\n", httpCode);
      //判断请求是否成功
      if (httpCode == HTTP_CODE_OK) {
        //读取响应内容
        response = http.getString();
        DebugPrintln("Get the data from Internet!");
        DebugPrintln(response);
        //解析响应内容
        if (parseUserData(response, &responsedata)) {
          //打印响应内容
          if(responsedata.key1){
              DebugPrintln(responsedata.key1);
              DebugPrintln("Built in Led on");
              digitalWrite(LED_BUILTIN, HIGH);
              myservo.write(0);
              delay(5000);
              digitalWrite(LED_BUILTIN, LOW);
              myservo.write(90);
              delay(1000);
            }
          printUserData(&responsedata);
        }
//        if (parseUserData(response, &weatherData)) {
//          //打印响应内容
//          printUserData(&weatherData);
//        }
      }
  } else {
      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }
  http.end();
  delay(1000);//每1s调用一次 
}
  
/**
 * @Desc 解析数据 Json解析
 * 数据格式如下：
 * {
 *    {"key1": true, "key2": 123, "key3": "abc"}
 *}
 */
bool parseUserData(String content, struct ResponseData* responsedata) {
//    -- 根据我们需要解析的数据来计算JSON缓冲区最佳大小
//   如果你使用StaticJsonBuffer时才需要
//    const size_t BUFFER_SIZE = 1024;
//   在堆栈上分配一个临时内存池
//    StaticJsonBuffer<BUFFER_SIZE> jsonBuffer;
//    -- 如果堆栈的内存池太大，使用 DynamicJsonBuffer jsonBuffer 代替
  DynamicJsonBuffer jsonBuffer;
   
  JsonObject& root = jsonBuffer.parseObject(content);
   
  if (!root.success()) {
    DebugPrintln("JSON parsing failed!");
    return false;
  }
    
  //复制我们感兴趣的字符串
  //  -- 这不是强制复制，你可以使用指针，因为他们是指向“内容”缓冲区内，所以你需要确保
  //   当你读取字符串时它仍在内存中
  responsedata->key1 = root["key1"];
  //strcpy(responsedata->key1, root["key1"]);
  responsedata->key2 = root["key2"];
  //strcpy(responsedata->key2, root["key2"]);
  //responsedata->key3 = root["key3"];
  //strcpy(responsedata->key3, root["key3"]);
  
  return true;
  
}
 //打印从JSON中提取的数据
void printUserData(const struct ResponseData* responsedata) {
  DebugPrintln("Print parsed data :");
  DebugPrint("Key1 : ");
  DebugPrint(responsedata->key1);
  DebugPrint(", \t");
  DebugPrint("Key2 : ");
  DebugPrint(responsedata->key2);
  DebugPrint(",\t");
  DebugPrint("Key3 : ");
  DebugPrint(responsedata->key3[10]);
}
// 打印从JSON中提取的数据
//void printUserData(const struct WeatherData* weatherData) {
//  DebugPrintln("Print parsed data :");
//  DebugPrint("City : ");
//  DebugPrint(weatherData->city);
//  DebugPrint(", \t");
//  DebugPrint("Weather : ");
//  DebugPrint(weatherData->weather);
//  DebugPrint(",\t");
//  DebugPrint("Temp : ");
//  DebugPrint(weatherData->temp);
//  DebugPrint(" C");
//  DebugPrint(",\t");
//  DebugPrint("Last Updata : ");
//  DebugPrint(weatherData->udate);
//  DebugPrintln("\r\n");
//}
