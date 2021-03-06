#include "EmonLib.h"   
#include <WiFi.h>
#include <WiFiClient.h>
#include <Wire.h>
#include "ESP32_MailClient.h"
 #include "Adafruit_INA219.h"

#define emailSenderAccount    "ghuyniot@gmail.com"    
#define emailSenderPassword   "weekndcold1"
#define emailRecipient        "nguyenkhaikh321@gmail.com"
#define smtpServer            "smtp.gmail.com"
#define smtpServerPort        465
#define emailSubject          "NodeWifi32 Test"
 SMTPData smtpData;
#define LED 2 
#define button 0
//////////DC volt///////////
#define ANALOG_IN_PIN 34
 
// Floats for ADC voltage & Input voltage
float adc_voltage = 0.0;
float in_voltage = 0.0;
 
// Floats for resistor values in divider (in ohms)
float R1 = 30000.0;
float R2 = 7500.0; 
 
// Float for Reference Voltage
float ref_voltage = 5.0;
 
// Integer for ADC value
int adc_value = 0;
///////////////////////////
String apiKeyValue = "tPmAT5Ab3j7F9";
Adafruit_INA219 ina219;
int check=0;
int checkDCngan=0;
int checkDCho=0;
boolean nut=true;
 //Access point credentials
const char* ssid = "Sinh Vien Home";
const char* password = "06117370";
unsigned long startMillis;  //some global variables available anywhere in the program
unsigned long currentMillis;
const unsigned long period = 3000000; 
float kWh = 0;
unsigned long lastmillis = millis();
 WiFiServer server(80);  // open port 80 for server connection
void sendCallback(SendStatus info);

void IRAM_ATTR isr(){
  if (nut){
    nut=false;
  }else{nut=true;}
}

void setup() {
    pinMode(LED,OUTPUT);
  WiFi.begin(ssid, password);
    //starting the server
    server.begin();
    startMillis = millis();
    Serial.begin(9600);
  attachInterrupt(button,isr, FALLING);
  if (! ina219.begin()) {
    Serial.println("Failed to find INA219 chip");
    while (1) { delay(10); }
  }
  delay(2000);
  }
 void send_mail(String noidung)
  {
    // Set the SMTP Server Email host, port, account and password
  smtpData.setLogin(smtpServer, smtpServerPort, emailSenderAccount, emailSenderPassword);
  // Set the sender name and Email
  smtpData.setSender("SmartGarden", emailSenderAccount);
  // Set Email priority or importance High, Normal, Low or 1 to 5 (1 is highest)
  smtpData.setPriority("High");
  // Set the subject
  smtpData.setSubject(emailSubject);
  // Set the message with HTML format
  smtpData.setMessage("<div style=\"color:#2f4468;\"><h1>"+noidung+"</p></div>", true);
  // Set the email message in text format (raw)
  //smtpData.setMessage("Hello World! - Sent from NodeWifi32 board", false);
  // Add recipients, you can add more than one recipient
  smtpData.addRecipient(emailRecipient);
  //smtpData.addRecipient("YOUR_OTHER_RECIPIENT_EMAIL_ADDRESS@EXAMPLE.com");
  smtpData.setSendCallback(sendCallback);
  //Start sending Email, can be set callback function to track the status
  if (!MailClient.sendMail(smtpData))
    Serial.println("Error sending Email, " + MailClient.smtpErrorReason());
  //Clear all data from Email object to free memory
  smtpData.empty();
  }
  // Callback function to get the Email sending status
void sendCallback(SendStatus msg) {
  // Print the current status
  Serial.println(msg.info());
  // Do something when complete
  if (msg.success()) {
    Serial.println("----------------");
  }
} 

//do dien dap///////////////////////////////////////////////////////////////////////
void dodienap(){
  ////////////////////do dong
  float shuntvoltage = 0;
  float busvoltage = 0;
  float current_mA = 0;
  float loadvoltage = 0;
  float power_mW = 0;
  for(int i=0;i<10;i++){
  shuntvoltage += ina219.getShuntVoltage_mV();
  busvoltage += ina219.getBusVoltage_V();
  current_mA += ina219.getCurrent_mA();
  power_mW += ina219.getPower_mW();
  loadvoltage += busvoltage + (shuntvoltage / 1000);
  }
  busvoltage=busvoltage/10;
  shuntvoltage=shuntvoltage/10;
  loadvoltage=loadvoltage/10;
  current_mA=current_mA/10;
  power_mW=power_mW/10;
   Serial.print(busvoltage); Serial.print("\t"); 
  Serial.print(shuntvoltage); Serial.print("\t");
  Serial.print(loadvoltage); Serial.print("\t");
  Serial.print(current_mA); Serial.print("\t");
  Serial.println(power_mW);
  ////////////////////////////////////DO AP
  // Read the Analog Input
   adc_value = analogRead(ANALOG_IN_PIN);
   
   // Determine voltage at ADC input
   adc_voltage  = (adc_value * ref_voltage) / 4096.0; 
   
   // Calculate voltage at divider input
   in_voltage = adc_voltage / (R2/(R1+R2)) ; 
   
   // Print results to Serial Monitor to 2 decimal places
  Serial.print("Input Voltage = ");
  Serial.println(in_voltage, 2);

  
    if (in_voltage==0 && power_mW==0&&check==0 ){
      send_mail("Ngan mach DC");
      comment("Mach hong","Ngan mach","0");
      check=1;
    }
//  if (loadvoltage<10&&power_mW>10&&power_mW<100&&busvoltage<1&& checkDCngan==0){
//    comment("Mach hong","Ngan mach","0");
//    checkDCngan=1;
//    send_mail("Ngan Mach");
//  }
  if (in_voltage!=0 &&power_mW==0&& checkDCho==0){
    comment("Mach hong","Ho mach","0");
    checkDCho=1;
    send_mail("Ho mach");
  }
  delay(1000);
}
void comment(String comment_subject,String comment_text,String comment_status )
{
//  if(WiFi.status()== WL_CONNECTED){
    WiFiClient client;
    HTTPClient http;
    const char* serverName1 = "http://dighuyniot.tech/demo/postdemo_comment.php";
    // Your Domain name with URL path or IP address with path
    http.begin(client, serverName1);
    
    // Specify content-type header
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    
    // Prepare your HTTP POST request data
    String httpRequestData = "api_key=" + apiKeyValue + "&comment_subject=" + comment_subject
                           + "&comment_text=" + comment_text + "&comment_status=" + comment_status + "";
    Serial.print("httpRequestData: ");
    Serial.println(httpRequestData);
    
    // You can comment the httpRequestData variable above
    // then, use the httpRequestData variable below (for testing purposes without the BME280 sensor)
    //String httpRequestData = "api_key=tPmAT5Ab3j7F9&value1=24.75&value2=49.54&value3=1005.14";

    // Send HTTP POST request
    int httpResponseCode = http.POST(httpRequestData);
     
    // If you need an HTTP request with a content type: text/plain
    //http.addHeader("Content-Type", "text/plain");
    //int httpResponseCode = http.POST("Hello, World!");
    
    // If you need an HTTP request with a content type: application/json, use the following:
    //http.addHeader("Content-Type", "application/json");
    //int httpResponseCode = http.POST("{\"value1\":\"19\",\"value2\":\"67\",\"value3\":\"78\"}");
    
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
//  }
//  else {
//    Serial.println("WiFi Disconnected");
//  }
  //Send an HTTP POST request every 30 seconds
  //delay(30000);  
}
void loop() {
if(nut){
   Serial.println("CHE DO AC");
   delay(2000);
   digitalWrite(LED,HIGH);   
   //myTimerEvent();
   currentMillis = millis();
   dodienap();
   if(currentMillis-startMillis>= period){
    check=0;
    checkDCngan=0;
    checkDCho=0;
    startMillis = currentMillis;
   }
}else{
  Serial.println("CHE DO DC");
   delay(2000);
   digitalWrite(LED,LOW);  
}
}
