# LalaTouch

## ESP8266

Our figurines are based on the ESP8266 WiFi module, thus this is the repo
hosting its code.

So far, you need the following to run this code :

- [Arduino "IDE"](https://www.arduino.cc/en/Main/Software)
- [ESP8266 Core for Arduino](https://github.com/esp8266/Arduino)
- [WifiManager](https://github.com/tzapu/WiFiManager) library
- [MPU9250 and quaternionFilters] (https://github.com/sparkfun/SparkFun_MPU-9250_Breakout_Arduino_Library) (Our corresponding code is not written yet)

Then, it is a matter of compiling it and burning it to the ESP. Note that it
implements OTA updates, thus it needs to be burned serially only once.

## License

Copyright (c) 2017 The LalaTouch Team

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

