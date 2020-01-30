#include <MeMCore.h>
#include <WroobImp.h>

MeDCMotor motor1(M1);
MeDCMotor motor2(M2);
MeUltrasonicSensor us0(PORT_3);
MeLineFollower lineFinder(PORT_2);
MeLEDMatrix ledMtx(PORT_1);
MeBuzzer buzzer;
MeRGBLed rgbLed(7, 2);
MeLightSensor lightSensor(6);

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
const char *get_line_snr_str = "GetLineSensor";
const char *play_tone_str = "PlayTone";
const char *set_color_str = "SetColor";
const char *ls0_str = "LN0";
const char *ls1_str = "LN1";
const char *get_light_str = "GetLightSensor";
const char *li0_str = "LI0";
const char *note_freq_str = "frequency";
const char *note_dur_str = "duration";
const char *red_str = "red";
const char *green_str = "green";
const char *blue_str = "blue";

StaticJsonDocument<80> event;
void my_callback(JsonObject &payload) {
  event.clear();
  
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
    return;
  } 
  
  if (strcmp(payload[cmd_str], get_speed_str) == 0) {
    event[ev_str] = get_speed_str;
    for (int i = 0; i < payload[motors_str].size(); i++) {
      if (strcmp(payload[motors_str].getElement(i), motor1_str) == 0)
        event[values_str][motor1_str] = speed1;
      if (strcmp(payload[motors_str].getElement(i), motor2_str) == 0)
        event[values_str][motor2_str] = speed2;
    }
    wroob.sendMessage(event);
    return;
  }
  
  if (strcmp(payload[cmd_str], get_range_str) == 0) {
    event[ev_str] = get_range_str;
    for (int i = 0; i < payload[sensors_str].size(); i++) {
      if (strcmp(payload[sensors_str].getElement(i), us0_str) == 0)
        event[values_str][us0_str] = us0.distanceCm();
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
  
  if (strcmp(payload[cmd_str], get_line_snr_str) == 0) {
    event[ev_str] = get_line_snr_str;
    for (int i = 0; i < payload[sensors_str].size(); i++) {
      if (strcmp(payload[sensors_str].getElement(i), ls0_str) == 0)
        event[values_str][ls0_str] = lineFinder.readSensor1();
      if (strcmp(payload[sensors_str].getElement(i), ls1_str) == 0)
        event[values_str][ls1_str] = lineFinder.readSensor2();        
    }
    wroob.sendMessage(event);
    return;
  }

  if (strcmp(payload[cmd_str], play_tone_str) == 0) {
    if (payload.containsKey(note_freq_str) &&
        payload.containsKey(note_dur_str)) {
      buzzer.tone(payload[note_freq_str], payload[note_dur_str]);
    }
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
      if (strcmp(payload[sensors_str].getElement(i), li0_str) == 0)
        event[values_str][li0_str] = lightSensor.read();     
    }
    wroob.sendMessage(event);
    return;
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
