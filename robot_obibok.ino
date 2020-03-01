#include <MeMCore.h>
#include <WroobImp.h>

MeDCMotor motor1(M1);
MeDCMotor motor2(M2);
MeUltrasonicSensor us1(PORT_3);
MeLineFollower lineFinder(PORT_2);
MeLEDMatrix ledMtx(PORT_1);
MeBuzzer buzzer;
MeRGBLed rgbLed(7, 2);
MeLightSensor lightSensor(6);

WroobImp wroob("mbsm");

int speed0 = 0;
unsigned char drawBuffer[16] = {0,112,136,142,137,113,1,1,1,1,113,137,142,136,112,0};
const char *motor1_str = "M1";
const char *motor2_str = "M2";
const char *sensors_str = "sensors";
const char *values_str = "values";
const char *us1_str = "US1";
const char *cols_str = "cols";
const char *cmd_str = "cmd";
const char *ev_str = "ev";
const char *set_speed_str = "SetSpeed";
const char *get_range_str = "GetRange";
const char *set_led_str = "SetLedDispBmp";
const char *set_ledlvl_str = "SetLedDspLvl";
const char *get_line_snr_str = "GetLine";
const char *play_tone_str = "PlayTone";
const char *set_color_str = "SetColor";
const char *get_light_str = "GetLight";
const char *button_str = "GetButtons";
const char *ln1_str = "LN1";
const char *ln2_str = "LN2";
const char *lt1_str = "LT1";
const char *bt1_str = "BT1";
const char *note_freq_str = "freq";
const char *note_dur_str = "dur";
const char *red_str = "r";
const char *green_str = "g";
const char *blue_str = "b";
const char *lvl_str = "level";
const char *stop_str = "Stop";

StaticJsonDocument<80> event;
void my_callback(JsonObject &payload) {
  event.clear();
  
  if (payload[cmd_str].isNull())
    return;
      
  if (strcmp(payload[cmd_str], set_speed_str) == 0) {  
    if (payload[values_str].containsKey(motor1_str)) {
      speed0 = payload[values_str][motor1_str];
      motor1.run(speed0 * (-1));
    }
    if (payload[values_str].containsKey(motor2_str)) {
      motor2.run(payload[values_str][motor2_str]);
    }
    return;
  } 
  
  if (strcmp(payload[cmd_str], get_range_str) == 0) {
    event[ev_str] = get_range_str;
    for (int i = 0; i < payload[sensors_str].size(); i++) {
      if (strcmp(payload[sensors_str].getElement(i), us1_str) == 0)
        event[values_str][us1_str] = us1.distanceCm();
    }
    wroob.sendMessage(event);
    return;
  }
  
  if (strcmp(payload[cmd_str], set_led_str) == 0) {
    ledMtx.clearScreen();
    copyArray(payload[cols_str], drawBuffer, payload[cols_str].size());
    ledMtx.drawBitmap(0, 0, 16, drawBuffer);

    return;
  }

  if (strcmp(payload[cmd_str], set_ledlvl_str) == 0) {
    if (payload.containsKey(lvl_str)) {
      ledMtx.setBrightness(payload[lvl_str]);
    }
    return;
  }
  
  if (strcmp(payload[cmd_str], get_line_snr_str) == 0) {
    event[ev_str] = get_line_snr_str;
    for (int i = 0; i < payload[sensors_str].size(); i++) {
      if (strcmp(payload[sensors_str].getElement(i), ln1_str) == 0)
        event[values_str][ln1_str] = lineFinder.readSensor1();
      if (strcmp(payload[sensors_str].getElement(i), ln2_str) == 0)
        event[values_str][ln2_str] = lineFinder.readSensor2();        
    }
    wroob.sendMessage(event);
    return;
  }

  if (strcmp(payload[cmd_str], play_tone_str) == 0) {
    event[ev_str] = play_tone_str;
    if (payload.containsKey(note_freq_str) &&
        payload.containsKey(note_dur_str)) {
      buzzer.tone(payload[note_freq_str], payload[note_dur_str]);
    }
    wroob.sendMessage(event);
    return;
  }

  if (strcmp(payload[cmd_str], set_color_str) == 0) {
    if (payload.containsKey(red_str) &&
        payload.containsKey(green_str) &&
        payload.containsKey(blue_str)) {
      rgbLed.setColor(0, payload[red_str], payload[green_str], payload[blue_str]);
      rgbLed.show();
    }
    return;
  }
  
  if (strcmp(payload[cmd_str], get_light_str) == 0) {
    event[ev_str] = get_light_str;
    for (int i = 0; i < payload[sensors_str].size(); i++) {
      if (strcmp(payload[sensors_str].getElement(i), lt1_str) == 0)
        event[values_str][lt1_str] = lightSensor.read();     
    }
    wroob.sendMessage(event);
    return;
  }

  if (strcmp(payload[cmd_str], button_str) == 0) {
    event[ev_str] = button_str;
    for (int i = 0; i < payload[sensors_str].size(); i++) {
      if (strcmp(payload[sensors_str].getElement(i), bt1_str) == 0)
        event[values_str][bt1_str] = (0 ^ (analogRead(A7) > 10 ? 0 : 1));     
    }
    wroob.sendMessage(event);
    return;
  }

  if (strcmp(payload[cmd_str], stop_str) == 0) {
    motor1.run(0);
    motor2.run(0);
    return;
  }
}

void setup() {
  // put your setup code here, to run once:
  wroob.begin(my_callback);
  ledMtx.setBrightness(3);
  ledMtx.drawBitmap(0, 0, 16, drawBuffer);
  rgbLed.reset(7);
  rgbLed.show();
}

bool pressed = false;
void loop() {
  // put your main code here, to run repeatedly:
  wroob.feed();
}
