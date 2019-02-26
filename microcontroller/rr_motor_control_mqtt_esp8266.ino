/*
 The sketch listens to certain MQTT messages and sends motor control commands to the Motor Shield 
 works with the ESP8266 board/library.
 (based on Basic MQTT example provided by PubSubClient library)

 It connects to the MQTT server then:
 - subscribes to the topic "rr", printing out any messages it receives. 
   NB: it assumes the received payloads are strings not binary
 - If it receives the message "f" it activates the motor and switches ON the ESP LED, else switches it off
 - other supported commands are "h", "r", and "test"

 It will reconnect to the server if the connection is lost using a blocking
 reconnect function. See the 'mqtt_reconnect_nonblocking' example for how to
 achieve the same result without blocking the main loop.
*/

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <PubSubClient.h>

ESP8266WiFiMulti wifiMulti;

// settings GPIO pins for LED & NodeMCU Motor Driver Shield
// const byte BUILTIN_LED = 2;
const byte MOTA_DIR = 0; // motor A direction
const byte  MOTA_PWM = 5; // motor A PWM (translates to power/voltage)
const byte MOTB_DIR = 2;
const byte MOTB_PWM = 4;
const int SPEED_PWM = 1023;

// Update these with values suitable for your network.

// const char* ssid = "SSID";
// const char* password = "PW";
const char* mqtt_server = "192.168.178.115";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;
char announce[100];
char ssid_name[50];

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();

  // connect to the "best" available network using WifiMulti lib
  WiFi.mode(WIFI_STA);
  // wifiMulti.addAP("SSID1", "PW1");
  // wifiMulti.addAP("SSID2", "PW2");
  wifiMulti.addAP("SSID3", "PW3");
  wifiMulti.addAP("st_oberholz_bureau_2.4", "followthewhiteraBBit");
  // wifiMulti.addAP("berlin.freifunk.net");

  Serial.println("Connecting Wifi...");
  // int i = 0;
  while (wifiMulti.run() != WL_CONNECTED) { 
    delay(1000);
    Serial.print('.');
  }
  
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("SSID: ");
  // Serial.println(WiFi.SSID());
  WiFi.SSID().toCharArray(ssid_name, 50);
  Serial.println(ssid_name);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

bool motor_forward() {
  // all motors forward
  Serial.println("Motor forward");
  digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on
  digitalWrite(MOTA_DIR, HIGH);
  analogWrite(MOTA_PWM, SPEED_PWM);
  // digitalWrite(MOTB_DIR, HIGH);
  // analogWrite(MOTB_PWM, SPEED_PWM);
  client.publish("log", "MCU received command MOTOR FORWARD");
  return true;
}

bool motor_reverse() {
  // reverses all motors
  Serial.println("Motor reverse");
  digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off
  digitalWrite(MOTA_DIR, LOW);
  analogWrite(MOTA_PWM, SPEED_PWM);
  // digitalWrite(MOTB_DIR, LOW);
  // analogWrite(MOTB_PWM, SPEED_PWM);
  client.publish("log", "MCU received command MOTOR REVERSE");
  return true;
}

bool motor_halt() {
  // stops all motors
  Serial.println("Motor halt");
  digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off
  analogWrite(MOTA_PWM, 0);
  analogWrite(MOTB_PWM, 0);
  client.publish("log", "MCU received command MOTOR HALT");
  return true;
}

bool motor_test() {
  // test motor A speed
  Serial.println("Motor test");
  client.publish("log", "MCU received command MOTOR TEST");

  digitalWrite(MOTA_DIR, HIGH);
  for (int i=127; i<1024; i+=128) {
    Serial.print("PWM=");
    Serial.println(i);
    digitalWrite(BUILTIN_LED, LOW);   // LED on for 200 ms
    analogWrite(MOTA_PWM, i);
    delay(200);
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off
    delay(1800);
  }
  // motor stop
  analogWrite(MOTA_PWM, 0);
  return true;
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // control motor according to received first character
  switch ((char)payload[0]) {
    case 'f':
      motor_forward();
      // digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on
      break;
    case 'r':
      motor_reverse();
      break;
    case 'h':
      motor_halt();
      break;
    case 't':
      motor_test();
      break;
    default:
      Serial.println('unknown command'); 
      // digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off
      break;
  }

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "RailRover-MCU-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      sprintf(announce, "Motor Control Unit online, connected to SSID %s", ssid_name);
      client.publish("rr", announce);
      // ... and resubscribe
      client.subscribe("rr");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  pinMode(MOTA_DIR, OUTPUT);
  pinMode(MOTA_PWM, OUTPUT);
  // pinMode(MOTB_DIR, OUTPUT);
  // pinMode(MOTB_PWM, OUTPUT);

  digitalWrite(MOTA_DIR,HIGH);
  analogWrite(MOTA_PWM,0);
  // digitalWrite(MOTB_DIR,HIGH);
  // analogWrite(MOTB_PWM,0);
  
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  /*
  long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    ++value;
    sprintf (msg, 50, "hello world #%ld", value);
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish("rr", msg);
  }
  */
}
