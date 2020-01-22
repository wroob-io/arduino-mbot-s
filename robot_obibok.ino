#include <MeMCore.h>
#include <WroobImp.h>

#define MOTORS F("motors")
#define SENSORS F("sensors")
#define MOTOR1 F("motor1")
#define MOTOR2 F("motor2")
#define US0 F("US0")
#define VALUES F("values")
#define EV F("ev")
#define CMD F("cmd")
#define SET_SPEED   F("SetSpeed")
#define GET_SPEED   F("GetSpeed")
#define GET_RANGE   F("GetRange")
#define SET_CONTENT F("SetLedDispBmp")
#define COLUMNS     F("cols")

MeDCMotor motor1(M1);
MeDCMotor motor2(M2);
MeUltrasonicSensor us0(PORT_3);
MeLEDMatrix ledMtx(PORT_1);
WroobImp wroob("MBSM");

int speed1 = 0;
int speed2 = 0;
unsigned char drawBuffer[16] = {0,112,136,142,137,113,1,1,1,1,113,137,142,136,112,0};

void my_callback(JsonObject &payload) {
  StaticJsonDocument<128> event;

  if (payload[CMD] == SET_SPEED) {
    if (payload[VALUES].containsKey(MOTOR1)) {
      motor1.run(payload[VALUES][MOTOR1]);
      speed1 = payload[VALUES][MOTOR1];
    }
    if (payload[VALUES].containsKey(MOTOR2)) {
      motor2.run(payload[VALUES][MOTOR2]);
      speed2 = payload[VALUES][MOTOR2];
    }
  } else if (payload[CMD] == GET_SPEED) {
    event[EV] = GET_SPEED;
    for (int i = 0; i < payload[MOTORS].size(); i++) {
      if (payload[MOTORS].getElement(i) == MOTOR1)
        event[VALUES][MOTOR1] = speed1;
      if (payload[MOTORS].getElement(i) == MOTOR2)
        event[VALUES][MOTOR2] = speed2;
    }
    wroob.sendMessage(event);
  } else if (payload[CMD] == GET_RANGE) {
    event[EV] = GET_RANGE;
    for (int i = 0; i < payload[SENSORS].size(); i++) {
      if (payload[SENSORS].getElement(i) == US0)
        event[VALUES][US0] = us0.distanceCm();
    }
    wroob.sendMessage(event);
  } else if (payload[CMD] == SET_CONTENT) {
    ledMtx.clearScreen();
    copyArray(payload[COLUMNS], drawBuffer, payload[COLUMNS].size());
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
