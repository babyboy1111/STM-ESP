#include <DHT.h>


#ifdef ESP32
  #include <WiFi.h>
  #include <HTTPClient.h>
#else
  #include <ESP8266WiFi.h>
  #include <ESP8266HTTPClient.h>
  #include <WiFiClient.h>
#endif

#include <Wire.h>

#define DHTPIN 4 

#define DHTTYPE DHT11 // DHT 11

DHT dht(DHTPIN, DHTTYPE);

const char* ssid     = "Sinh Vien Home";
const char* password = "06117370";


const char* serverName = "http://dighuyniot.tech/demo/postdemo.php";
const char* serverName1 = "http://dighuyniot.tech/demo/postdemo_comment.php";


String apiKeyValue = "tPmAT5Ab3j7F9";



int value_analog,real_value;

void setup() {
  Serial.begin(115200);
  
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  
  dht.begin();
  pinMode(5,INPUT_PULLUP); 
}
void comment()
{
  if(WiFi.status()== WL_CONNECTED){
    WiFiClient client;
    HTTPClient http;
    
    
    http.begin(client, serverName1);
    
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    
    String httpRequestData = "api_key=" + apiKeyValue + "&comment_subject=" + "Toi"
                           + "&comment_text=" + "Se" + "&comment_status=" + "0" + "";
    Serial.print("httpRequestData: ");
    Serial.println(httpRequestData);
    
   

    int httpResponseCode = http.POST(httpRequestData);
     
   
    
    if (httpResponseCode>0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
    }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    
    http.end();
  }
  else {
    Serial.println("WiFi Disconnected");
  }
  
}
  void loop() {

  for(int i=0;i<=9;i++){
    real_value+=analogRead(A0);
  }
  value_analog=real_value/10;
  float percent = (float)(1024-value_analog)*100/684 ;
                         // Tính giá trị phần trăm thực. Chuyển điện thế từ 3.3V ( khô ) thành 3.3V ( ẩm )
  Serial.print(percent);
  Serial.print('%');
  Serial.print(" ");
  Serial.print("Gia tri analog: ");
  Serial.print(value_analog);
  Serial.print(" ");
  Serial.print("Gia tri digital: ");
  Serial.println(digitalRead(5));
  delay(200);
  real_value=0;
  //------------------------------------------------------------------------------------
  // Wait a few seconds between measurements.
 delay(2000);

// Reading temperature or humidity takes about 250 milliseconds!
 // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
 float h = dht.readHumidity();
 // Read temperature as Celsius (the default)
 float t = dht.readTemperature();
 // Read temperature as Fahrenheit (isFahrenheit = true)
 float f = dht.readTemperature(true);

// Check if any reads failed and exit early (to try again).
 if (isnan(h) || isnan(t) || isnan(f)) {
 Serial.println("Failed to read from DHT sensor!");
 return;
 }

// Compute heat index in Fahrenheit (the default)
 float hif = dht.computeHeatIndex(f, h);
 // Compute heat index in Celsius (isFahreheit = false)
 float hic = dht.computeHeatIndex(t, h, false);

Serial.print("Humidity: ");
 Serial.print(h);
 Serial.print(" %\t");
 Serial.print("Temperature: ");
 Serial.print(t);
 Serial.print(" *C ");
 Serial.print(f);
 Serial.print(" *F\t");
 Serial.print("Heat index: ");
 Serial.print(hic);
 Serial.print(" *C ");
 Serial.print(hif);
 Serial.println(" *F");
   //----------------------------------------------------------------------------------------------
  //Check WiFi connection status
  if(WiFi.status()== WL_CONNECTED){
    WiFiClient client;
    HTTPClient http;
    
    // Your Domain name with URL path or IP address with path
    http.begin(client, serverName);
    
    // Specify content-type header
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    
    // Prepare your HTTP POST request data
    String httpRequestData = "api_key=" + apiKeyValue + "&value1=" + String(t)
                           + "&value2=" + String(h) + "&value3=" + String(percent) + "";
    Serial.print("httpRequestData: ");
    Serial.println(httpRequestData);
    
    int httpResponseCode = http.POST(httpRequestData);
     
   
    
    if (httpResponseCode>0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
    }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    // Free resources
    http.end();
  }
  else {
    Serial.println("WiFi Disconnected");
  }
  //Send an HTTP POST request every 30 seconds
  delay(1800000);  
}
