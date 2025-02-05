/*******************************************************************

  ESP32-CAM-Video-Telegram

  This program records an mjpeg avi video in the psram of a ESP32-CAM, and sends a jpeg and a avi video to Telegram.

  https://github.com/jameszah/ESP32-CAM-Video-Telegram is licensed under the
    GNU General Public License v3.0


   Version 11 - Major Update/Rewrite April 2024

     -- replace wifimanager with homemade version
     -- softap always running, initally called "deskpir" at 192.168.4.1
     -- you can "hide" your softap, so neighbors will not see it
     -- goto that site to setup 3 wifi routers, and change name/pass of softap
     -- best router signal will be the one used
     -- you can access this setup webpage from softap, or regular router ip
     -- use the endpoint 192.168.4.1/capture or 192.168.1.99/capture to see an image
        (could be used for motioneye etc)
     -- use the endpoint 192.168.4.1/clip or 192.168.1.99/clip to record video

     -- if you plug in a sd card, all photos video will be saved to the sd in a folder for this date
     -- these photos files can be downloaded to phone or computer through softap or regular ip
     -- if you have no router or internet, then photos will still be on sd card, but not to telegram
     -- so it will work with no internet, or no sd card

     -- you can download the photo video direct to you pc without the time-consuming send to telegram
     -- you can also look inside the avi to see if the clip is good before downloading it to computer
     -- you can also send old videos to telegram that were saved on sd card (if you are not near your esp32)
     -- you can configure it to always send to telegram even if you have sd card
     -- sd card will save a 3MB video in a second, leaving camera ready for another "event" recording,
        rather then the 2-3 minute send to telegram delay
     -- if you are not sending avi's to telegram, you will still get the first image, and
        links to download video to computer, or send video to telegram

     -- also adds "radu mode" used in very dark settings when exposure control inside the camera cannot
        handle to the dark, so exposures up to 4 seconds can be set to capture the glimpse of light
     -- this is not yet automatic, but telegram will tell you to turn it on

  by James Zahary  Apr 17, 2024
  jamzah.plc@gmail.com

  based on:

  https://github.com/jameszah/ESP32-CAM-JAMCAM


  //////////////////////////////////////////

  vvvv  old history and origins of ESP32-CAM-Video-Telegram below   vvvv
  ----------------------------------------------------------------------

  by James Zahary  June 1, 2020
  jamzah.plc@gmail.com


  The is Arduino code, with standard setup for ESP32-CAM
    - Board ESP32 Wrover Module
    - Partition Scheme Huge APP (3MB No OTA)
    - or with AI Thinker ESP32-CAM

  Oct 13, 2022 ver 9.5
  - mods to measure psram size, and fit movie into it, so 2mb and 8 mb boards are accomodated
  - you still need to modify max_frames to fully use larger memory
  - add wifimanager to set up ssid/pass, and telegram id and bot id
  - store parameters in in eprom, so it reboots into the same place
  - add timed photo feature to send a photo every 1 minute (up to 1440) to telegram
  - see the github site for photos of configuration and use
  - add one-click installer to install fresh over the web, and then use wifimanager to set parameters
    such as ssid/pass telegram id/bot, and then use the telegram interface to set more parameters such as
    video length, timed photo frequency

  Oct 21, 2022 ver 9.5.1
  - increase wifi timeout from 60s to 5 minutes for router outage
  - add ap mode timeout for 5 minutes, so it will reset and try to start wifi again
  - reset photo timer on /entim

  Oct 22, 2022 ver 9.6
  - add wifimanager option to select HD or VGA

    Sketch uses 1097929 bytes (34%) of program storage space. Maximum is 3145728 bytes.
  Global variables use 65116 bytes (19%) of dynamic memory, leaving 262564 bytes for local variables. Maximum is 327680 bytes.
  C:\ArduinoPortable\arduino-1.8.19\portable\packages\esp32\tools\esptool_py\3.3.0/esptool.exe --chip esp32 --port COM7 --baud 460800 --before default_reset --after hard_reset write_flash -z --flash_mode dio --flash_freq 80m --flash_size 4MB 0x1000 C:\Users\James\AppData\Local\Temp\arduino_build_232834/ESP32-CAM-Video-Telegram_9.6.ino.bootloader.bin 0x8000 C:\Users\James\AppData\Local\Temp\arduino_build_232834/ESP32-CAM-Video-Telegram_9.6.ino.partitions.bin 0xe000 C:\ArduinoPortable\arduino-1.8.19\portable\packages\esp32\hardware\esp32\2.0.4/tools/partitions/boot_app0.bin 0x10000 C:\Users\James\AppData\Local\Temp\arduino_build_232834/ESP32-CAM-Video-Telegram_9.6.ino.bin

  Jan 18, 2022 ver 8.9
  - updates from Arduino 1.8.19
    - return from void problem re-runs the function if you dont do a return ???
      https://stackoverflow.com/questions/22742581/warning-control-reaches-end-of-non-void-function-wreturn-type
  - updates for esp32-arduino 2.0.2
    - bug with 2.0.2 handshake timeout - added timeout resets in this file as a workaround
      https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot/issues/270#issuecomment-1003795884
   - updates for esp32-arduino 2.0.2
     - esp-camera seems to have changed to fill all free fb buffers in sequence, so must empty them to get a snapshot

  Based on these two:

  https://github.com/jameszah/ESP32-CAM-Video-Recorder-junior
  https://github.com/jameszah/ESP32-CAM-Video-Recorder

  and using a modified old version of:
  https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot



  ~~~~~~~~~~~~



*******************************************************************/


/*******************************************************************
  -  original opening from Brian Lough telegram bot demo

   A Telegram bot for taking a photo with an ESP32Cam

   Parts used:
   ESP32-CAM module* - http://s.click.aliexpress.com/e/bnXR1eYs

    = Affiliate Links

   Note:
   - Make sure that you have either selected ESP32 Wrover Module,
           or another board which has PSRAM enabled
   - Choose "Huge App" partion scheme

   Some of the camera code comes from Rui Santos:
   https://randomnerdtutorials.com/esp32-cam-take-photo-save-microsd-card/

   Written by Brian Lough
    YouTube: https://www.youtube.com/brianlough
    Tindie: https://www.tindie.com/stores/brianlough/
    Twitter: https://twitter.com/witnessmenow

    Aug 7, 2020 - jz
    Mods to library and example to demonstrate
     - bugfix with missing println statement
     - method to send big and small jpegs
     - sending a caption with a pictire

    Mar 26, 2021 - jz
    Mods for esp32 version 1.05
    See line 250 of UniversalTelegramBot.cpp in the current github software
    https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot/issues/235#issue-842397567
    See https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot/blob/65e6f826cbab242366d69f00cebc25cdd1e81305/src/UniversalTelegramBot.cpp#L250


*******************************************************************/

// ----------------------------
// Standard Libraries - Already Installed if you have ESP32 set up
// ----------------------------

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <WiFiAP.h>
#include <WiFiMulti.h>
WiFiMulti jMulti;
#include <HTTPClient.h>
#include "esp_http_server.h"

#include <SD_MMC.h>

//#include <WiFiManager.h>
//WiFiManager wm;
#include "esp_camera.h"
httpd_handle_t index_httpd = NULL;
bool send_a_telegram_test = false;
bool do_initial_send = true;
bool we_have_sd_card = false;
bool bool_send_all_avi = false;
int radu_level = -1;
int new_radu = -1;
// ----------------------------
// Additional Libraries - each one of these will need to be installed.
// ----------------------------

//#include <UniversalTelegramBot.h>
#include "UniversalTelegramBot.h"  // use local library which is a modified copy of an old version
// Library for interacting with the Telegram API
// Search for "Telegegram" in the Library manager and install
// The universal Telegram library
// https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot

#include <ArduinoJson.h>
// Library used for parsing Json from the API responses
// Search for "Arduino Json" in the Arduino Library manager
// https://github.com/bblanchon/ArduinoJson

#include "ESPxWebFlMgr.h"
const word filemanagerport = 8080;
ESPxWebFlMgr filemgr(filemanagerport);
int delete_old_stuff_flag = 0;
#include <ArduinoOTA.h>

static const char vernum[] = "pir-cam 11.16.5";
String devstr = "deskpir";
int max_frames = 300;
framesize_t configframesize = FRAMESIZE_VGA;  // FRAMESIZE_ + QVGA|CIF|VGA|SVGA|XGA|SXGA|UXGA
int frame_interval = 0;                       // 0 = record at full speed, 100 = 100 ms delay between frames
float speed_up_factor = 0.5;                  // 1 = play at realtime, 0.5 = slow motion, 10 = speedup 10x
int framesize = FRAMESIZE_VGA;                //FRAMESIZE_HD;
int quality = 10;
//int qualityconfig = 5;


// Initialize Wifi connection to the router and Telegram BOT
/*
  char ssid[] = "yourssid";     // your network SSID (name)
  char password[] = "yourssidpassword"; // your network key
  // https://sites.google.com/a/usapiens.com/opnode/time-zones  -- find your timezone here
  String timezone = "GMT0BST,M3.5.0/01,M10.5.0/02";

  // you can enter your home chat_id, so the device can send you a reboot message, otherwise it responds to the chat_id talking to telegram

  String chat_id = "1234567890";
  #define BOTtoken "XXXXXXXXX:XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"  // your Bot Token (Get from Botfather)
*/

// https://sites.google.com/a/usapiens.com/opnode/time-zones  -- find your timezone here
String timezonestr = "MST7MDT,M3.2.0/2:00:00,M11.1.0/2:00:00";  //"GMT0BST,M3.5.0/01,M10.5.0/02";

// you can enter your home chat_id, so the device can send you a reboot message, otherwise it responds to the chat_id talking to telegram

//String chat_id = "1234567890";
String chat_id;
//#define BOTtoken "XXXXXXXXX:XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"  // your Bot Token (Get from Botfather)
String BOTtoken;

int MagicNumber = 14;

// see here for information about getting free telegram credentials
// https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot
// https://randomnerdtutorials.com/telegram-esp32-motion-detection-arduino/

bool reboot_request = false;
bool reset_request = false;
bool test_send = false;
bool test_send_avi = false;
bool clear_the_queue = true;

File avifile;
File idxfile;
File outfile;

struct oneframe {
  uint8_t* the_frame;
  int the_frame_length;
  int the_frame_number;
  int the_frame_total;
};

//////////////////////////////

// These routines are to look inside the avi files on the sd card

//
// Reads an uint32_t in Big Endian at current file position
//
int read_quartet(File fd) {

  uint8_t y[4];
  size_t i1_err = fd.read(y, 4);
  uint32_t value = y[0] | y[1] << 8 | y[2] << 16 | y[3] << 24;
  return value;
}

oneframe find_a_frame(char* avi_file_name, int frame_num) {
  oneframe x;
  avifile = SD_MMC.open(avi_file_name, "r");  // avifile = SD_MMC.open(avi_file_name, "w");
  if (!avifile) {
    Serial.printf("Could not open %s file\n", avi_file_name);
    x.the_frame = NULL;
    return x;

  } else {
    //Serial.printf("File open: %s\n", avi_file_name);
    avifile.seek(0x30, SeekSet);
    int frame_cnt = read_quartet(avifile);
    Serial.printf("Frames %d\n", frame_cnt);

    //int frame_num = 0.01 * frame_pct * frame_cnt;
    //Serial.printf("Frames pct %d, Frame num %d \n", frame_pct, frame_num);

    if (frame_cnt < frame_num) {
      Serial.printf("Only %d frames, less than %d frame_num -- start at 0\n", frame_cnt, frame_num);
      frame_num = 0;
    }

    avifile.seek(0xe8, SeekSet);
    long index_start = read_quartet(avifile);
    Serial.printf("Len of movi / index_start %d\n", index_start);

    //bool success = avifile.seek(  , SeekEnd);

    Serial.printf("Len of file %d\n", avifile.size());
    Serial.printf("Seek %d\n", index_start + 244 + frame_num * 16 + 8);

    if (avifile.size() < index_start + 244 + frame_num * 16 + 8, SeekSet) {
      Serial.printf("File too small / broken %s file\n", avi_file_name);
      x.the_frame = NULL;
      return x;
    }
    long the_index_location = index_start + 244 + frame_num * 16 + 8;
    bool success = avifile.seek(the_index_location, SeekSet);
    if (!success) {
      Serial.printf("File incomplete %s file\n", avi_file_name);
      x.the_frame = NULL;
      return x;
    }
    long index_frame_start = read_quartet(avifile);
    int index_frame_length = read_quartet(avifile);

    avifile.seek(index_frame_start + 236, SeekSet);
    int the_oodc = read_quartet(avifile);
    if (the_oodc == 1667510320 || the_oodc == 1073470332 ) {
    } else {
      Serial.printf("No frame %s file, num %d\n", avi_file_name, frame_num);
      Serial.printf("the_oodc is %d,%X\n", the_oodc, the_oodc);
      x.the_frame = NULL;
      return x;
    }
    int the_len = read_quartet(avifile);
    Serial.printf("frame len %d \n", the_len);

    //Serial.printf("Your frame is %d bytes, at address %d or %X\n", index_frame_length, index_frame_start, index_frame_start);
    uint8_t* framebuffer;
    //framebuffer = (uint8_t*)ps_malloc(48 * 1024);
    framebuffer = (uint8_t*)ps_malloc(96 * 1024);

    avifile.seek(index_frame_start + 244, SeekSet);

    size_t err = avifile.read(framebuffer, index_frame_length);

    x.the_frame = framebuffer;
    x.the_frame_length = index_frame_length;
    x.the_frame_number = frame_num;
    x.the_frame_total = frame_cnt;
    return x;

  }  // else yes to no avi file
}

///////////////////////////////////////

// camera stuff

#define CAMERA_MODEL_AI_THINKER
#define PWDN_GPIO_NUM 32
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM 0
#define SIOD_GPIO_NUM 26
#define SIOC_GPIO_NUM 27

#define Y9_GPIO_NUM 35
#define Y8_GPIO_NUM 34
#define Y7_GPIO_NUM 39
#define Y6_GPIO_NUM 36
#define Y5_GPIO_NUM 21
#define Y4_GPIO_NUM 19
#define Y3_GPIO_NUM 18
#define Y2_GPIO_NUM 5
#define VSYNC_GPIO_NUM 25
#define HREF_GPIO_NUM 23
#define PCLK_GPIO_NUM 22

#include "esp_system.h"

bool setupCamera() {
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  config.grab_mode = CAMERA_GRAB_LATEST;  // CAMERA_GRAB_WHEN_EMPTY;
  config.fb_location = CAMERA_FB_IN_PSRAM;
  //init with high specs to pre-allocate larger buffers
  if (psramFound()) {
    config.frame_size = (framesize_t)framesize; // configframesize;
    config.jpeg_quality = quality; // 10;  //qualityconfig;
    config.fb_count = 3;
  } else {
    Serial.printf("\n\nNO PSRAM !!!!!!!!!!!!!\n\n");
    //scroll ("Dude -- no pasram"); -- screen not started yet
    while (1) {
      delay(1000);
    }
  }


  esp_err_t cam_err = ESP_FAIL;
  int attempt = 5;
  while (attempt && cam_err != ESP_OK) {
    cam_err = esp_camera_init(&config);
    if (cam_err != ESP_OK) {
      Serial.printf("Camera init failed with error 0x%x", cam_err);
      digitalWrite(PWDN_GPIO_NUM, 1);
      delay(800);
      digitalWrite(PWDN_GPIO_NUM, 0);  // power cycle the camera (OV2640)
      delay(200);
      attempt--;
    }
  }
  if (cam_err != ESP_OK) {
    Serial.printf("Camera init failed - trying a reboot\n");
    delay(10000);
    ESP.restart();
  } else {
    Serial.printf("Camera init succeeded!\n");
  }


  sensor_t* s = esp_camera_sensor_get();

  // s->set_aec_value(s, 1200); // ???

  delay(200);
  return true;
}

///////////////////////////////////

// routines for the timed exposures in very dark conditions

// https://github.com/raduprv/esp32-cam_ov2640-timelapse

void set_radu(int r_level) {
  camera_fb_t * fb = NULL;
  sensor_t * s = esp_camera_sensor_get();

  radu_level = r_level;

  Serial.printf("set radu to %d\n", radu_level);

  int y = millis();
  fb = esp_camera_fb_get();
  esp_camera_fb_return(fb);
  Serial.printf("before radu Get took %d ms, fail %d\n", millis() - y, !fb);

  //s->set_exposure_ctrl(s, 0);  // 0 = disable , 1 = enable -- almost implements 2 manual lines below
  s->set_reg(s, 0xff, 0xff, 0x01); //banksel

  s->set_reg(s, 0x13, 0xff, 0); //manual everything
  s->set_reg(s, 0x0c, 0x6, 0x8); //manual banding

  if (radu_level < 5) {
    s->set_reg(s, 0x47, 0xff, 0x40); //Frame Length Adjustment MSBs
    //  s->set_reg(s, 0x2a, 0xf0, 0xff); //line adjust MSB
    //  https://github.com/espressif/esp32-camera/blob/bae46be5eb126c46c692a29a953991117f68336b/driver/esp_camera.c#L359
    //  longest timed exposure shorter than esp32-camera 4 second timeout
    s->set_reg(s, 0x2a, 0xc0, 0xff); //line adjust MSB
    s->set_reg(s, 0x2b, 0xff, 0xff); //line adjust

  } else if (radu_level < 10) {
    s->set_reg(s, 0x47, 0xff, 0x20); //Frame Length Adjustment MSBs
    s->set_reg(s, 0x2a, 0xf0, 0x80); //line adjust MSB
    s->set_reg(s, 0x2b, 0xff, 0xff); //line adjust

  } else if (radu_level < 18) {
    s->set_reg(s, 0x47, 0xff, 0x10); //Frame Length Adjustment MSBs
    s->set_reg(s, 0x2a, 0xf0, 0x70); //line adjust MSB
    s->set_reg(s, 0x2b, 0xff, 0xff); //line adjust

  } else if (radu_level < 24) {
    s->set_reg(s, 0x47, 0xff, 0x08); //Frame Length Adjustment MSBs
    s->set_reg(s, 0x2a, 0xf0, 0xb0); //line adjust MSB
    s->set_reg(s, 0x2b, 0xff, 0xff); //line adjust

  } else if (radu_level < 35) {
    s->set_reg(s, 0x47, 0xff, 0x04); //Frame Length Adjustment MSBs
    s->set_reg(s, 0x2a, 0xf0, 0xd0); //line adjust MSB
    s->set_reg(s, 0x2b, 0xff, 0xff); //line adjust

  } else if (radu_level < 50) {
    s->set_reg(s, 0x47, 0xff, 0x02); //Frame Length Adjustment MSBs
    s->set_reg(s, 0x2a, 0xf0, 0x70); //line adjust MSB
    s->set_reg(s, 0x2b, 0xff, 0xff); //line adjust
  }

  s->set_reg(s, 0x43, 0xff, 0x40); //magic value to give us the frame faster (bit 6 must be 1)
  s->set_reg(s, 0xff, 0xff, 0x00); //banksel
  s->set_reg(s, 0xd3, 0xff, 0x8); //clock
  s->set_reg(s, 0x42, 0xff, 0x2f); //image quality (lower is bad)

  //no sharpening
  s->set_reg(s, 0x92, 0xff, 0x1);
  s->set_reg(s, 0x93, 0xff, 0x0);

  s->set_reg(s, 0xff, 0xff, 0x01); //banksel
  Serial.println("After the radu");

  int light_bef;
  int light_aft;
  for (int i = 0; i < 5; i++) {
    light_bef = s->get_reg(s, 0x2f, 0xff);

    // delay(300); //Serial.println("delay 75");

    y = millis();
    fb = esp_camera_fb_get();
    esp_camera_fb_return(fb);

    light_aft = s->get_reg(s, 0x2f, 0xff);

    Serial.printf("after radu, pic #%2d, bef light %4d, aft light %4d Get %5d ms, fail % d\n", i, light_bef, light_aft, millis() - y, !fb);
    if (!fb) {
      Serial.printf("Frame failed, so wait another 1 second\n");
      delay (1000);
    }
  }

}

void re_init() {
  esp_err_t err = esp_camera_deinit();
  if (err != ESP_OK) {
    Serial.printf("Camera de-init failed with error 0x%x", err);
    //return false;
  }
  delay(100);
  radu_level = -1;
  Serial.println("Try to re-init");
  bool x  = setupCamera();
}

/////////////////////////////////

// back to telegram bot code

#define FLASH_LED_PIN 4

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

int Bot_mtbs = 5000;      //mean time between scan messages        -- 5000 -> 15000 ver 10 // ver 11 back to 5000
long Bot_lasttime;        //last time messages' scan has been done
long TimePhoto_lasttime;  // last time we sent a timed photo
long RaduPhoto_lasttime;  // last time we sent a timed photo

int TimePhoto_Minutes;    // 2 minutes between photos

bool flashState = LOW;

camera_fb_t* fb = NULL;
camera_fb_t* vid_fb = NULL;

TaskHandle_t the_camera_loop_task;
void the_camera_loop(void* pvParameter);
static void IRAM_ATTR PIR_ISR(void* arg);

bool video_ready = false;
bool picture_ready = false;
bool active_interupt = false;
bool pir_enabled = false;
bool avi_enabled = false;
bool tim_enabled = false;
bool hdcam = true;

int avi_buf_size = 0;
int idx_buf_size = 0;

bool isMoreDataAvailable();


////////////////////////////////  send photo as 512 byte blocks or jzblocksize
int currentByte;
uint8_t* fb_buffer;
size_t fb_length;

bool isMoreDataAvailable() {
  return (fb_length - currentByte);
}

uint8_t getNextByte() {
  currentByte++;
  return (fb_buffer[currentByte - 1]);
}

////////////////////////////////  send avi as 512 byte blocks or jzblocksize
int avi_ptr;
uint8_t* avi_buf;
size_t avi_len;

bool avi_more() {
  return (avi_len - avi_ptr);
}

uint8_t avi_next() {
  avi_ptr++;
  return (avi_buf[avi_ptr - 1]);
}

bool dataAvailable = false;


///////////////////////////////

// main event loop for taking to telegram

void handleNewMessages(int numNewMessages) {
  //Serial.println("handleNewMessages");
  //Serial.println(String(numNewMessages));

  for (int i = 0; i < numNewMessages; i++) {

    // This line gets id of the person who sent the message .. it could be any telegram user
    // remove this line, and all messages get send to you

    // chat_id = String(bot.messages[i].chat_id);

    String text = bot.messages[i].text;
    Serial.printf("\nGot a message: ");
    Serial.println(text);

    String from_name = bot.messages[i].from_name;
    if (from_name == "") from_name = "Guest";

    String hi = "Got: ";
    hi += text;
    bot.sendMessage(chat_id, hi, "MarkdownV2");

    if (text == "/flash") {
      flashState = !flashState;
      digitalWrite(FLASH_LED_PIN, flashState);
    }

    if (text.substring(1).toInt() >= 1 && text.substring(1).toInt() <= 1440) {
      TimePhoto_Minutes = text.substring(1).toInt();
      TimePhoto_lasttime = millis();
      do_eprom_write();
    }
    if (text == "/status") {
      String stat = "Device: " + devstr + "\nVer: " + String(vernum) + "\nRssi: " + String(WiFi.RSSI()) + "\nip: " + WiFi.localIP().toString() + "\nPir Enabled: " + pir_enabled + "\nAvi Enabled: " + avi_enabled + "\nTim Enabled: " + tim_enabled;
      stat = stat + "\nInterval: " + frame_interval;
      stat = stat + "\nTimer: " + TimePhoto_Minutes;

      bot.sendMessage(chat_id, stat, "");
    }

    //Serial.println(text.substring(1,5));
    if (text.substring(1, 5) == "send") {
      String fname = text.substring(7);

      //send_the_file ( "/20240411/deskpir_2024-04-11_19.18.13.avi");
      char fnamechar[50];
      sprintf(fnamechar, "%s", fname.c_str());
      send_the_file ( fnamechar );
    }

    if (text.substring(0, 5) == "/rad1") {
      //Serial.println("Calling  radu");
      int level = text.substring(6).toInt();
      //Serial.println(level);
      if (level >= 0 && level <= 50) {
        Serial.printf("Calling radu with % d\n", level);
        set_radu(level);
      }
    }

    if (text.substring(0, 5) == "/radu") { // radu with a re_init
      int level;
      if (text.substring(0, 6) == "/radu:") { // radu with a re_init
        // with string(5) there is no colon before the number
        level = text.substring(6).toInt();
      } else {
        level = text.substring(5).toInt();
      }

      if (level > 0 && level <= 50) {
        Serial.printf("Calling radu with % d\n", level);
        re_init(); //bool x  = setupCamera();
        set_radu(level);
      }
    }


    if (text.substring(0, 5) == "/brdu") { // radu with re_init and black and white
      //Serial.println("Calling  radu");
      int level = text.substring(6).toInt();
      //Serial.println(level);
      if (level >= 0 && level <= 50) {
        Serial.printf("Calling radu with % d\n", level);

        re_init(); // bool x  = setupCamera();

        Serial.println("bw");
        camera_fb_t * fb = NULL;
        sensor_t * s = esp_camera_sensor_get();
        s->set_special_effect(s, 2); // 0 to 6 (0 - No Effect, 1 - Negative, 2 - Grayscale, 3 - Red Tint, 4 - Green Tint, 5 - Blue Tint, 6 - Sepia)
        fb = esp_camera_fb_get();
        esp_camera_fb_return(fb);
        fb = esp_camera_fb_get();
        esp_camera_fb_return(fb);
        fb = esp_camera_fb_get();
        esp_camera_fb_return(fb);
        Serial.println("done bw");

        set_radu(level);
      }
    }


    //
    if (text == "/bw") {  //black and white
      Serial.println("bw");
      camera_fb_t * fb = NULL;
      sensor_t * s = esp_camera_sensor_get();
      s->set_special_effect(s, 2); // 0 to 6 (0 - No Effect, 1 - Negative, 2 - Grayscale, 3 - Red Tint, 4 - Green Tint, 5 - Blue Tint, 6 - Sepia)
      fb = esp_camera_fb_get();
      esp_camera_fb_return(fb);
      fb = esp_camera_fb_get();
      esp_camera_fb_return(fb);
      fb = esp_camera_fb_get();
      esp_camera_fb_return(fb);
      Serial.println("done bw");
    }

    if (text == "/color") {
      Serial.println("color");
      camera_fb_t * fb = NULL;
      sensor_t * s = esp_camera_sensor_get();
      s->set_special_effect(s, 0); // 0 to 6 (0 - No Effect, 1 - Negative, 2 - Grayscale, 3 - Red Tint, 4 - Green Tint, 5 - Blue Tint, 6 - Sepia)
      fb = esp_camera_fb_get();
      esp_camera_fb_return(fb);
      fb = esp_camera_fb_get();
      esp_camera_fb_return(fb);
      fb = esp_camera_fb_get();
      esp_camera_fb_return(fb);
      Serial.println("done color");
    }


    if (text == "/regular" || text == "/reg") {
      Serial.println("Forcing regular");
      re_init();
      Serial.println("Finished regular");
    }

    if (text == "/reboot") {
      reboot_request = true;
    }

    //if (text == "/reset") {
    //  reset_request = true;
    //}

    if (text == "/enpir") {
      pir_enabled = true;
      do_eprom_write();
    }

    if (text == "/dispir") {
      pir_enabled = false;
      do_eprom_write();
    }

    if (text == "/enavi") {
      avi_enabled = true;
      do_eprom_write();
    }

    if (text == "/disavi") {
      avi_enabled = false;
      do_eprom_write();
    }

    if (text == "/entim") {
      tim_enabled = true;
      TimePhoto_lasttime = millis();
      do_eprom_write();
    }

    if (text == "/distim") {
      tim_enabled = false;
      do_eprom_write();
    }

    if (text == "/fast") {
      max_frames = 300;
      frame_interval = 0;
      speed_up_factor = 0.5;
      pir_enabled = true;
      avi_enabled = true;
      do_eprom_write();
    }

    if (text == "/med") {
      max_frames = 300;
      frame_interval = 125;
      speed_up_factor = 1;
      pir_enabled = true;
      avi_enabled = true;
      do_eprom_write();
    }

    if (text == "/slow") {
      max_frames = 300;
      frame_interval = 500;
      speed_up_factor = 5;
      pir_enabled = true;
      avi_enabled = true;
      do_eprom_write();
    }

    if (text == "/vslow") {
      max_frames = 300;
      frame_interval = 2000;
      speed_up_factor = 20;
      pir_enabled = true;
      avi_enabled = true;
      do_eprom_write();
    }

    if (text == "/vvslow") {
      max_frames = 300;
      frame_interval = 5000;
      speed_up_factor = 100;
      pir_enabled = true;
      avi_enabled = true;
      do_eprom_write();
    }

    if (text == "/photo" || text == "/caption") {

      fb = NULL;

      // Take Picture with Camera
      int y = millis();
      fb = esp_camera_fb_get();
      if (!fb) {
        Serial.println("Camera capture failed at /photo");
        bot.sendMessage(chat_id, "Camera capture failed at /photo", "");
        return;
      }
      int pic_time = millis() - y;

      currentByte = 0;
      fb_length = fb->len;
      fb_buffer = fb->buf;

      sensor_t * s2 = esp_camera_sensor_get();
      s2->set_reg(s2, 0xff, 0xff, 0x01); //banksel
      delay(1);
      int li2 = s2->get_reg(s2, 0x2f, 0xff);
      if (text == "/caption") {

        Serial.println("\n>>>>> Sending with a caption, bytes=  " + String(fb_length));

        String caption_text = "Caption Photo, mode: " + String(radu_level) + ", light: " + String(li2)  +
                              ", pic time: "  + String(pic_time) + ", len " + String(fb_length) +  "\n";

        String sent = bot.sendMultipartFormDataToTelegramWithCaption("sendPhoto", "photo", "img.jpg",
                      "image/jpeg", caption_text, chat_id, fb_length,
                      isMoreDataAvailable, getNextByte, nullptr, nullptr);

        Serial.println("done!");
        if (we_have_sd_card) save_jpg("caption", fb_buffer, fb_length);
      } else {

        Serial.println("\n>>>>> Sending, bytes=  " + String(fb_length));

        bot.sendPhotoByBinary(chat_id, "image/jpeg", fb_length,
                              isMoreDataAvailable, getNextByte,
                              nullptr, nullptr);
        if (we_have_sd_card) save_jpg("image", fb_buffer, fb_length);
        dataAvailable = true;

        Serial.println("done!");
      }
      esp_camera_fb_return(fb);
    }


    if (text == "/clip") {

      // record the video
      bot.longPoll = 0;
      Serial.printf("Telegram clip req\n");

      xTaskCreatePinnedToCore(the_camera_loop, "the_camera_loop", 10000, NULL, 1, &the_camera_loop_task, 1);

      if (the_camera_loop_task == NULL) {
        //vTaskDelete( xHandle );
        Serial.printf("do_the_steaming_task failed to start! %d\n", the_camera_loop_task);
      }
    }

    if (text == "/freecoffee") {
      String welcome = "ESP32Cam Telegram BOT\n\n";
      welcome += "https://ko-fi.com/jameszah/\n";
      welcome += "https://github.com/jameszah/ESP32-CAM-Video-Telegram\n";
      bot.sendMessage(chat_id, welcome, "Markdown");
    }

    if (text == "/start") {
      String welcome = "ESP32Cam Telegram BOT\n\n";
      welcome += "/photo: take a photo\n";
      welcome += "/flash: toggle flash LED\n";
      welcome += "/caption: photo with caption\n";
      welcome += "/clip: short video clip\n";
      welcome += "\n Configure the clip\n";
      welcome += "/enpir: enable pir\n";
      welcome += "/dispir: disable pir\n";
      welcome += "/enavi: enable avi\n";
      welcome += "/disavi: disable avi\n";
      welcome += "\n/entim: enable timed photo\n";
      welcome += "/distim: disable timed photo\n";
      welcome += "/10: 1..10..1440 minutes timed photos\n";
      welcome += "\n/fast: 25 fps - play .5x speed\n";
      welcome += "/med: 8  fps - play 1x speed\n";
      welcome += "/slow: 2  fps - play 5x speed\n";
      welcome += "/vslow: .5  fps - play 20x speed\n";
      welcome += "/vvslow: 5 sec/fr  - play 100x speed\n";
      welcome += "\n/status: status\n";
      welcome += "/radu:15 set radu mode 1..15..50\n" ;
      welcome += "/brdu:15 set radu mode 1..15..50 b&w \n" ;
      welcome += "/regular: force regular mode\n";
      welcome += "/bw: black and white\n";
      welcome += "/color: color\n";
      welcome += "/start: start\n\n";
      welcome += "/reboot: reboot\n";
      welcome += "\n/freecoffee\n";
      //welcome += "\n https://ko-fi.com/jameszah/\n";
      bot.sendMessage(chat_id, welcome, "");
    }
  }
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// setup some interupts during reboot
//
//  int read13 = digitalRead(13); -- pir for video

int PIRpin = 13;

static void pir_setup() {

  pinMode(PIRpin, INPUT_PULLDOWN);  //INPUT_PULLDOWN);
  delay(500);

  Serial.print("Setup PIRpin = ");
  for (int i = 0; i < 5; i++) {
    Serial.print(digitalRead(PIRpin));
    Serial.print(", ");
  }
  Serial.println(" ");

  //esp_err_t err = gpio_isr_handler_add((gpio_num_t)PIRpin, &PIR_ISR, NULL);

  esp_err_t err = gpio_isr_handler_add( GPIO_NUM_13, &PIR_ISR, NULL);
  if (err != ESP_OK) Serial.printf("gpio_isr_handler_add failed (%x)", err);

  gpio_set_intr_type( GPIO_NUM_13, GPIO_INTR_POSEDGE);
}
static void pir_enable() {
  gpio_intr_enable(GPIO_NUM_13 );
}

static void pir_disable() {
  gpio_intr_disable(GPIO_NUM_13 );
}

#include "avi.h"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//  PIR_ISR - interupt handler for PIR  - starts or extends a video
//
static void IRAM_ATTR PIR_ISR(void* arg) {

  int PIRstatus = digitalRead(PIRpin) + digitalRead(PIRpin) + digitalRead(PIRpin);
  if (PIRstatus == 3) {
    //Serial.print("PIR Interupt>> "); Serial.println(PIRstatus);

    if (!active_interupt && pir_enabled) {
      active_interupt = true;
      digitalWrite(33, HIGH);
      Serial.print("PIR Interupt ... start recording ... ");

      pir_disable();

      xTaskCreatePinnedToCore(the_camera_loop, "the_camera_loop", 10000, NULL, 1, &the_camera_loop_task, 1);


      if (the_camera_loop_task == NULL) {
        Serial.printf("do_the_steaming_task failed to start! %d\n", the_camera_loop_task);
      }
    }
  }
}

char ssidap[30];
char ssidapsecret[10];
char raduauto[10];
char send_all_avi[10];
char passap[30];
char ssid1[30];
char pass1[30];
char ssid2[30];
char pass2[30];
char ssid3[30];
char pass3[30];
char telegramid[15];
char telegrambot[60];
char frame[10];
char timezone[50];

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//  eprom functions
//

#include <EEPROM.h>

struct eprom_data {
  bool checkpir;
  bool checkavi;
  bool checktim;
  bool hdcam;  // hd=1,vga=0
  int max_frames;
  int frame_interval;
  float speed_up_factor;
  int timmin;
  char devname[16];
  char telegramid[16];
  char telegrambot[62];
  char timezone[52];
  char ssid1[32];
  char pass1[32];
  char ssid2[32];
  char pass2[32];
  char ssid3[32];
  char pass3[32];
  char ssidap[32];
  char passap[32];
  char frame[10];
  char ssidapsecret[10];
  char send_all_avi[10];
  char raduauto[10];
  int eprom_good;

};

void do_eprom_read() {

  eprom_data ed;

  EEPROM.begin(600);
  EEPROM.get(0, ed);

  if (ed.eprom_good == MagicNumber) {
    Serial.println("Good settings in the EPROM ");

    //devstr = ed.devname;
    //devstr.toCharArray(devname, 12); //devstr.length() + 1);
    //BOTtoken = ed.BOTtoken;
    //chat_id = ed.chat_id;
    //timezone = ed.timzon;
    hdcam = ed.hdcam;
    pir_enabled = ed.checkpir;
    avi_enabled = ed.checkavi;
    tim_enabled = ed.checktim;
    TimePhoto_Minutes = ed.timmin;
    max_frames = ed.max_frames;
    frame_interval = ed.frame_interval;
    speed_up_factor = ed.speed_up_factor;

    strcpy(timezone, ed.timezone);
    strcpy(devname, ed.devname);
    strcpy(ssidap, ed.ssidap);
    strcpy(ssidapsecret, ed.ssidapsecret);
    strcpy(raduauto, ed.raduauto);
    strcpy(send_all_avi, ed.send_all_avi);
    if (strcmp(send_all_avi, "true") == 0) {
      bool_send_all_avi = true;
      Serial.printf("send all avi %s\n", send_all_avi);
    } else {
      Serial.printf("send all avi %s\n", send_all_avi);
      bool_send_all_avi = false;
    }
    strcpy(passap, ed.passap);
    strcpy(ssid1, ed.ssid1);
    strcpy(ssid2, ed.ssid2);
    strcpy(ssid3, ed.ssid3);
    strcpy(pass1, ed.pass1);
    strcpy(pass2, ed.pass2);
    strcpy(pass3, ed.pass3);
    strcpy(telegramid, ed.telegramid);
    strcpy(telegrambot, ed.telegrambot);
    strcpy(frame, ed.frame);
    //Serial.printf("Eprom read %s, %s, %s\n", ssidap, ssid1, pass1);
    BOTtoken = telegrambot;  //BOTtoken.toCharArray(ed.BOTtoken, 50); //BOTtoken.length() + 1);
    chat_id = telegramid;    //chat_id.toCharArray(ed.chat_id, 12); //chat_id.length() + 1);
    devstr = devname;
    timezonestr = ed.timezone;
    bot.updateToken(BOTtoken);

  } else {
    Serial.println("No settings in EPROM ");

    chat_id = "1234567890";
    BOTtoken = "123456789:12345678901234567890123456789012345";
    timezonestr = "GMT";
    devstr = "deskpir";

    pir_enabled = 1;
    avi_enabled = 1;
    tim_enabled = 1;
    TimePhoto_Minutes = 30;
    max_frames = 300;
    frame_interval = 125;
    speed_up_factor = 1;
    hdcam = false;
    strcpy(ssidap, "deskpir");

    strcpy(passap, "12344321");
    strcpy(ssid1, "none");
    strcpy(ssid2, "none");
    strcpy(ssid3, "none");
    strcpy(ssidapsecret, "false");
    strcpy(send_all_avi, "false");
    strcpy(raduauto, "false");
    do_eprom_write();
  }
}

void do_eprom_write() {

  eprom_data ed;
  ed.eprom_good = MagicNumber;

  devstr.toCharArray(ed.devname, 12);  //devstr.length() + 1);
  //BOTtoken.toCharArray(ed.BOTtoken, 50); //BOTtoken.length() + 1);
  //chat_id.toCharArray(ed.chat_id, 12); //chat_id.length() + 1);
  //timezone.toCharArray(ed.timzon, 50); //timezone.length() + 1);

  ed.checkpir = pir_enabled;
  ed.checkavi = avi_enabled;
  ed.checktim = tim_enabled;
  ed.hdcam = hdcam;
  ed.timmin = TimePhoto_Minutes;
  ed.max_frames = max_frames;
  ed.frame_interval = frame_interval;
  ed.speed_up_factor = speed_up_factor;

  strcpy(ed.ssidap, ssidap); //Serial.printf("Eprom write %s, %s, %s\n", ssidap, ssid1, pass1);
  strcpy(ed.ssidapsecret, ssidapsecret);
  strcpy(ed.raduauto, raduauto);
  strcpy(ed.send_all_avi, send_all_avi);
  strcpy(ed.passap, passap);
  strcpy(ed.ssid1, ssid1);
  strcpy(ed.ssid2, ssid2);
  strcpy(ed.ssid3, ssid3);
  strcpy(ed.pass1, pass1);
  strcpy(ed.pass2, pass2);
  strcpy(ed.pass3, pass3);
  strcpy(ed.telegramid, telegramid);
  strcpy(ed.telegrambot, telegrambot);
  strcpy(ed.frame, frame);
  strcpy(ed.timezone, timezone);

  Serial.println("Writing to EPROM ...");

  Serial.printf("Size of eprom %d\n", sizeof(ed));

  EEPROM.begin(600);
  EEPROM.put(0, ed);
  EEPROM.commit();
  EEPROM.end();
}

#include "esp_wifi.h"
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#include <ESPmDNS.h>

/////////////////////////////////////////

// here is the homemade wifi manager

#include "http.h"

bool init_soft() {
  uint32_t brown_reg_temp = READ_PERI_REG(RTC_CNTL_BROWN_OUT_REG);
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);

  Serial.printf("Starting Soft AP ...\n");
  delay(50);

  Serial.printf("Name for soft ssid and hostname, and OTA: %s\n", ssidap);
  WiFi.setHostname(ssidap);  // must be before the mode apparantly
  WiFi.mode(WIFI_AP_STA);
  WiFi.disconnect();
  if  (strcmp(ssidapsecret, "false") == 0 ) {
    WiFi.softAP(ssidap, passap);
  } else {
    Serial.printf("Secret ssidap\n");
    WiFi.softAP(ssidap, passap, 1, 1, 3);  // channel 1, 1 = ssid hidden, 3 max connections
  }
  delay(50);
  Serial.printf("Get Soft AP ...\n");
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());

  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, brown_reg_temp);

  return true;
}

void init_sd() {

  bool save_pir = pir_enabled;
  pir_enabled = false;
  pir_disable();

  pinMode(4, INPUT);    // Blinding Disk-Avtive Light
  pinMode(13, INPUT);
  pinMode(12, INPUT);

  delay(500);

  int succ = SD_MMC.begin("/sdcard", true);
  if (succ) {
    we_have_sd_card = true;

    uint64_t cardSize = SD_MMC.cardSize() / (1024 * 1024);
    Serial.printf("SD_MMC Card Size: %lluMB\n", cardSize);


    filemgr.begin();
    Serial.printf("after fileman RAM  %7d / %7d, SPI %7d / %7d\n ", ESP.getFreeHeap(), ESP.getHeapSize(), ESP.getFreePsram(), ESP.getPsramSize());
    delete_old_stuff_flag == 1;

  } else {
    we_have_sd_card = false;
    Serial.printf("Failed to mount SD card VFAT filesystem. \n");
    Serial.println("Do you have an SD Card installed?");
    Serial.println("Check pin 12 and 13, not grounded, or grounded with 10k resistors!\n\n");
  }

  pir_enabled = save_pir;

  pinMode(13, INPUT_PULLDOWN);
  pinMode(4, OUTPUT);    // Blinding Disk-Avtive Light
  digitalWrite(4, LOW);  // turn off
  delay(100);

  pir_enable();

}

bool init_wifi() {

  WiFi.disconnect(true, true);

  //jMulti.setStrictMode(); // not implemented yet

  if (strlen(ssid1) > 0 && (strcmp(ssid1, "none") != 0)) {
    jMulti.addAP(ssid1, pass1);
    Serial.printf("added %s / %s\n", ssid1, pass1);
  }
  if (strlen(ssid2) > 0 && (strcmp(ssid2, "none") != 0)) {
    jMulti.addAP(ssid2, pass2);
    Serial.printf("added %s / %s\n", ssid2, pass2);
  }
  if (strlen(ssid3) > 0 && (strcmp(ssid3, "none") != 0)) {
    jMulti.addAP(ssid3, pass3);
    Serial.printf("added %s / %s\n", ssid3, pass3);
  }

  jMulti.run();
  int fails = 0;
  while (WiFi.status() != WL_CONNECTED && fails < 20) {
    delay(500);
    Serial.print(".");
    fails++;
  }

  if (WiFi.status() != WL_CONNECTED) {
    if (!MDNS.begin(devname)) {
      Serial.println("Error setting up MDNS responder!");
      return false;
    } else {
      Serial.printf("mDNS responder started '%s'\n", devname);
    }
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }

  wifi_ps_type_t the_type;
  //esp_err_t get_ps = esp_wifi_get_ps(&the_type);
  esp_err_t set_ps = esp_wifi_set_ps(WIFI_PS_NONE);

  time(&now);

  configTime(0, 0, "pool.ntp.org");
  Serial.println(timezone);
  setenv("TZ", timezone, 1);  // mountain time zone from #define at top
  tzset();

  return true;
}


//////////////////////////////////////////////////////////////////////////////////////

void setup() {
  Serial.begin(115200);
  Serial.println("---------------------------------");
  Serial.printf("ESP32-CAM Video-Telegram %s\n", vernum);
  Serial.println("---------------------------------");

  pinMode(FLASH_LED_PIN, OUTPUT);
  digitalWrite(FLASH_LED_PIN, flashState);  //defaults to low

  pinMode(12, INPUT_PULLUP);  // pull this down to stop recording

  pinMode(33, OUTPUT);    // little red led on back of chip
  digitalWrite(33, LOW);  // turn on the red LED on the back of chip

  int avail_psram = ESP.getFreePsram();
  Serial.print("PSRAM size to store the video ");
  Serial.println(avail_psram);

  do_eprom_read();

  Serial.printf("Init soft\n");
  bool soft_status = init_soft();
  Serial.printf("Init wifi\n");
  bool wifi_status = init_wifi();

  startHttpServer();

  idx_buf_size = max_frames * 10 + 20;


  if (strcmp(frame, "HD") == 0) {
    avi_buf_size = avail_psram - 900 * 1024;  //900 for hd, 500 for vga
    //avi_buf_size = avail_psram - 700 * 1024;  //900 for hd, 500 for vga
    framesize = FRAMESIZE_HD;
    Serial.println("Camera to HD ");
  } else {
    framesize = FRAMESIZE_VGA;
    avi_buf_size = avail_psram - 500 * 1024;  //900 for hd, 500 for vga
    //avi_buf_size = avail_psram - 300 * 1024;  //900 for hd, 500 for vga
    Serial.println("Camera to VGA ");
  }

  Serial.print("try to allocate ");
  Serial.println(avi_buf_size);
  Serial.printf("before malloc RAM  %7d / %7d, SPI %7d / %7d\n ", ESP.getFreeHeap(), ESP.getHeapSize(),  ESP.getFreePsram(), ESP.getPsramSize() );

  psram_avi_buf = (uint8_t*)ps_malloc(avi_buf_size);
  if (psram_avi_buf == 0) Serial.printf("psram_avi allocation failed\n");
  psram_idx_buf = (uint8_t*)ps_malloc(idx_buf_size);  // save file in psram
  if (psram_idx_buf == 0) Serial.printf("psram_idx allocation failed\n");
  Serial.printf("after malloc RAM  %7d / %7d, SPI %7d / %7d\n ", ESP.getFreeHeap(), ESP.getHeapSize(),  ESP.getFreePsram(), ESP.getPsramSize() );

  if (!setupCamera()) {
    Serial.println("Camera Setup Failed!");
    while (true) {
      delay(1000);
    }
  }
  Serial.printf("after cam RAM  %7d / %7d, SPI %7d / %7d\n ", ESP.getFreeHeap(), ESP.getHeapSize(),  ESP.getFreePsram(), ESP.getPsramSize() );

  sensor_t * s2 = esp_camera_sensor_get();
  s2->set_reg(s2, 0xff, 0xff, 0x01); //banksel
  delay(1);
  int li2 = s2->get_reg(s2, 0x2f, 0xff);

  for (int j = 0; j < 7; j++) {
    camera_fb_t* fb = esp_camera_fb_get();
    if (!fb) {
      Serial.println("Camera Capture Failed at setup");
    } else {
      int li2 = s2->get_reg(s2, 0x2f, 0xff);
      Serial.print("Pic, len=");
      Serial.print(fb->len);
      Serial.printf(", new fb %X, light %3d\n", (long)fb->buf, li2);
      esp_camera_fb_return(fb);
      delay(50);
    }
  }

  ///  ota updates always enabled without password at either softap ip or router ip
  ArduinoOTA.setHostname(ssidap);
  ArduinoOTA.setPassword("mrpeanut");
  ArduinoOTA
  .onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
      type = "sketch";
    else // U_SPIFFS
      type = "filesystem";

    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    Serial.println("Start updating " + type);
  })
  .onEnd([]() {
    Serial.println("\nEnd");
  })
  .onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  })
  .onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });


  // Make the bot wait for a new message for up to 60seconds
  //bot.longPoll = 60;
  bot.longPoll = 15;  //ver10

  //client.setInsecure();                          // use April 2024 Brian Lough library with a certificate
  client.setCACert(TELEGRAM_CERTIFICATE_ROOT);



  // mount the sd card if it is plugged in

  init_sd();

  //setupinterrupts();

  pir_setup();

  do_initial_send = true;
  ArduinoOTA.begin();
  digitalWrite(33, HIGH);
}

int loopcount = 0;
int next_wifi_check = 60000;
int next_sd_check = 60000;

void loop() {
  loopcount++;

  //Serial.printf("l");

  // filemanager do download view files on sd card

  if (we_have_sd_card == true) {
    pir_disable();
    delay(1);
    for (int i = 0; i < 1000; i++) {
      filemgr.handleClient();
    }
    pir_enable();
  }

  // ota handler

  for (int i = 0; i < 1000; i++) {
    ArduinoOTA.handle();
  }

  // if we have internet router, then talk to telegram, else camera still has softap access and event with sd card

  if (do_initial_send && WiFi.status() == WL_CONNECTED) {
    do_initial_send = false;
    String stat = "Reboot\nDevice: " + devstr + "\nVer: " + String(vernum) + "\nRssi: " + String(WiFi.RSSI()) + "\nip: " + WiFi.localIP().toString() + "\n/start";
    Serial.println(stat);
    Serial.print("Sending a message to: ");
    Serial.print(chat_id);
    Serial.print(" at ");
    Serial.println(BOTtoken);

    if (bot.sendMessage(chat_id, stat, "")) {
      Serial.println("Initial send worked!");
    } else {
      Serial.println("Initial send failed");
      reset_request = true;
    }
  }

  if (reboot_request) {
    String stat = "Rebooting on request\nDevice: " + devstr + "\nVer: " + String(vernum) + "\nRssi: " + String(WiFi.RSSI()) + "\nip: " + WiFi.localIP().toString();
    bot.sendMessage(chat_id, stat, "");
    delay(10000);
    ESP.restart();
  }


  if (send_a_telegram_test && WiFi.status() == WL_CONNECTED) {
    send_a_telegram_test = false;
    String stat = "Test from ESP32 -> Telegram\nDevice: " + devstr + "\nVer: " + String(vernum) + "\nRssi: " + String(WiFi.RSSI()) + "\nip: " + WiFi.localIP().toString() + "\n/start";
    //String stat = "Test from ESP32 -> Telegram";
    Serial.println(stat);

    if (bot.sendMessage(chat_id, stat, "")) {
      Serial.println("Telegram test worked!");
    } else {
      Serial.println("Telegram test failed");
    }
  }

  //  do some jobs using the heap of the Loop rather than inside an event

  if (picture_ready) {
    picture_ready = false;
    send_the_picture();
  }

  if (video_ready) {
    video_ready = false;
    send_the_video();
  }

  if (millis() > Bot_lasttime + Bot_mtbs) {

    if (WiFi.status() != WL_CONNECTED) {

      if (millis() > next_wifi_check) {

        // check the wifi once an minute to try to connect

        next_wifi_check = millis() + 60000;
        if (WiFi.status() != WL_CONNECTED) {
          Serial.println("***** WiFi reconnect *****");
          init_wifi();
        }
      }
    }

    if (!we_have_sd_card && millis() > next_sd_check) {
      next_sd_check = millis() + 5 * 60000;
      init_sd();
    }

    int numNewMessages = 0;
    if (WiFi.status() == WL_CONNECTED) {
      //Serial.print("get>>>");
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
      //Serial.println("<<<got");
    }

    if (clear_the_queue) {
      clear_the_queue = false;
      while (numNewMessages) {
        for (int i = 0; i < numNewMessages; i++) {
          String text = bot.messages[i].text;
          Serial.printf("\nSkip a message: ");
          Serial.println(text);
        }
        numNewMessages = bot.getUpdates(bot.last_message_received + 1);
      }
    } else {
      while (numNewMessages) {
        //Serial.println("got response");
        handleNewMessages(numNewMessages);
        numNewMessages = bot.getUpdates(bot.last_message_received + 1);
      }
    }
    Bot_lasttime = millis();
  }

  // record avi on web request, rather than telegram request

  if (test_send_avi) {
    // record the video
    test_send_avi = false;
    bot.longPoll = 0;
    Serial.printf("Web clip req\n");
    xTaskCreatePinnedToCore(the_camera_loop, "the_camera_loop", 10000, NULL, 1, &the_camera_loop_task, 1);

    if (the_camera_loop_task == NULL) {
      Serial.printf("do_the_steaming_task failed to start! %d\n", the_camera_loop_task);
    }
  }

  // web request photo

  if (test_send) {
    test_send = false;
    camera_fb_t* tfb = esp_camera_fb_get();
    if (!tfb) {
      Serial.println("Camera Capture Failed at test_send");
    } else {

      currentByte = 0;
      fb_length = tfb->len;
      fb_buffer = tfb->buf;

      Serial.println("\n>>>>> Sending test photo, bytes=  " + String(fb_length));

      if (WiFi.status() == WL_CONNECTED) String sent = bot.sendMultipartFormDataToTelegramWithCaption("sendPhoto", "photo", "img.jpg",
            "image/jpeg", "Test Photo", chat_id, fb_length,
            isMoreDataAvailable, getNextByte, nullptr, nullptr);

      if (we_have_sd_card) save_jpg("test", fb_buffer, fb_length);

      Serial.println("done!");
    }
    esp_camera_fb_return(tfb);
    delay(10);
  }

  // one minute before timed photo

  if (millis() > (RaduPhoto_lasttime + TimePhoto_Minutes  * 60000)) {

    int li2 = 60;
    sensor_t * s2 = esp_camera_sensor_get();
    s2->set_reg(s2, 0xff, 0xff, 0x01); //banksel
    delay(1);
    li2 = s2->get_reg(s2, 0x2f, 0xff);

    if ((li2  < 50 && radu_level == -1) || (radu_level > 0 && li2 < (radu_level - 8) )) {

      String stat;
      if  (strcmp(raduauto, "true") == 0 ) {
        stat = "Very *dark* , setting radu /radu" + String(li2) + "\n";
      } else {
        stat = "Very *dark* , click to set radu /radu" + String(li2) + "\n";

        //String stat = "Very *dark* , set radu [/radu" + String(li2) + "](/radu" + String(li2) + ")\n";

      }
      if (WiFi.status() == WL_CONNECTED) bot.sendMessage(chat_id, stat, "Markdown");

      if  (strcmp(raduauto, "true") == 0 ) {
        Serial.printf("Timed Calling radu with % d\n", li2);
        re_init();
        set_radu(li2);
      }
    }

    String stat;
    if (li2  > 150 ) {
      if  (strcmp(raduauto, "true") == 0 ) {
        stat = "Very bright " + String(li2) + " -- setting auto-light /reg \n";
      } else {
        stat = "Very bright " + String(li2) + " -- click to go back to auto-light  /reg \n";
      }

      if (WiFi.status() == WL_CONNECTED) bot.sendMessage(chat_id, stat, "");

      if  (strcmp(raduauto, "true") == 0 ) {
        Serial.printf("Timed re_init % d\n", li2);
        re_init();
      }
    }
    RaduPhoto_lasttime = millis();
  }

  // timed photo

  if (millis() > (TimePhoto_lasttime + TimePhoto_Minutes * 60000)) {

    if (tim_enabled) {
      int y = millis();
      int li2 = 60;
      camera_fb_t* tfb = esp_camera_fb_get();
      if (!tfb) {
        Serial.println("Camera Capture Failed at timed");
      } else {

        int pic_time = millis() - y;

        currentByte = 0;
        fb_length = tfb->len;
        fb_buffer = tfb->buf;

        sensor_t * s2 = esp_camera_sensor_get();
        s2->set_reg(s2, 0xff, 0xff, 0x01); //banksel
        delay(1);
        li2 = s2->get_reg(s2, 0x2f, 0xff);

        String caption_text = "Timed Photo, mode: " + String(radu_level) + ", light: " + String(li2)  +
                              ", pic time: "  + String(pic_time) + ", len " + String(fb_length) +  "\n";

        if (WiFi.status() == WL_CONNECTED) {
          Serial.println("\n>>>>> Sending timed photo, bytes=  " + String(fb_length));
          Serial.printf("RAM  %7d / %7d, SPI %7d / %7d\n ", ESP.getFreeHeap(), ESP.getHeapSize(), ESP.getFreePsram(), ESP.getPsramSize());
          String sent = bot.sendMultipartFormDataToTelegramWithCaption("sendPhoto", "photo", "img.jpg",
                        "image/jpeg", caption_text, chat_id, fb_length,
                        isMoreDataAvailable, getNextByte, nullptr, nullptr);
        }
        if (we_have_sd_card) save_jpg("tim", fb_buffer, fb_length);
        Serial.println("done!");

        esp_camera_fb_return(tfb);
        //Serial.printf(">%s<\n",raduauto);


        TimePhoto_lasttime = millis();
        RaduPhoto_lasttime = millis() - 60000;
      }
    }


    //zzz


  } ////////////// time


  if (new_radu != -1) {
    int x = new_radu;
    new_radu = -1;
    if (x >= 50) {
      re_init();
    } else {

      set_radu(x);
    }
  }

}  // end of the loop


// if we have an sd card, save photos and video to the card under a folder with todays date

void save_jpg(const char* name, uint8_t* fb_buffer, size_t fb_length) {
  char buf2[50];
  char buf1[80];
  struct tm timeinfo;
  time_t now;
  char fname[80];
  File thefile;

  bool save_pir = pir_enabled;
  pir_enabled = false;
  pir_disable();

  time(&now);
  localtime_r(&now, &timeinfo);

  strftime(buf2, sizeof(buf2), "/%Y%m%d", &timeinfo);
  SD_MMC.mkdir(buf2);

  strftime(buf1, sizeof(buf), "%F_%H.%M.%S", &timeinfo);
  sprintf(fname, "/%s/%s_%s.jpg", buf2, devname, buf1);
  //Serial.println(fname);

  thefile = SD_MMC.open(fname, "w");
  size_t err = thefile.write(fb_buffer, fb_length);
  if (err != fb_length) {
    Serial.print("Error on avi write: err = ");
    Serial.print(err);
  }

  thefile.close();

  pir_enabled = save_pir;
  pinMode(13, INPUT_PULLDOWN);
  pir_enable();

}

void save_avi(const char* fname, uint8_t* fb_buffer, size_t fb_length) {


  File thefile;
#define fbsb 2
  uint8_t framebuffer_static[fbsb * 1024 + 20];
  int fb_block_length;
  uint8_t* fb_block_start;
  size_t fblen;
  int remnant;

  bool save_pir = pir_enabled;
  pir_enabled = false;
  pir_disable();

  thefile = SD_MMC.open(fname, "w");

  fb_block_start = fb_buffer;
  fblen = fb_length;

  while (fblen > 0) {
    if (fblen > fbsb * 1024) {
      fb_block_length = fbsb * 1024;
      fblen = fblen - fb_block_length;
    } else {
      fb_block_length = fblen;
      fblen = 0;
    }

    memcpy(framebuffer_static, fb_block_start, fb_block_length);

    size_t err = thefile.write(framebuffer_static, fb_block_length);

    if (err != fb_block_length) {
      Serial.print("Error on avi write: err = ");
      Serial.print(err);
      Serial.print(" len = ");
      Serial.println(fb_block_length);
    }

    fb_block_start = fb_block_start + fb_block_length;
    delay(0);
  }


  thefile.close();

  pir_enabled = save_pir;
  pinMode(13, INPUT_PULLDOWN);
  pir_enable();

}


void send_the_picture() {
  digitalWrite(33, LOW);  // light on
  currentByte = 0;
  fb_length = vid_fb->len;
  fb_buffer = vid_fb->buf;



  if (active_interupt) {
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("\n>>>>> Sending as 512 byte blocks, bytes=  " + String(fb_length));
      String sent = bot.sendMultipartFormDataToTelegramWithCaption("sendPhoto", "photo", "img.jpg",
                    "image/jpeg", "PIR Event!", chat_id, fb_length,
                    isMoreDataAvailable, getNextByte, nullptr, nullptr);
    }
    if (we_have_sd_card) save_jpg("pir", fb_buffer, fb_length);
  } else {
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("\n>>>>> Sending as 512 byte blocks, bytes=  " + String(fb_length));
      String sent = bot.sendMultipartFormDataToTelegramWithCaption("sendPhoto", "photo", "img.jpg",

                    "image/jpeg", "Telegram Request", chat_id, fb_length,
                    isMoreDataAvailable, getNextByte, nullptr, nullptr);
    }
    if (we_have_sd_card)  save_jpg("req", fb_buffer, fb_length);
  }
  esp_camera_fb_return(vid_fb);
  bot.longPoll = 0;
  digitalWrite(33, HIGH);  // light oFF
  if (!avi_enabled) active_interupt = false;
}

void send_the_file(char * fname) {
  digitalWrite(33, LOW);  // light on
  Serial.println("\n\n\nSending file");


  File thefile;

  bool save_pir = pir_enabled;
  pir_enabled = false;
  pir_disable();

  thefile = SD_MMC.open(fname, "r");
  if (!thefile) {
    Serial.printf("Filename does not exist: >%s<\n", fname);
  } else {
    int len = thefile.size();
    size_t err = thefile.read(psram_avi_buf, len);
    if (err != len) {
      Serial.print("Error on avi read: err = ");
      Serial.print(err);
    }
    thefile.close();

    avi_buf = psram_avi_buf;
    //psram_avi_ptr = psram_avi_buf + len;

    avi_ptr = 0;
    avi_len = len; // psram_avi_ptr - psram_avi_buf;

    if (WiFi.status() == WL_CONNECTED) {
      String sent2 = bot.sendMultipartFormDataToTelegramWithCaption("sendDocument", "document", String(fname).substring(1),
                     "image/jpeg", "Resquested file", chat_id, len,
                     avi_more, avi_next, nullptr, nullptr);

      Serial.println("done!");
    }
  }
  digitalWrite(33, HIGH);  // light off

  bot.longPoll = 15;  //ver10 from 5


  active_interupt = false;

  pir_enabled = save_pir;
  pinMode(13, INPUT_PULLDOWN);
  pir_enable();
}

void send_the_video() {
  digitalWrite(33, LOW);  // light on

  char buf2[50];
  char buf1[80];
  struct tm timeinfo;
  char fname[80];

  pir_disable();

  time_t now;
  time(&now);
  localtime_r(&now, &timeinfo);

  strftime(buf2, sizeof(buf2), "/%Y%m%d", &timeinfo);

  strftime(buf1, sizeof(buf), "%F_%H.%M.%S", &timeinfo);
  sprintf(fname, "/%s/%s_%s.avi", buf2, devname, buf1);



  avi_buf = psram_avi_buf;

  avi_ptr = 0;
  avi_len = psram_avi_ptr - psram_avi_buf;

  if (we_have_sd_card) {
    SD_MMC.mkdir(buf2);
    save_avi(fname, avi_buf, avi_len);
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n\n\nSending clip with caption");
    Serial.println("\n>>>>> Sending as 512 byte blocks, with a caption,  bytes=  " + String(psram_avi_ptr - psram_avi_buf));


    //String stat = "Video http://" + WiFi.localIP().toString() + ":8080/c?dwn=" + String(fname) + "\n\n" + "[/send=" + String(fname) + "](/send=" + String(fname) + ")\n";
    String stat = "Video http://" + WiFi.localIP().toString() + ":8080/c?dwn=" + String(fname) + "\n\n" + "/send=" + String(fname) + "\n (Cut and paste all that)\n";
    bot.sendMessage(chat_id, stat, "");

    //time(&now);
    //localtime_r(&now, &timeinfo);
    //strftime(strftime_buf, sizeof(strftime_buf), "DoorCam %F %H.%M.%S.avi", &timeinfo);

    if (bool_send_all_avi) {
      String sent2 = bot.sendMultipartFormDataToTelegramWithCaption("sendDocument", "document", fname,
                     "image/jpeg", "Intruder alert!", chat_id, psram_avi_ptr - psram_avi_buf,
                     avi_more, avi_next, nullptr, nullptr);

      Serial.println("done!");
    }
  }


  digitalWrite(33, HIGH);  // light off

  bot.longPoll = 15;  //ver10 from 5
  active_interupt = false;
  pir_enable();
}
