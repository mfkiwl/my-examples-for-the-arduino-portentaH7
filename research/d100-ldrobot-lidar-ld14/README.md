# D100 Lidar Kit


LDrobot youtube site is at   https://www.youtube.com/@ldrobot840/featured

JUST found the sensorteam at 


https://gitee.com/ldrobotSensorTeam



and the main LDRobot github at 

https://github.com/ldrobotSensorTeam


Arduino code here from LDrobot 

https://github.com/ldrobotsensor/Lidar_LD06_for_Arduino





Arduino forum here   https://forum.arduino.cc/t/error-compiling-for-board-arduino-mega-or-mega-2560/1119540


As far as I can tell the website has no downloads   https://www.ldrobot.com/download/en/124

Found this useful datasheet   https://github.com/ldrobotSensorTeam/DeveloperKit/blob/master/D100Kit.md


A manual that may be for it LD14   https://user-images.githubusercontent.com/66579525/176196048-d3281842-04d8-40bf-89e5-80dc58a70afd.png

And testing this old rplidar library  https://github.com/robopeak/rplidar_arduino which you need to unzip then zip the folder ```RPLidarDriver``` then install that.


### -------------------------------------------------------------------------

Drivers   https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers?tab=downloads

Point cloud software  https://github.com/ldrobotSensorTeam/ld_desktop_tool/releases


Manualv1-3   https://github.com/ldrobotSensorTeam/DeveloperKit/releases/download/v1.1/LDROBOT_LD14_Development_Manual_EN_v1.3.pdf

Manuial v1-2 https://github.com/ldrobotSensorTeam/DeveloperKit/releases/download/v1.0/LDROBOT_LD14_Development_Manual_EN_v1.2.pdf


Datasheet  https://github.com/ldrobotSensorTeam/DeveloperKit/releases/download/v1.0/LDROBOT_LD14_Datasheet_EN_v1.0.pdf




Found thi ssomewhere

```
#include "cloud.h"

//
// PointCyl
//
PointCart PointCyl::to_cart() const {
  int x = int(std::round(dist * std::sin(angle * (acos(-1.0f) / 180.0f))));
  int y = int(std::round(dist * std::cos(angle * (acos(-1.0f) / 180.0f))));
  return {x, y};
}

PointCart PointCyl::to_cart(float scale, float origin_x, float origin_y) const {
  int x = int(std::round(dist * std::sin(angle * (acos(-1.0f) / 180.0f)) * scale)) + origin_x;
  int y = int(std::round(dist * std::cos(angle * (acos(-1.0f) / 180.0f)) * scale)) + origin_y;
  return {x, y};
}
```



here is an interesting take on the rplidar library but without any example code

https://github.com/DanielKalicki/RPLidar_Arduino




