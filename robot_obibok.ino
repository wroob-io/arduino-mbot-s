#include <MeMCore.h>
#include <WroobImp.h>

MeDCMotor motor1(M1);
MeDCMotor motor2(M2);
MeUltrasonicSensor us0(PORT_3);
MeLEDMatrix ledMtx(PORT_1);
WroobImp wroob("MBSM");

int speed1 = 0;
int speed2 = 0;
unsigned char drawBuffer[16] = {0,112,136,142,137,113,1,1,1,1,113,137,142,136,112,0};
const char *motor1_str = "motor1";
const char *motor2_str = "motor2";
const char *motors_str = "motors";
const char *sensors_str = "sensors";
const char *values_str = "values";
const char *us0_str = "US0";
const char *cols_str = "cols";
const char *cmd_str = "cmd";
const char *ev_str = "ev";
const char *set_speed_str = "SetSpeed";
const char *get_speed_str = "GetSpeed";
const char *get_range_str = "GetRange";
const char *set_led_str = "SetLedDispBmp";

void my_callback(JsonObject &payload) {
  StaticJsonDocument<128> event;

  if (payload[cmd_str].isNull())
    return;
      
  if (strcmp(payload[cmd_str], set_speed_str) == 0) {  
    if (payload[values_str].containsKey(motor1_str)) {
      motor1.run(payload[values_str][motor1_str]);
      speed1 = payload[values_str][motor1_str];
    }
    if (payload[values_str].containsKey(motor2_str)) {
      motor2.run(payload[values_str][motor2_str]);
      speed2 = payload[values_str][motor2_str];
    }
  } else if (strcmp(payload[cmd_str], get_speed_str) == 0) {
    event[ev_str] = get_speed_str;
    for (int i = 0; i < payload[motors_str].size(); i++) {
      if (payload[motors_str].getElement(i) == motor1_str)
        event[values_str][motor1_str] = speed1;
      if (payload[motors_str].getElement(i) == motor2_str)
        event[values_str][motor2_str] = speed2;
    }
    wroob.sendMessage(event);
  } else if (strcmp(payload[cmd_str], get_range_str) == 0) {
    event[ev_str] = get_range_str;
    for (int i = 0; i < payload[sensors_str].size(); i++) {
      if (payload[sensors_str].getElement(i) == us0_str)
        event[values_str][us0_str] = us0.distanceCm();
    }
    wroob.sendMessage(event);
  } else if (strcmp(payload[cmd_str], set_led_str) == 0) {
    ledMtx.clearScreen();
    copyArray(payload[cols_str], drawBuffer, payload[cols_str].size());
    ledMtx.drawBitmap(0, 0, 16, drawBuffer);
  }
}

void setup() {
  // put your setup code here, to run once:
  wroob.begin(my_callback);
  ledMtx.setBrightness(3);
  ledMtx.drawBitmap(0, 0, 16, drawBuffer);
}

void loop() {
  // put your main code here, to run repeatedly:
  wroob.feed();
}
