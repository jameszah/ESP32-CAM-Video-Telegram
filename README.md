# ESP32-CAM-Video-Telegram
Record avi video on ESP32-CAM and send to Telegram on event or request


<h2> Compile Time Parameters </h2>

```
char ssid[] = "yourssid";         
char password[] = "yourssidpass"; 
String TIMEZONE = "GMT0BST,M3.5.0/01,M10.5.0/02";  
String chat_id = "1234567890";
#define BOTtoken "XXXXXXXXX:XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX" 

// see here for information about getting free telegram credentials
// https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot
// https://randomnerdtutorials.com/telegram-esp32-motion-detection-arduino/
// find your timezone here 
// https://sites.google.com/a/usapiens.com/opnode/time-zones  
```

As seen on Telegram screen: <br>

<img src="./magpie.jpg">

The avi as seen on Youtube - the picture here is the first frame of the avi, which is one frame after the jpeg sent to Telegram.

[![Click to see on Youtube](http://img.youtube.com/vi/x4JaLOefIWA/hqdefault.jpg)](http://www.youtube.com/watch?v=x4JaLOefIWA "Magpie from Youtube")

The file as served by Telegram:<br>

https://github.com/jameszah/ESP32-CAM-Video-Recorder-samples/blob/master/ESP32-Cam-Telegram/DoorCam%202021-06-03%2014.47.52.avi
  
The file uploaded to Youtube - now converted to Youtube's formats:  
https://youtu.be/x4JaLOefIWA

Demonstrating the slow motion.<br>

[![Click to see on Youtube](http://img.youtube.com/vi/KBXaL-kRRFI/hqdefault.jpg)](http://www.youtube.com/watch?v=KBXaL-kRRFI "Robin from Youtube")
