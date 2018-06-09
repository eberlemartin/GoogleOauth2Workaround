#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

/*
1. Set up your firebase project, enable authentication through Google
2. Create a Oauth2.0-Client-ID in developers console and copy Client-ID and Client-Secret into this sketch.
3. Replace "<your_client_id>" in this URL with your client_id and open it in your browser:
    https://accounts.google.com/o/oauth2/auth?redirect_uri=https://localhost&response_type=code&client_id=<your_client_id>&scope=https://www.googleapis.com/auth/firebase.database+https://www.googleapis.com/auth/userinfo.email&approval_prompt=force&access_type=offline
4. Log-in/select with the Google User you would like to authorize.
5. Copy the code after "?code=" from the URL of your browser into this sketch. (without the # at the end)
    This code works only once, if you try to use it in Oauthplayground or somewhere else it wont work here anymore.
    (open URL again and get new code if you used it already)
4. Run the sketch.
5. Save all data printed to Serial, it is the access_token and the refresh_token you will need.
*/

#define WIFI_SSID "ssid"
#define WIFI_PASSWORD "password"
String client_id = ""; //enter your Oauth2 account client id
String client_secret = ""; //enter your Oauth2 account client secret
String code = ""; //enter the code you recieved in the browser url field (only code, without beginning https://... or  &code=...)
String message = "redirect_uri=https://localhost&client_id=" + client_id + "&client_secret=" + client_secret + "&code=" + code + "&grant_type=authorization_code";

WiFiClientSecure client;

void connectWifi(){
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);
  WiFi.mode(WIFI_STA);
  if(WiFi.status() != WL_CONNECTED){
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  }
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void setup() {
  DynamicJsonBuffer jsonBuffer;
  String response;
  
  Serial.begin(9600);
  delay(5000);
  connectWifi();

  Serial.println("Starting to redeem token from code...");
  if(!client.connect("accounts.google.com", 443)){
    Serial.println("Could not connect to accounts.google.com");
  }

  client.print( String("POST /o/oauth2/token HTTP/1.1\r\n") +
                String("Host: accounts.google.com\r\n") +
                String("Content-Type: application/x-www-form-urlencoded\r\n") +
                String("Content-Length: " + String(message.length()) + "\r\n\r\n") +
                message
              );

  unsigned long sendTime = millis();
  while(client.available() == 0){
    if(millis() - sendTime > 5000){
      client.stop();
      Serial.println("Failed to recieve response after 5 seconds, aborting");
      break;
    }
  }

  while(client.available()){
    response = client.readString();
    int payloadStart = response.indexOf("\r\n\r\n");
    int payloadEnd = response.indexOf("\r\n\r\n", payloadStart+1);
    response = response.substring(payloadStart, payloadEnd);
    payloadStart = response.indexOf('{');
    payloadEnd = response.indexOf('}');
    response = response.substring(payloadStart, payloadEnd+1);
    response.trim();
  }
  JsonObject& responseJson = jsonBuffer.parseObject(response);

  if(responseJson.containsKey("error")){
    responseJson.printTo(Serial);
  } else {
    Serial.println("Your Oauth2 token from google:");
    Serial.println("Access Token: " + responseJson["access_token"].as<String>());
    Serial.println("Refresh Token: " + responseJson["refresh_token"].as<String>());
    Serial.println("Token Type: " + responseJson["token_type"].as<String>());
    Serial.println("Expires in (seconds): " + responseJson["expires_in"].as<String>());
    Serial.println("ID Token: " + responseJson["id_token"].as<String>());
  }
}

void loop() {
    // nothing to do...
}

