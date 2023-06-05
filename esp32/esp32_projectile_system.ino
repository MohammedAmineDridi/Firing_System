#include <WiFi.h>
#include <HTTPClient.h>
#include <Servo.h>
#include <math.h>

#define WIFI_SSID ""
#define WIFI_PASSWORD ""

#define GREEN_LED_PIN 5
#define LASER_SENSOR_DATA_PIN 9
#define ULTRASONIC_TRIG_Pin 6
#define ULTRASONIC_ECHO_Pin 7
#define SERVO1_DATA_SIGNAL_PIN 12 // servo 1 : control 'teta' angle of projectile motion .
#define SERVO2_DATA_SIGNAL_PIN 13 // servo 2 : control 'alpha' angle of manivelle rotation .
#define SERVO3_DATA_SIGNAL_PIN 11 // servo 3 : control the ball joint link 'liasion rotule' to control laser sensor .  
#define SERVO4_DATA_SIGNAL_PIN 10 // servo 4 : control the ball joint link 'liasion rotule' to control ultrasonic position .
#define SERVO5_DATA_SIGNAL_PIN 8 // servo 5 : control the on/off of the bullet shoot 

/* -------------------------------------------------- SENSORS -------------------------------------------------------- */
/* 1 : ultrasonic sensor */
String Get_ultrasonic_distance()
{
  digitalWrite(ULTRASONIC_TRIG_Pin,LOW);
  delay(2);
  digitalWrite(ULTRASONIC_TRIG_Pin,HIGH);
  delay(10);
  digitalWrite(ULTRASONIC_TRIG_Pin,LOW);
  delay(2);
  // get distance 
  int duration = pulseIn(ULTRASONIC_ECHO_Pin, HIGH);
  float distance = duration * 0.034 / 2;
  return distance
}
/* 2 : laser sensor */
char Get_Laser_Value()
{
  return digitalRead(LASER_SENSOR_DATA_PIN); // HIGH/LOW 
}
/* -------------------------------------------------- ACTUATORS -------------------------------------------------------- */
void Set_ServoMotor_Angle(Servo ServoMotor , char Angle)
{
  ServoMotor.write(Angle);
}
void SetLed_State(int Led_State){
  digitalWrite(GREEN_LED_PIN,Led_State);
}
/* -------------------------------------------------- NETWORK -------------------------------------------------------- */
String GET_HTTP_Request(String url,String params)
{
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(url+params); // Replace with your API URL
    int httpResponseCode = http.GET();
    if (httpResponseCode == HTTP_CODE_OK) {
      String payload = http.getString();
      Serial.println(payload);
      return payload;
    } else {
      Serial.print("HTTP Request failed with error code: ");
      Serial.println(httpResponseCode);
    }
    http.end();
  } 
}
/* ------------------------------------------------------- SETUP ------------------------------------------------------ */
Servo ServoMotor_1,ServoMotor_2,ServoMotor_3,ServoMotor_4,ServoMotor_5 = 0;
void setup() {
  Serial.begin(115200);
  // config green led 
  pinMode(GREEN_LED_PIN,OUTPUT);
  // config laser sensor 
  pinMode(LASER_SENSOR_DATA_PIN,INPUT_PULLUP); 
  // config ultrasonic sensor 
  pinMode(ULTRASONIC_TRIG_Pin,OUTPUT);
  pinMode(ULTRASONIC_ECHO_Pin,INPUT);
  // config 5 servos 
  ServoMotor_1.attach(SERVO1_DATA_SIGNAL_PIN);
  ServoMotor_2.attach(SERVO2_DATA_SIGNAL_PIN);
  ServoMotor_3.attach(SERVO3_DATA_SIGNAL_PIN);
  ServoMotor_4.attach(SERVO4_DATA_SIGNAL_PIN);
  ServoMotor_5.attach(SERVO5_DATA_SIGNAL_PIN)
  // config wifi params .
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD); // add WIFI_CHANNEL
  Serial.print("Connecting to WiFi ");
  Serial.print(WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  Serial.println("Connected");
}

/* ---------------------------------------------------- LOOP ------------------------------------------------------ */
void loop() {
  // init system 
  int angle_servo3 = 0;
  SetLed_State(GREEN_LED_PIN,LOW);
  //check if application start btn is clicked
  String Btn_State = GET_HTTP_Request("http://localhost:80/web_app_projectile/php/Btn_Start_Stop/get_btn_state/get_btn_state.php","");
  if (Btn_State == '1'){
    // 1 : if system started green led is on
    SetLed_State(GREEN_LED_PIN,HIGH);
    // wait laser sensor to detect the target displacement .
    while( Get_Laser_Value() == LOW ){
      if (angle_servo3 <= 180){
        Set_ServoMotor_Angle(ServoMotor_3,angle_servo3++);
        delay(100);
      } else {
        angle_servo3 = 0;
      }
    }
    // 2 : if quit while (laser detect object with angle servo3 = x deg ) => rotate the servo4 get distance coords comparing to our object : DX/DY .
    Set_ServoMotor_Angle(ServoMotor_4,angle_servo3);
    // 3 : then get the distance of the target => and then , determine : DX & DY .
    float ultrasonic_target_distance = Get_ultrasonic_distance();
    float DX = ultrasonic_target_distance*cos(angle_servo3);
    float DY = ultrasonic_target_distance*sin(Get_ultrasonic_distance);
    // 4 - send DX/DY to web server to do calcuation and get : angles(Teta & alpha) = fct(DX,DY) ...
    String params = "?DX="+String(DX)+"&DY="+String(DY);
    String get_teta_angle = (GET_HTTP_Request("http://localhost:80/web_app_projectile/php/Calculations/calculation.php",params))[0];
    String get_alpha_angle = (GET_HTTP_Request("http://localhost:80/web_app_projectile/php/Calculations/calculation.php",params))[1];
    // 5 - affect this angles to concerning servo's .
    Set_ServoMotor_Angle(ServoMotor_1,get_teta_angle);
    Set_ServoMotor_Angle(ServoMotor_2,get_alpha_angle);
    // 6 - shoot the bullet 
    Set_ServoMotor_Angle(ServoMotor_5,180);
    // 7 - force the shutdown of the system automatically after shooting .
    String shutdown_system = GET_HTTP_Request("http://localhost:80/web_app_projectile/php/Btn_Start_Stop/set_btn_state/set_btn_state.php","?state=0");
  }
}





