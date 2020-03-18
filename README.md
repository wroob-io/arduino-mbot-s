#### Prerequisite (dependencies installation):

1. Download and install `Arduino IDE`. Please follow instructions from [here](https://www.arduino.cc/en/guide/linux).
2. Download and install `MakeBlock` Arduino library. Library source code and installation instructions can be found [here](https://github.com/Makeblock-official/Makeblock-Libraries).
3. Go to Arduino library directory (on Linux it is usually `~/Arduino/libraries`) and clone there `WroobImp` Arduino library:
```sh
$ git clone https://github.com/wroob-io/arduino-wroobimp.git
```
4. Install necessary Arduino libraries directly from `Arduino IDE`: "Tools -> LIbrary Manager -> Filter your search... -> Install":
 * `ArduinoJson`,
 * `TimerOne`,
 * `ArduinoUniqueID`.
5. Clone `arduino-mbot-s` project:
```sh
$ git clone https://github.com/wroob-io/arduino-mbot-s.git
```

#### How to build and flash device:

1. Open `arduino-mbot-s.ino` file with `Arduino IDE`,
2. Select correct Arduino board (mBot-S is compatible with `Arduino Uno` board): "Tools -> Board -> Arduino/Genuino Uno",
3. Click "verify" button ("check" sign),
4. Click "upload" button (arrow).
