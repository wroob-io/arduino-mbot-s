#include <WroobImp.h>
#include <MeOrion.h>

#define MOTORS F("motors")
#define SENSORS F("sensors")
#define MOTOR1 F("motor1")
#define MOTOR2 F("motor2")
#define US0 F("US0")
#define VALUES F("values")
#define VALUE F("value")
#define EV F("ev")
#define CMD F("cmd")
#define SET_SPEED F("SetSpeed")
#define GET_SPEED F("GetSpeed")
#define GET_RANGE F("GetRange")

MeDCMotor motor1(M1);
MeDCMotor motor2(M2);
MeUltrasonicSensor us0(PORT_6);
WroobImp wroob;

int speed1 = 0;
int speed2 = 0;

void my_callback(JsonObject &paylaod) {
  StaticJsonDocument<128> event;

  if (paylaod[CMD] == SET_SPEED) {
    if (paylaod[VALUES].containsKey(MOTOR1)) {
      motor1.run(paylaod[VALUES][MOTOR1]);
      speed1 = paylaod[VALUES][MOTOR1];
    }
    if (paylaod[VALUES].containsKey(MOTOR2)) {
      motor2.run(paylaod[VALUES][MOTOR2]);
      speed2 = paylaod[VALUES][MOTOR2];
    }
  }
  if (paylaod[CMD] == GET_SPEED) {
    event[EV] = GET_SPEED;
    for (int i = 0; i < paylaod[MOTORS].size(); i++) {
      if (paylaod[MOTORS].getElement(i) == "motor1")
        event[VALUES][MOTOR1] = speed1;
      if (paylaod[MOTORS].getElement(i) == "motor2")
        event[VALUES][MOTOR2] = speed2;
    }
    wroob.sendMessage(event);
  }
  if (paylaod[CMD] == GET_RANGE) {
    event[EV] = GET_RANGE;
    for (int i = 0; i < paylaod[SENSORS].size(); i++) {
      if (paylaod[SENSORS].getElement(i) == US0)
        event[VALUES][US0] = us0.distanceCm();
    }
    wroob.sendMessage(event);
  }
}

void setup() {
  // put your setup code here, to run once:
  wroob.begin(my_callback);
}

void loop() {
  // put your main code here, to run repeatedly:
  wroob.feed();
}
