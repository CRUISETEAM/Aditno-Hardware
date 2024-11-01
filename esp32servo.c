/* https://www.hivemq.com/demos/websocket-client/ */
#include <WiFi.h>
#include <PubSubClient.h>
#include <ESP32Servo.h>  

const char* ssid = "bssm_free";        
const char* password = "bssm_free"; 
const char* mqtt_server = "mqtt-dashboard.com";  
const int mqtt_port = 1883;            
const char* topic = "bssm/servomotor";   

WiFiClient espClient;
PubSubClient client(espClient);
Servo myServo;

int angle = 0;           
int prevState = -1;     

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  client.setServer(mqtt_server, mqtt_port);

  myServo.attach(18);  
  myServo.write(angle);  
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  if (angle == 0) {
    angle = 90;
  } else {
    angle = 0;
  }

  myServo.write(angle);
  
  int currentState = (angle == 90) ? 1 : 0;
  client.publish(topic, String(currentState).c_str());
  Serial.print("Angle: ");
  Serial.print(angle);
  Serial.print(", Published: ");
  Serial.println(currentState);
  
  delay(2000);  
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Connecting to MQTT...");
    if (client.connect("ESP32Client")) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}