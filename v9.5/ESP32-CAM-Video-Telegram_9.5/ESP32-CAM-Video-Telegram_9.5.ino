/*******************************************************************

  ESP32-CAM-Video-Telegram

  This program records an mjpeg avi video in the psram of a ESP32-CAM, and sends a jpeg and a avi video to Telegram.

  https://github.com/jameszah/ESP32-CAM-Video-Telegram is licensed under the
    GNU General Public License v3.0

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
  
Linking everything together...
"C:\\ArduinoPortable\\arduino-1.8.19\\portable\\packages\\esp32\\tools\\xtensa-esp32-elf-gcc\\gcc8_4_0-esp-2021r2-patch3/bin/xtensa-esp32-elf-g++" "-Wl,--Map=C:\\Users\\James\\AppData\\Local\\Temp\\arduino_build_302777/ESP32-CAM-Video-Telegram_9.5.ino.map" "-LC:\\ArduinoPortable\\arduino-1.8.19\\portable\\packages\\esp32\\hardware\\esp32\\2.0.4/tools/sdk/esp32/lib" "-LC:\\ArduinoPortable\\arduino-1.8.19\\portable\\packages\\esp32\\hardware\\esp32\\2.0.4/tools/sdk/esp32/ld" "-LC:\\ArduinoPortable\\arduino-1.8.19\\portable\\packages\\esp32\\hardware\\esp32\\2.0.4/tools/sdk/esp32/qio_qspi" -T esp32.rom.redefined.ld -T memory.ld -T sections.ld -T esp32.rom.ld -T esp32.rom.api.ld -T esp32.rom.libgcc.ld -T esp32.rom.newlib-data.ld -T esp32.rom.syscalls.ld -T esp32.peripherals.ld -mlongcalls -Wno-frame-address -Wl,--cref -Wl,--gc-sections -fno-rtti -fno-lto -u ld_include_hli_vectors_bt -u _Z5setupv -u _Z4loopv -u esp_app_desc -u pthread_include_pthread_impl -u pthread_include_pthread_cond_impl -u pthread_include_pthread_local_storage_impl -u pthread_include_pthread_rwlock_impl -u include_esp_phy_override -u ld_include_highint_hdl -u start_app -u start_app_other_cores -u __ubsan_include -Wl,--wrap=longjmp -u __assert_func -u vfs_include_syscalls_impl -Wl,--undefined=uxTopUsedPriority -u app_main -u newlib_include_heap_impl -u newlib_include_syscalls_impl -u newlib_include_pthread_impl -u newlib_include_assert_impl -u __cxa_guard_dummy -DESP32 -DCORE_DEBUG_LEVEL=0 -DBOARD_HAS_PSRAM -mfix-esp32-psram-cache-issue -mfix-esp32-psram-cache-strategy=memw -DARDUINO_USB_CDC_ON_BOOT=0 -Wl,--start-group "C:\\Users\\James\\AppData\\Local\\Temp\\arduino_build_302777\\sketch\\ESP32-CAM-Video-Telegram_9.5.ino.cpp.o" "C:\\Users\\James\\AppData\\Local\\Temp\\arduino_build_302777\\sketch\\UniversalTelegramBot.cpp.o" "C:\\Users\\James\\AppData\\Local\\Temp\\arduino_build_302777\\libraries\\WiFi\\WiFi.cpp.o" "C:\\Users\\James\\AppData\\Local\\Temp\\arduino_build_302777\\libraries\\WiFi\\WiFiAP.cpp.o" "C:\\Users\\James\\AppData\\Local\\Temp\\arduino_build_302777\\libraries\\WiFi\\WiFiClient.cpp.o" "C:\\Users\\James\\AppData\\Local\\Temp\\arduino_build_302777\\libraries\\WiFi\\WiFiGeneric.cpp.o" "C:\\Users\\James\\AppData\\Local\\Temp\\arduino_build_302777\\libraries\\WiFi\\WiFiMulti.cpp.o" "C:\\Users\\James\\AppData\\Local\\Temp\\arduino_build_302777\\libraries\\WiFi\\WiFiSTA.cpp.o" "C:\\Users\\James\\AppData\\Local\\Temp\\arduino_build_302777\\libraries\\WiFi\\WiFiScan.cpp.o" "C:\\Users\\James\\AppData\\Local\\Temp\\arduino_build_302777\\libraries\\WiFi\\WiFiServer.cpp.o" "C:\\Users\\James\\AppData\\Local\\Temp\\arduino_build_302777\\libraries\\WiFi\\WiFiUdp.cpp.o" "C:\\Users\\James\\AppData\\Local\\Temp\\arduino_build_302777\\libraries\\WiFiClientSecure\\esp_crt_bundle.c.o" "C:\\Users\\James\\AppData\\Local\\Temp\\arduino_build_302777\\libraries\\WiFiClientSecure\\WiFiClientSecure.cpp.o" "C:\\Users\\James\\AppData\\Local\\Temp\\arduino_build_302777\\libraries\\WiFiClientSecure\\ssl_client.cpp.o" "C:\\Users\\James\\AppData\\Local\\Temp\\arduino_build_302777\\libraries\\WiFiManager\\WiFiManager.cpp.o" "C:\\Users\\James\\AppData\\Local\\Temp\\arduino_build_302777\\libraries\\Update\\HttpsOTAUpdate.cpp.o" "C:\\Users\\James\\AppData\\Local\\Temp\\arduino_build_302777\\libraries\\Update\\Updater.cpp.o" "C:\\Users\\James\\AppData\\Local\\Temp\\arduino_build_302777\\libraries\\WebServer\\Parsing.cpp.o" "C:\\Users\\James\\AppData\\Local\\Temp\\arduino_build_302777\\libraries\\WebServer\\WebServer.cpp.o" "C:\\Users\\James\\AppData\\Local\\Temp\\arduino_build_302777\\libraries\\WebServer\\detail\\mimetable.cpp.o" "C:\\Users\\James\\AppData\\Local\\Temp\\arduino_build_302777\\libraries\\DNSServer\\DNSServer.cpp.o" "C:\\Users\\James\\AppData\\Local\\Temp\\arduino_build_302777\\libraries\\EEPROM\\EEPROM.cpp.o" "C:\\Users\\James\\AppData\\Local\\Temp\\arduino_build_302777\\libraries\\ESPmDNS\\ESPmDNS.cpp.o" "C:\\Users\\James\\AppData\\Local\\Temp\\arduino_build_302777\\libraries\\FS\\FS.cpp.o" "C:\\Users\\James\\AppData\\Local\\Temp\\arduino_build_302777\\libraries\\FS\\vfs_api.cpp.o" "C:\\Users\\James\\AppData\\Local\\Temp\\arduino_cache_728960\\core\\core_2baeb7081b63b786f353f94f995ac23a.a" -lesp_ringbuf -lefuse -lesp_ipc -ldriver -lesp_pm -lmbedtls -lapp_update -lbootloader_support -lspi_flash -lnvs_flash -lpthread -lesp_gdbstub -lespcoredump -lesp_phy -lesp_system -lesp_rom -lhal -lvfs -lesp_eth -ltcpip_adapter -lesp_netif -lesp_event -lwpa_supplicant -lesp_wifi -lconsole -llwip -llog -lheap -lsoc -lesp_hw_support -lxtensa -lesp_common -lesp_timer -lfreertos -lnewlib -lcxx -lapp_trace -lasio -lbt -lcbor -lunity -lcmock -lcoap -lnghttp -lesp-tls -lesp_adc_cal -lesp_hid -ltcp_transport -lesp_http_client -lesp_http_server -lesp_https_ota -lesp_https_server -lesp_lcd -lprotobuf-c -lprotocomm -lmdns -lesp_local_ctrl -lsdmmc -lesp_serial_slave_link -lesp_websocket_client -lexpat -lwear_levelling -lfatfs -lfreemodbus -ljsmn -ljson -llibsodium -lmqtt -lopenssl -lperfmon -lspiffs -lulp -lwifi_provisioning -lbutton -lrmaker_common -ljson_parser -ljson_generator -lesp_schedule -lesp_rainmaker -lqrcode -lws2812_led -lesp-dsp -lesp-sr -lesp32-camera -lesp_littlefs -lfb_gfx -lasio -lcbor -lcmock -lunity -lcoap -lesp_lcd -lesp_websocket_client -lexpat -lfreemodbus -ljsmn -llibsodium -lperfmon -lesp_adc_cal -lesp_hid -lfatfs -lwear_levelling -lopenssl -lesp_rainmaker -lesp_local_ctrl -lesp_https_server -lwifi_provisioning -lprotocomm -lbt -lbtdm_app -lprotobuf-c -lmdns -lrmaker_common -lmqtt -ljson_parser -ljson_generator -lesp_schedule -lqrcode -lcat_face_detect -lhuman_face_detect -lcolor_detect -lmfn -ldl -lwakenet -lmultinet -lesp_audio_processor -lesp_audio_front_end -lesp-sr -lwakenet -lmultinet -lesp_audio_processor -lesp_audio_front_end -ljson -lspiffs -ldl_lib -lc_speech_features -lhilexin_wn5 -lhilexin_wn5X2 -lhilexin_wn5X3 -lnihaoxiaozhi_wn5 -lnihaoxiaozhi_wn5X2 -lnihaoxiaozhi_wn5X3 -lnihaoxiaoxin_wn5X3 -lcustomized_word_wn5 -lmultinet2_ch -lesp_tts_chinese -lvoice_set_xiaole -lesp_ringbuf -lefuse -lesp_ipc -ldriver -lesp_pm -lmbedtls -lapp_update -lbootloader_support -lspi_flash -lnvs_flash -lpthread -lesp_gdbstub -lespcoredump -lesp_phy -lesp_system -lesp_rom -lhal -lvfs -lesp_eth -ltcpip_adapter -lesp_netif -lesp_event -lwpa_supplicant -lesp_wifi -lconsole -llwip -llog -lheap -lsoc -lesp_hw_support -lxtensa -lesp_common -lesp_timer -lfreertos -lnewlib -lcxx -lapp_trace -lnghttp -lesp-tls -ltcp_transport -lesp_http_client -lesp_http_server -lesp_https_ota -lsdmmc -lesp_serial_slave_link -lulp -lmbedtls_2 -lmbedcrypto -lmbedx509 -lcoexist -lcore -lespnow -lmesh -lnet80211 -lpp -lsmartconfig -lwapi -lesp_ringbuf -lefuse -lesp_ipc -ldriver -lesp_pm -lmbedtls -lapp_update -lbootloader_support -lspi_flash -lnvs_flash -lpthread -lesp_gdbstub -lespcoredump -lesp_phy -lesp_system -lesp_rom -lhal -lvfs -lesp_eth -ltcpip_adapter -lesp_netif -lesp_event -lwpa_supplicant -lesp_wifi -lconsole -llwip -llog -lheap -lsoc -lesp_hw_support -lxtensa -lesp_common -lesp_timer -lfreertos -lnewlib -lcxx -lapp_trace -lnghttp -lesp-tls -ltcp_transport -lesp_http_client -lesp_http_server -lesp_https_ota -lsdmmc -lesp_serial_slave_link -lulp -lmbedtls_2 -lmbedcrypto -lmbedx509 -lcoexist -lcore -lespnow -lmesh -lnet80211 -lpp -lsmartconfig -lwapi -lesp_ringbuf -lefuse -lesp_ipc -ldriver -lesp_pm -lmbedtls -lapp_update -lbootloader_support -lspi_flash -lnvs_flash -lpthread -lesp_gdbstub -lespcoredump -lesp_phy -lesp_system -lesp_rom -lhal -lvfs -lesp_eth -ltcpip_adapter -lesp_netif -lesp_event -lwpa_supplicant -lesp_wifi -lconsole -llwip -llog -lheap -lsoc -lesp_hw_support -lxtensa -lesp_common -lesp_timer -lfreertos -lnewlib -lcxx -lapp_trace -lnghttp -lesp-tls -ltcp_transport -lesp_http_client -lesp_http_server -lesp_https_ota -lsdmmc -lesp_serial_slave_link -lulp -lmbedtls_2 -lmbedcrypto -lmbedx509 -lcoexist -lcore -lespnow -lmesh -lnet80211 -lpp -lsmartconfig -lwapi -lesp_ringbuf -lefuse -lesp_ipc -ldriver -lesp_pm -lmbedtls -lapp_update -lbootloader_support -lspi_flash -lnvs_flash -lpthread -lesp_gdbstub -lespcoredump -lesp_phy -lesp_system -lesp_rom -lhal -lvfs -lesp_eth -ltcpip_adapter -lesp_netif -lesp_event -lwpa_supplicant -lesp_wifi -lconsole -llwip -llog -lheap -lsoc -lesp_hw_support -lxtensa -lesp_common -lesp_timer -lfreertos -lnewlib -lcxx -lapp_trace -lnghttp -lesp-tls -ltcp_transport -lesp_http_client -lesp_http_server -lesp_https_ota -lsdmmc -lesp_serial_slave_link -lulp -lmbedtls_2 -lmbedcrypto -lmbedx509 -lcoexist -lcore -lespnow -lmesh -lnet80211 -lpp -lsmartconfig -lwapi -lesp_ringbuf -lefuse -lesp_ipc -ldriver -lesp_pm -lmbedtls -lapp_update -lbootloader_support -lspi_flash -lnvs_flash -lpthread -lesp_gdbstub -lespcoredump -lesp_phy -lesp_system -lesp_rom -lhal -lvfs -lesp_eth -ltcpip_adapter -lesp_netif -lesp_event -lwpa_supplicant -lesp_wifi -lconsole -llwip -llog -lheap -lsoc -lesp_hw_support -lxtensa -lesp_common -lesp_timer -lfreertos -lnewlib -lcxx -lapp_trace -lnghttp -lesp-tls -ltcp_transport -lesp_http_client -lesp_http_server -lesp_https_ota -lsdmmc -lesp_serial_slave_link -lulp -lmbedtls_2 -lmbedcrypto -lmbedx509 -lcoexist -lcore -lespnow -lmesh -lnet80211 -lpp -lsmartconfig -lwapi -lphy -lrtc -lesp_phy -lphy -lrtc -lesp_phy -lphy -lrtc -lxt_hal -lm -lnewlib -lstdc++ -lpthread -lgcc -lcxx -lapp_trace -lgcov -lapp_trace -lgcov -lc -Wl,--end-group -Wl,-EL -o "C:\\Users\\James\\AppData\\Local\\Temp\\arduino_build_302777/ESP32-CAM-Video-Telegram_9.5.ino.elf"
"C:\\ArduinoPortable\\arduino-1.8.19\\portable\\packages\\esp32\\tools\\esptool_py\\3.3.0/esptool.exe" --chip esp32 elf2image --flash_mode dio --flash_freq 80m --flash_size 4MB -o "C:\\Users\\James\\AppData\\Local\\Temp\\arduino_build_302777/ESP32-CAM-Video-Telegram_9.5.ino.bin" "C:\\Users\\James\\AppData\\Local\\Temp\\arduino_build_302777/ESP32-CAM-Video-Telegram_9.5.ino.elf"
esptool.py v3.3
Creating esp32 image...
Merged 25 ELF sections
Successfully created esp32 image.
"C:\\ArduinoPortable\\arduino-1.8.19\\portable\\packages\\esp32\\hardware\\esp32\\2.0.4/tools/gen_esp32part.exe" -q "C:\\Users\\James\\AppData\\Local\\Temp\\arduino_build_302777/partitions.csv" "C:\\Users\\James\\AppData\\Local\\Temp\\arduino_build_302777/ESP32-CAM-Video-Telegram_9.5.ino.partitions.bin"
Multiple libraries were found for "WiFi.h"
 Used: C:\ArduinoPortable\arduino-1.8.19\portable\packages\esp32\hardware\esp32\2.0.4\libraries\WiFi
 Not used: C:\ArduinoPortable\arduino-1.8.19\libraries\WiFi
Using library WiFi at version 2.0.0 in folder: C:\ArduinoPortable\arduino-1.8.19\portable\packages\esp32\hardware\esp32\2.0.4\libraries\WiFi 
Using library WiFiClientSecure at version 2.0.0 in folder: C:\ArduinoPortable\arduino-1.8.19\portable\packages\esp32\hardware\esp32\2.0.4\libraries\WiFiClientSecure 
Using library WiFiManager at version 2.0.11-beta in folder: C:\ArduinoPortable\sketch\libraries\WiFiManager 
Using library Update at version 2.0.0 in folder: C:\ArduinoPortable\arduino-1.8.19\portable\packages\esp32\hardware\esp32\2.0.4\libraries\Update 
Using library WebServer at version 2.0.0 in folder: C:\ArduinoPortable\arduino-1.8.19\portable\packages\esp32\hardware\esp32\2.0.4\libraries\WebServer 
Using library DNSServer at version 2.0.0 in folder: C:\ArduinoPortable\arduino-1.8.19\portable\packages\esp32\hardware\esp32\2.0.4\libraries\DNSServer 
Using library ArduinoJson at version 6.19.4 in folder: C:\ArduinoPortable\sketch\libraries\ArduinoJson 
Using library EEPROM at version 2.0.0 in folder: C:\ArduinoPortable\arduino-1.8.19\portable\packages\esp32\hardware\esp32\2.0.4\libraries\EEPROM 
Using library ESPmDNS at version 2.0.0 in folder: C:\ArduinoPortable\arduino-1.8.19\portable\packages\esp32\hardware\esp32\2.0.4\libraries\ESPmDNS 
Using library FS at version 2.0.0 in folder: C:\ArduinoPortable\arduino-1.8.19\portable\packages\esp32\hardware\esp32\2.0.4\libraries\FS 
"C:\\ArduinoPortable\\arduino-1.8.19\\portable\\packages\\esp32\\tools\\xtensa-esp32-elf-gcc\\gcc8_4_0-esp-2021r2-patch3/bin/xtensa-esp32-elf-size" -A "C:\\Users\\James\\AppData\\Local\\Temp\\arduino_build_302777/ESP32-CAM-Video-Telegram_9.5.ino.elf"
Sketch uses 1096981 bytes (34%) of program storage space. Maximum is 3145728 bytes.
Global variables use 65108 bytes (19%) of dynamic memory, leaving 262572 bytes for local variables. Maximum is 327680 bytes.
C:\ArduinoPortable\arduino-1.8.19\portable\packages\esp32\tools\esptool_py\3.3.0/esptool.exe --chip esp32 --port COM7 --baud 460800 --before default_reset --after hard_reset write_flash -z --flash_mode dio --flash_freq 80m --flash_size 4MB 0x1000 C:\Users\James\AppData\Local\Temp\arduino_build_302777/ESP32-CAM-Video-Telegram_9.5.ino.bootloader.bin 0x8000 C:\Users\James\AppData\Local\Temp\arduino_build_302777/ESP32-CAM-Video-Telegram_9.5.ino.partitions.bin 0xe000 C:\ArduinoPortable\arduino-1.8.19\portable\packages\esp32\hardware\esp32\2.0.4/tools/partitions/boot_app0.bin 0x10000 C:\Users\James\AppData\Local\Temp\arduino_build_302777/ESP32-CAM-Video-Telegram_9.5.ino.bin 

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
#include <WiFiManager.h>
WiFiManager wm;
#include "esp_camera.h"

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


static const char vernum[] = "pir-cam 9.5.1";
String devstr =  "deskpir";
int max_frames = 300;
framesize_t configframesize = FRAMESIZE_VGA; // FRAMESIZE_ + QVGA|CIF|VGA|SVGA|XGA|SXGA|UXGA
int frame_interval = 0;          // 0 = record at full speed, 100 = 100 ms delay between frames
float speed_up_factor = 0.5;          // 1 = play at realtime, 0.5 = slow motion, 10 = speedup 10x
int framesize = FRAMESIZE_VGA; //FRAMESIZE_HD;
int quality = 10;
int qualityconfig = 5;
char def_BOTtoken[50];
char def_timezone[50];
char def_chat_id[50];

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
char ssid[] = "ssid1234";     // your network SSID (name)
char password[] = "mrpeanut"; // your network key
// https://sites.google.com/a/usapiens.com/opnode/time-zones  -- find your timezone here
String timezone = "MST7MDT,M3.2.0/2:00:00,M11.1.0/2:00:00" ; //"GMT0BST,M3.5.0/01,M10.5.0/02";

// you can enter your home chat_id, so the device can send you a reboot message, otherwise it responds to the chat_id talking to telegram

//String chat_id = "1234567890";
String chat_id;
//#define BOTtoken "XXXXXXXXX:XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"  // your Bot Token (Get from Botfather)
String BOTtoken;

int MagicNumber = 33;

// see here for information about getting free telegram credentials
// https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot
// https://randomnerdtutorials.com/telegram-esp32-motion-detection-arduino/

bool reboot_request = false;
bool reset_request = false;

#define CAMERA_MODEL_AI_THINKER
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27

#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

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
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  //init with high specs to pre-allocate larger buffers
  if (psramFound()) {
    config.frame_size = configframesize;
    config.jpeg_quality = qualityconfig;
    config.fb_count = 4;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }

  //Serial.printf("Internal Total heap %d, internal Free Heap %d\n", ESP.getHeapSize(), ESP.getFreeHeap());
  //Serial.printf("SPIRam Total heap   %d, SPIRam Free Heap   %d\n", ESP.getPsramSize(), ESP.getFreePsram());

  static char * memtmp = (char *) malloc(32 * 1024);
  static char * memtmp2 = (char *) malloc(32 * 1024); //32767

  // camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return false;
  }
  free(memtmp2);
  memtmp2 = NULL;
  free(memtmp);
  memtmp = NULL;
  //Serial.printf("Internal Total heap %d, internal Free Heap %d\n", ESP.getHeapSize(), ESP.getFreeHeap());
  //Serial.printf("SPIRam Total heap   %d, SPIRam Free Heap   %d\n", ESP.getPsramSize(), ESP.getFreePsram());

  sensor_t * s = esp_camera_sensor_get();

  //  drop down frame size for higher initial frame rate
  s->set_framesize(s, (framesize_t)framesize);
  s->set_quality(s, quality);
  delay(200);
  return true;
}

#define FLASH_LED_PIN 4

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

int Bot_mtbs = 5000; //mean time between scan messages
long Bot_lasttime;   //last time messages' scan has been done
long TimePhoto_lasttime;   // last time we sent a timed photo
int TimePhoto_Minutes ;     // 2 minutes between photos

bool flashState = LOW;

camera_fb_t * fb = NULL;
camera_fb_t * vid_fb = NULL;

TaskHandle_t the_camera_loop_task;
void the_camera_loop (void* pvParameter) ;
static void IRAM_ATTR PIR_ISR(void* arg) ;

bool video_ready = false;
bool picture_ready = false;
bool active_interupt = false;
bool pir_enabled = false;
bool avi_enabled = false;
bool tim_enabled = false;

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

uint8_t * psram_avi_buf = NULL;
uint8_t * psram_idx_buf = NULL;
uint8_t * psram_avi_ptr = 0;
uint8_t * psram_idx_ptr = 0;
char strftime_buf[64];


void handleNewMessages(int numNewMessages) {
  //Serial.println("handleNewMessages");
  //Serial.println(String(numNewMessages));

  for (int i = 0; i < numNewMessages; i++) {
    
    // This line gets id of the person who sent the message .. it could be any telegram user
    // remove this line, and all messages get send to you
    
    // chat_id = String(bot.messages[i].chat_id);
    
    String text = bot.messages[i].text;
    Serial.printf("\nGot a message %s\n", text);

    String from_name = bot.messages[i].from_name;
    if (from_name == "") from_name = "Guest";

    String hi = "Got: ";
    hi += text;
    bot.sendMessage(chat_id, hi, "Markdown");
    client.setHandshakeTimeout(120000);
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
      String stat = "Device: " + devstr + "\nVer: " + String(vernum) + "\nRssi: " + String(WiFi.RSSI()) + "\nip: " +  WiFi.localIP().toString() + "\nPir Enabled: " + pir_enabled + "\nAvi Enabled: " + avi_enabled + "\nTim Enabled: " + tim_enabled;
      stat = stat + "\nInterval: " + frame_interval;
      stat = stat + "\nTimer: " + TimePhoto_Minutes;

      bot.sendMessage(chat_id, stat, "");
    }

    if (text == "/reboot") {
      reboot_request = true;
    }

    if (text == "/reset") {
      reset_request = true;
    }

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


    for (int j = 0; j < 4; j++) {
      camera_fb_t * newfb = esp_camera_fb_get();
      if (!newfb) {
        Serial.println("Camera Capture Failed");
      } else {
        //Serial.print("Pic, len="); Serial.print(newfb->len);
        //Serial.printf(", new fb %X\n", (long)newfb->buf);
        esp_camera_fb_return(newfb);
        delay(10);
      }
    }
    if ( text == "/photo" || text == "/caption" ) {

      fb = NULL;

      // Take Picture with Camera
      fb = esp_camera_fb_get();
      if (!fb) {
        Serial.println("Camera capture failed");
        bot.sendMessage(chat_id, "Camera capture failed", "");
        return;
      }

      currentByte = 0;
      fb_length = fb->len;
      fb_buffer = fb->buf;

      if (text == "/caption") {

        Serial.println("\n>>>>> Sending with a caption, bytes=  " + String(fb_length));

        String sent = bot.sendMultipartFormDataToTelegramWithCaption("sendPhoto", "photo", "img.jpg",
                      "image/jpeg", "Your photo", chat_id, fb_length,
                      isMoreDataAvailable, getNextByte, nullptr, nullptr);

        Serial.println("done!");

      } else {

        Serial.println("\n>>>>> Sending, bytes=  " + String(fb_length));

        bot.sendPhotoByBinary(chat_id, "image/jpeg", fb_length,
                              isMoreDataAvailable, getNextByte,
                              nullptr, nullptr);

        dataAvailable = true;

        Serial.println("done!");
      }
      esp_camera_fb_return(fb);
    }

    if (text == "/vga" ) {

      fb = NULL;

      //sensor_t * s = esp_camera_sensor_get();
      //s->set_framesize(s, FRAMESIZE_VGA);

      Serial.println("\n\n\nSending VGA");

      // Take Picture with Camera
      fb = esp_camera_fb_get();
      if (!fb) {
        Serial.println("Camera capture failed");
        bot.sendMessage(chat_id, "Camera capture failed", "");
        return;
      }

      currentByte = 0;
      fb_length = fb->len;
      fb_buffer = fb->buf;

      Serial.println("\n>>>>> Sending as 512 byte blocks, with jzdelay of 0, bytes=  " + String(fb_length));

      bot.sendPhotoByBinary(chat_id, "image/jpeg", fb_length,
                            isMoreDataAvailable, getNextByte,
                            nullptr, nullptr);

      esp_camera_fb_return(fb);
    }


    if (text == "/clip") {

      // record the video
      bot.longPoll =  0;

      xTaskCreatePinnedToCore( the_camera_loop, "the_camera_loop", 10000, NULL, 1, &the_camera_loop_task, 1);
      //xTaskCreatePinnedToCore( the_camera_loop, "the_camera_loop", 10000, NULL, 1, &the_camera_loop_task, 0);  //v8.5

      if ( the_camera_loop_task == NULL ) {
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
      welcome += "\n/status: status\n";
      welcome += "/start: start\n\n";
      welcome += "/reset: reset wifi params\n";
      welcome += "/reboot: reboot\n";
      welcome += "\n/freecoffee\n";
      //welcome += "\n https://ko-fi.com/jameszah/\n";
      bot.sendMessage(chat_id, welcome, "Markdown");
    }
  }
}

void saveParamCallback() {
  if (wm.server->hasArg("DevName")) {
    String sDevName  = wm.server->arg("DevName");
    devstr = sDevName;
    Serial.println(sDevName);
  }

  if (wm.server->hasArg("chat_id")) {
    String schat_id  = wm.server->arg("chat_id");
    Serial.println(schat_id);
    chat_id = schat_id;
  }
  if (wm.server->hasArg("BOTtoken")) {
    String sBOTtoken  = wm.server->arg("BOTtoken");
    Serial.println(sBOTtoken);
    BOTtoken = sBOTtoken;
    bot.updateToken(BOTtoken);

  }
  /* //do this in telegram, rather than wifiman
  if (wm.server->hasArg("CheckAvi")) {
    String sCheckAvi  = wm.server->arg("CheckAvi");
    Serial.println(sCheckAvi);
    if (sCheckAvi == "yes") {
      avi_enabled = true;
    }
  }
  if (wm.server->hasArg("CheckPir")) {
    String sCheckPir  = wm.server->arg("CheckPir");
    Serial.println(sCheckPir);
    if (sCheckPir == "yes") {
      pir_enabled = true;
    }
  }
  if (wm.server->hasArg("CheckTim")) {
    String sCheckTim  = wm.server->arg("CheckTim");
    Serial.println(sCheckTim);
    if (sCheckTim == "yes") {
      tim_enabled = true;
    }
  }
  if (wm.server->hasArg("avispped")) {
    String avispeed  = wm.server->arg("avispeed");
    Serial.println(avispeed);
    int int_avispeed = avispeed.toInt();
    if (int_avispeed == 0) {
      max_frames = 300;
      frame_interval = 0;
      speed_up_factor = 0.5;
    } else {
      max_frames = 300;
      frame_interval = int_avispeed;
      speed_up_factor = int_avispeed / 100;
    }
  }
  */
  do_eprom_write();

}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Make the avi functions
//
//   start_avi() - open the file and write headers
//   another_pic_avi() - write one more frame of movie
//   end_avi() - write the final parameters and close the file


char devname[30];

struct tm timeinfo;
time_t now;

camera_fb_t * fb_curr = NULL;
camera_fb_t * fb_next = NULL;

#define fbs 8 // how many kb of static ram for psram -> sram buffer for sd write - not really used because not dma for sd

char avi_file_name[100];
long avi_start_time = 0;
long avi_end_time = 0;
int start_record = 0;
long current_frame_time;
long last_frame_time;

static int i = 0;
uint16_t frame_cnt = 0;
uint16_t remnant = 0;
uint32_t length = 0;
uint32_t startms;
uint32_t elapsedms;
uint32_t uVideoLen = 0;

unsigned long movi_size = 0;
unsigned long jpeg_size = 0;
unsigned long idx_offset = 0;

uint8_t zero_buf[4] = {0x00, 0x00, 0x00, 0x00};
uint8_t dc_buf[4] = {0x30, 0x30, 0x64, 0x63};    // "00dc"
uint8_t avi1_buf[4] = {0x41, 0x56, 0x49, 0x31};    // "AVI1"
uint8_t idx1_buf[4] = {0x69, 0x64, 0x78, 0x31};    // "idx1"

struct frameSizeStruct {
  uint8_t frameWidth[2];
  uint8_t frameHeight[2];
};

//  data structure from here https://github.com/s60sc/ESP32-CAM_MJPEG2SD/blob/master/avi.cpp, extended for ov5640

static const frameSizeStruct frameSizeData[] = {
  {{0x60, 0x00}, {0x60, 0x00}}, // FRAMESIZE_96X96,    // 96x96
  {{0xA0, 0x00}, {0x78, 0x00}}, // FRAMESIZE_QQVGA,    // 160x120
  {{0xB0, 0x00}, {0x90, 0x00}}, // FRAMESIZE_QCIF,     // 176x144
  {{0xF0, 0x00}, {0xB0, 0x00}}, // FRAMESIZE_HQVGA,    // 240x176
  {{0xF0, 0x00}, {0xF0, 0x00}}, // FRAMESIZE_240X240,  // 240x240
  {{0x40, 0x01}, {0xF0, 0x00}}, // FRAMESIZE_QVGA,     // 320x240
  {{0x90, 0x01}, {0x28, 0x01}}, // FRAMESIZE_CIF,      // 400x296
  {{0xE0, 0x01}, {0x40, 0x01}}, // FRAMESIZE_HVGA,     // 480x320
  {{0x80, 0x02}, {0xE0, 0x01}}, // FRAMESIZE_VGA,      // 640x480   8
  {{0x20, 0x03}, {0x58, 0x02}}, // FRAMESIZE_SVGA,     // 800x600   9
  {{0x00, 0x04}, {0x00, 0x03}}, // FRAMESIZE_XGA,      // 1024x768  10
  {{0x00, 0x05}, {0xD0, 0x02}}, // FRAMESIZE_HD,       // 1280x720  11
  {{0x00, 0x05}, {0x00, 0x04}}, // FRAMESIZE_SXGA,     // 1280x1024 12
  {{0x40, 0x06}, {0xB0, 0x04}}, // FRAMESIZE_UXGA,     // 1600x1200 13
  // 3MP Sensors
  {{0x80, 0x07}, {0x38, 0x04}}, // FRAMESIZE_FHD,      // 1920x1080 14
  {{0xD0, 0x02}, {0x00, 0x05}}, // FRAMESIZE_P_HD,     //  720x1280 15
  {{0x60, 0x03}, {0x00, 0x06}}, // FRAMESIZE_P_3MP,    //  864x1536 16
  {{0x00, 0x08}, {0x00, 0x06}}, // FRAMESIZE_QXGA,     // 2048x1536 17
  // 5MP Sensors
  {{0x00, 0x0A}, {0xA0, 0x05}}, // FRAMESIZE_QHD,      // 2560x1440 18
  {{0x00, 0x0A}, {0x40, 0x06}}, // FRAMESIZE_WQXGA,    // 2560x1600 19
  {{0x38, 0x04}, {0x80, 0x07}}, // FRAMESIZE_P_FHD,    // 1080x1920 20
  {{0x00, 0x0A}, {0x80, 0x07}}  // FRAMESIZE_QSXGA,    // 2560x1920 21

};


#define AVIOFFSET 240 // AVI main header length

uint8_t buf[AVIOFFSET] = {
  0x52, 0x49, 0x46, 0x46, 0xD8, 0x01, 0x0E, 0x00, 0x41, 0x56, 0x49, 0x20, 0x4C, 0x49, 0x53, 0x54,
  0xD0, 0x00, 0x00, 0x00, 0x68, 0x64, 0x72, 0x6C, 0x61, 0x76, 0x69, 0x68, 0x38, 0x00, 0x00, 0x00,
  0xA0, 0x86, 0x01, 0x00, 0x80, 0x66, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00,
  0x64, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x80, 0x02, 0x00, 0x00, 0xe0, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4C, 0x49, 0x53, 0x54, 0x84, 0x00, 0x00, 0x00,
  0x73, 0x74, 0x72, 0x6C, 0x73, 0x74, 0x72, 0x68, 0x30, 0x00, 0x00, 0x00, 0x76, 0x69, 0x64, 0x73,
  0x4D, 0x4A, 0x50, 0x47, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x73, 0x74, 0x72, 0x66,
  0x28, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x80, 0x02, 0x00, 0x00, 0xe0, 0x01, 0x00, 0x00,
  0x01, 0x00, 0x18, 0x00, 0x4D, 0x4A, 0x50, 0x47, 0x00, 0x84, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x49, 0x4E, 0x46, 0x4F,
  0x10, 0x00, 0x00, 0x00, 0x6A, 0x61, 0x6D, 0x65, 0x73, 0x7A, 0x61, 0x68, 0x61, 0x72, 0x79, 0x20,
  0x76, 0x39, 0x35, 0x20, 0x4C, 0x49, 0x53, 0x54, 0x00, 0x01, 0x0E, 0x00, 0x6D, 0x6F, 0x76, 0x69,
};


//
// Writes an uint32_t in Big Endian at current file position
//
static void inline print_quartet(unsigned long i, uint8_t * fd) {
  uint8_t y[4];
  y[0] = i % 0x100;
  y[1] = (i >> 8) % 0x100;
  y[2] = (i >> 16) % 0x100;
  y[3] = (i >> 24) % 0x100;
  memcpy( fd, y, 4);
}

//
// Writes 2 uint32_t in Big Endian at current file position
//
static void inline print_2quartet(unsigned long i, unsigned long j, uint8_t * fd) {
  uint8_t y[8];
  y[0] = i % 0x100;
  y[1] = (i >> 8) % 0x100;
  y[2] = (i >> 16) % 0x100;
  y[3] = (i >> 24) % 0x100;
  y[4] = j % 0x100;
  y[5] = (j >> 8) % 0x100;
  y[6] = (j >> 16) % 0x100;
  y[7] = (j >> 24) % 0x100;
  memcpy( fd, y, 8);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//  get_good_jpeg()  - take a picture and make sure it has a good jpeg
//
camera_fb_t *  get_good_jpeg() {

  camera_fb_t * fb;

  long start;
  int failures = 0;

  do {
    int fblen = 0;
    int foundffd9 = 0;

    fb = esp_camera_fb_get();

    if (!fb) {
      Serial.println("Camera Capture Failed");
      failures++;
    } else {
      int get_fail = 0;
      fblen = fb->len;

      for (int j = 1; j <= 1025; j++) {
        if (fb->buf[fblen - j] != 0xD9) {
        } else {
          if (fb->buf[fblen - j - 1] == 0xFF ) {
            foundffd9 = 1;
            break;
          }
        }
      }

      if (!foundffd9) {
        Serial.printf("Bad jpeg, Frame %d, Len = %d \n", frame_cnt, fblen);
        esp_camera_fb_return(fb);
        failures++;
      } else {
        break;
      }
    }

  } while (failures < 10);   // normally leave the loop with a break()

  // if we get 10 bad frames in a row, then quality parameters are too high - set them lower
  if (failures == 10) {
    Serial.printf("10 failures");
    sensor_t * ss = esp_camera_sensor_get();
    int qual = ss->status.quality ;
    ss->set_quality(ss, qual + 3);
    quality = qual + 3;
    Serial.printf("\n\nDecreasing quality due to frame failures %d -> %d\n\n", qual, qual + 5);
    delay(1000);
  }
  return fb;
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// the_camera_loop()

void the_camera_loop (void* pvParameter) {

  vid_fb = get_good_jpeg(); // esp_camera_fb_get();
  if (!vid_fb) {
    Serial.println("Camera capture failed");
    //bot.sendMessage(chat_id, "Camera capture failed", "");
    return;
  }
  picture_ready = true;

  if (avi_enabled) {
    frame_cnt = 0;

    ///////////////////////////// start a movie
    avi_start_time = millis();
    Serial.printf("\nStart the avi ... at %d\n", avi_start_time);
    Serial.printf("Framesize %d, quality %d, length %d seconds\n\n", framesize, quality,  max_frames * frame_interval / 1000);

    fb_next = get_good_jpeg();                     // should take zero time
    last_frame_time = millis();
    start_avi();

    ///////////////////////////// all the frames of movie

    for (int j = 0; j < max_frames - 1 ; j++) { // max_frames
      current_frame_time = millis();

      if (current_frame_time - last_frame_time < frame_interval) {
        if (frame_cnt < 5 || frame_cnt > (max_frames - 5) )Serial.printf("frame %d, delay %d\n", frame_cnt, (int) frame_interval - (current_frame_time - last_frame_time));
        delay(frame_interval - (current_frame_time - last_frame_time));             // delay for timelapse
      }

      last_frame_time = millis();
      frame_cnt++;

      if (frame_cnt !=  1) esp_camera_fb_return(fb_curr);
      fb_curr = fb_next;           // we will write a frame, and get the camera preparing a new one

      another_save_avi(fb_curr );
      fb_next = get_good_jpeg();               // should take near zero, unless the sd is faster than the camera, when we will have to wait for the camera

      digitalWrite(33, frame_cnt % 2);
      if (movi_size > avi_buf_size * .95) break;
    }

    ///////////////////////////// stop a movie
    Serial.println("End the Avi");

    esp_camera_fb_return(fb_curr);
    frame_cnt++;
    fb_curr = fb_next;
    fb_next = NULL;
    another_save_avi(fb_curr );
    digitalWrite(33, frame_cnt % 2);
    esp_camera_fb_return(fb_curr);
    fb_curr = NULL;
    end_avi();                                // end the movie
    digitalWrite(33, HIGH);          // light off
    avi_end_time = millis();
    float fps = 1.0 * frame_cnt / ((avi_end_time - avi_start_time) / 1000) ;
    Serial.printf("End the avi at %d.  It was %d frames, %d ms at %.2f fps...\n", millis(), frame_cnt, avi_end_time - avi_start_time, fps);
    frame_cnt = 0;             // start recording again on the next loop
    video_ready = true;
  }
  Serial.println("Deleting the camera task");
  delay(100);
  vTaskDelete(the_camera_loop_task);
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// start_avi - open the files and write in headers
//

void start_avi() {

  Serial.println("Starting an avi ");

  time(&now);
  localtime_r(&now, &timeinfo);
  strftime(strftime_buf, sizeof(strftime_buf), "DoorCam %F %H.%M.%S.avi", &timeinfo);

  //memset(psram_avi_buf, 0, avi_buf_size);  // save some time
  //memset(psram_idx_buf, 0, idx_buf_size);

  psram_avi_ptr = 0;
  psram_idx_ptr = 0;

  memcpy(buf + 0x40, frameSizeData[framesize].frameWidth, 2);
  memcpy(buf + 0xA8, frameSizeData[framesize].frameWidth, 2);
  memcpy(buf + 0x44, frameSizeData[framesize].frameHeight, 2);
  memcpy(buf + 0xAC, frameSizeData[framesize].frameHeight, 2);

  psram_avi_ptr = psram_avi_buf;
  psram_idx_ptr = psram_idx_buf;

  memcpy( psram_avi_ptr, buf, AVIOFFSET);
  psram_avi_ptr += AVIOFFSET;

  startms = millis();

  jpeg_size = 0;
  movi_size = 0;
  uVideoLen = 0;
  idx_offset = 4;

} // end of start avi

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//  another_save_avi saves another frame to the avi file, uodates index
//           -- pass in a fb pointer to the frame to add
//

void another_save_avi(camera_fb_t * fb ) {

  int fblen;
  fblen = fb->len;

  int fb_block_length;
  uint8_t* fb_block_start;

  jpeg_size = fblen;

  remnant = (4 - (jpeg_size & 0x00000003)) & 0x00000003;

  long bw = millis();
  long frame_write_start = millis();

  memcpy(psram_avi_ptr, dc_buf, 4);

  int jpeg_size_rem = jpeg_size + remnant;

  print_quartet(jpeg_size_rem, psram_avi_ptr + 4);

  fb_block_start = fb->buf;

  if (fblen > fbs * 1024 - 8 ) {                     // fbs is the size of frame buffer static
    fb_block_length = fbs * 1024;
    fblen = fblen - (fbs * 1024 - 8);
    memcpy( psram_avi_ptr + 8, fb_block_start, fb_block_length - 8);
    fb_block_start = fb_block_start + fb_block_length - 8;
  } else {
    fb_block_length = fblen + 8  + remnant;
    memcpy( psram_avi_ptr + 8, fb_block_start, fb_block_length - 8);
    fblen = 0;
  }

  psram_avi_ptr += fb_block_length;

  while (fblen > 0) {
    if (fblen > fbs * 1024) {
      fb_block_length = fbs * 1024;
      fblen = fblen - fb_block_length;
    } else {
      fb_block_length = fblen  + remnant;
      fblen = 0;
    }

    memcpy( psram_avi_ptr, fb_block_start, fb_block_length);

    psram_avi_ptr += fb_block_length;

    fb_block_start = fb_block_start + fb_block_length;
  }

  movi_size += jpeg_size;
  uVideoLen += jpeg_size;

  print_2quartet(idx_offset, jpeg_size, psram_idx_ptr);
  psram_idx_ptr += 8;

  idx_offset = idx_offset + jpeg_size + remnant + 8;

  movi_size = movi_size + remnant;

} // end of another_pic_avi

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//  end_avi writes the index, and closes the files
//

void end_avi() {

  Serial.println("End of avi - closing the files");

  if (frame_cnt <  5 ) {
    Serial.println("Recording screwed up, less than 5 frames, forget index\n");
  } else {

    elapsedms = millis() - startms;

    float fRealFPS = (1000.0f * (float)frame_cnt) / ((float)elapsedms) * speed_up_factor;

    float fmicroseconds_per_frame = 1000000.0f / fRealFPS;
    uint8_t iAttainedFPS = round(fRealFPS) ;
    uint32_t us_per_frame = round(fmicroseconds_per_frame);

    //Modify the MJPEG header from the beginning of the file, overwriting various placeholders

    print_quartet(movi_size + 240 + 16 * frame_cnt + 8 * frame_cnt, psram_avi_buf + 4);
    print_quartet(us_per_frame, psram_avi_buf + 0x20);

    unsigned long max_bytes_per_sec = (1.0f * movi_size * iAttainedFPS) / frame_cnt;
    print_quartet(max_bytes_per_sec, psram_avi_buf + 0x24);
    print_quartet(frame_cnt, psram_avi_buf + 0x30);
    print_quartet(frame_cnt, psram_avi_buf + 0x8c);
    print_quartet((int)iAttainedFPS, psram_avi_buf + 0x84);
    print_quartet(movi_size + frame_cnt * 8 + 4, psram_avi_buf + 0xe8);

    Serial.println(F("\n*** Video recorded and saved ***\n"));

    Serial.printf("Recorded %5d frames in %5d seconds\n", frame_cnt, elapsedms / 1000);
    Serial.printf("File size is %u bytes\n", movi_size + 12 * frame_cnt + 4);
    Serial.printf("Adjusted FPS is %5.2f\n", fRealFPS);
    Serial.printf("Max data rate is %lu bytes/s\n", max_bytes_per_sec);
    Serial.printf("Frame duration is %d us\n", us_per_frame);
    Serial.printf("Average frame length is %d bytes\n", uVideoLen / frame_cnt);


    Serial.printf("Writng the index, %d frames\n", frame_cnt);

    memcpy (psram_avi_ptr, idx1_buf, 4);
    psram_avi_ptr += 4;

    print_quartet(frame_cnt * 16, psram_avi_ptr);
    psram_avi_ptr += 4;

    psram_idx_ptr = psram_idx_buf;

    for (int i = 0; i < frame_cnt; i++) {
      memcpy (psram_avi_ptr, dc_buf, 4);
      psram_avi_ptr += 4;
      memcpy (psram_avi_ptr, zero_buf, 4);
      psram_avi_ptr += 4;

      memcpy (psram_avi_ptr, psram_idx_ptr, 8);
      psram_avi_ptr += 8;
      psram_idx_ptr += 8;
    }
  }

  Serial.println("---");
  digitalWrite(33, HIGH);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// setup some interupts during reboot
//
//  int read13 = digitalRead(13); -- pir for video

int PIRpin = 13;

static void setupinterrupts() {

  pinMode(PIRpin, INPUT_PULLDOWN) ; //INPUT_PULLDOWN);

  Serial.print("Setup PIRpin = ");
  for (int i = 0; i < 5; i++) {
    Serial.print( digitalRead(PIRpin) ); Serial.print(", ");
  }
  Serial.println(" ");

  esp_err_t err = gpio_isr_handler_add((gpio_num_t)PIRpin, &PIR_ISR, NULL);

  if (err != ESP_OK) Serial.printf("gpio_isr_handler_add failed (%x)", err);
  gpio_set_intr_type((gpio_num_t)PIRpin, GPIO_INTR_POSEDGE);


}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//  PIR_ISR - interupt handler for PIR  - starts or extends a video
//
static void IRAM_ATTR PIR_ISR(void* arg) {

  int PIRstatus = digitalRead(PIRpin) + digitalRead(PIRpin) + digitalRead(PIRpin) ;
  if (PIRstatus == 3) {
    //Serial.print("PIR Interupt>> "); Serial.println(PIRstatus);

    if (!active_interupt && pir_enabled) {
      active_interupt = true;
      digitalWrite(33, HIGH);
      //Serial.print("PIR Interupt ... start recording ... ");
      xTaskCreatePinnedToCore( the_camera_loop, "the_camera_loop", 10000, NULL, 1, &the_camera_loop_task, 1);
      //xTaskCreatePinnedToCore( the_camera_loop, "the_camera_loop", 10000, NULL, 1, &the_camera_loop_task, 0);  //v8.5

      if ( the_camera_loop_task == NULL ) {
        Serial.printf("do_the_steaming_task failed to start! %d\n", the_camera_loop_task);
      }
    }
  }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//  eprom functions  
//

#include <EEPROM.h>

struct eprom_data {
  bool checkpir;
  bool checkavi;
  bool checktim;
  int  max_frames ;
  int  frame_interval ;
  float speed_up_factor ;
  int timmin;
  char devname[16];
  char chat_id[16];
  char BOTtoken[52];
  char timzon[52];

  int eprom_good;
};

void do_eprom_read() {

  eprom_data ed;

  EEPROM.begin(200);
  EEPROM.get(0, ed);

  if (ed.eprom_good == MagicNumber) {
    Serial.println("Good settings in the EPROM ");

    /*
    Serial.println(ed.devname);
    Serial.println(ed.BOTtoken);
    Serial.println(ed.chat_id);
    Serial.println(ed.timzon);
    Serial.println(ed.timmin);
    */
    
    devstr = ed.devname;
    devstr.toCharArray(devname, 12); //devstr.length() + 1);
    BOTtoken = ed.BOTtoken;
    bot.updateToken(BOTtoken);
    chat_id = ed.chat_id;
    timezone = ed.timzon;
    pir_enabled = ed.checkpir;
    avi_enabled = ed.checkavi;
    tim_enabled = ed.checktim;
    TimePhoto_Minutes = ed.timmin;
    max_frames = ed.max_frames;
    frame_interval = ed.frame_interval ;
    speed_up_factor = ed.speed_up_factor;

    /* 
    Serial.println(devstr);
    Serial.println(BOTtoken);
    Serial.println(chat_id);
    Serial.println(timezone);
    Serial.println(TimePhoto_Minutes);
    */

  } else {
    Serial.println("No settings in EPROM ");

    chat_id = "1234567890";
    BOTtoken = "123456789:12345678901234567890123456789012345";
    timezone = "GMT";
    devstr = "deskpir";

    pir_enabled = 1; 
    avi_enabled = 1;  
    tim_enabled = 1;
    TimePhoto_Minutes = 30 ;
    max_frames = 300 ; 
    frame_interval = 125; 
    speed_up_factor = 1; 

    do_eprom_write();
    wm.resetSettings();
  }
}

void do_eprom_write() {

  eprom_data ed;
  ed.eprom_good = MagicNumber;

  devstr.toCharArray(ed.devname, 12); //devstr.length() + 1);
  BOTtoken.toCharArray(ed.BOTtoken, 50); //BOTtoken.length() + 1);
  chat_id.toCharArray(ed.chat_id, 12); //chat_id.length() + 1);
  timezone.toCharArray(ed.timzon, 50); //timezone.length() + 1);

  ed.checkpir =  pir_enabled  ;
  ed.checkavi = avi_enabled  ;
  ed.checktim = tim_enabled  ;
  ed.timmin = TimePhoto_Minutes;
  ed.max_frames = max_frames;
  ed.frame_interval = frame_interval ;
  ed.speed_up_factor = speed_up_factor;

  Serial.println("Writing to EPROM ...");

  /*
  Serial.println(ed.devname);
  Serial.println(ed.BOTtoken);
  Serial.println(ed.chat_id);
  Serial.println(ed.timzon);
  Serial.println(ed.timmin);
  */

  EEPROM.begin(200);
  EEPROM.put(0, ed);
  EEPROM.commit();
  EEPROM.end();
}

#include "esp_wifi.h"
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#include <ESPmDNS.h>

bool init_wifi() {
  uint32_t brown_reg_temp = READ_PERI_REG(RTC_CNTL_BROWN_OUT_REG);
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);

  do_eprom_read();

  devstr.toCharArray(devname, devstr.length() + 1);        // name of your camera for mDNS, Router, and filenames
  BOTtoken.toCharArray(def_BOTtoken, BOTtoken.length() + 1);
  chat_id.toCharArray(def_chat_id, chat_id.length() + 1);
  timezone.toCharArray(def_timezone, timezone.length() + 1);

  WiFiManagerParameter dev("DevName", "Name of Device", devname, 12);
  wm.addParameter(&dev);

  WiFiManagerParameter id("chat_id", "Telegram ID", def_chat_id, 15);
  wm.addParameter(&id);
  WiFiManagerParameter pass ("BOTtoken", "Telegram Pass", def_BOTtoken, 50);
  wm.addParameter(&pass);
  WiFiManagerParameter timzon ("timzon", "Time Zone", def_timezone, 50);
  wm.addParameter(&timzon);

  wm.setSaveParamsCallback(saveParamCallback);

  std::vector<const char *> menu = {"wifi", "info", "sep", "restart", "exit"};
  wm.setMenu(menu);

  // set dark theme
  wm.setClass("invert");

  bool res;
  //wm.resetSettings();  // for debugging

  wm.setConnectTimeout(60*5); // how long to try to connect for before continuing
  wm.setConfigPortalTimeout(60*5); // auto close configportal after n seconds

  // res = wm.autoConnect(); // auto generated AP name from chipid

  res = wm.autoConnect(devname); // use the devname defined above, with no password
  //res = wm.autoConnect("AutoConnectAP","password"); // password protected ap

  if (res) {
    Serial.println("Succesful Connection using WiFiManager");
    do_eprom_write();
  } else {

    //InternetFailed = true;
    Serial.println("Internet failed using WiFiManager - not starting Web services");
  }


  wifi_ps_type_t the_type;

  //esp_err_t get_ps = esp_wifi_get_ps(&the_type);
  esp_err_t set_ps = esp_wifi_set_ps(WIFI_PS_NONE);

  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, brown_reg_temp);

  configTime(0, 0, "pool.ntp.org");
  char tzchar[80];
  timezone.toCharArray(tzchar, timezone.length());          // name of your camera for mDNS, Router, and filenames
  setenv("TZ", tzchar, 1);  // mountain time zone from #define at top
  tzset();

  if (!MDNS.begin(devname)) {
    Serial.println("Error setting up MDNS responder!");
    return false;
  } else {
    Serial.printf("mDNS responder started '%s'\n", devname);
  }
  time(&now);

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  return true;
}


//////////////////////////////////////////////////////////////////////////////////////

void setup() {
  Serial.begin(115200);
  Serial.println("---------------------------------");
  Serial.printf("ESP32-CAM Video-Telegram %s\n", vernum);
  Serial.println("---------------------------------");

  pinMode(FLASH_LED_PIN, OUTPUT);
  digitalWrite(FLASH_LED_PIN, flashState); //defaults to low

  pinMode(12, INPUT_PULLUP);        // pull this down to stop recording

  pinMode(33, OUTPUT);             // little red led on back of chip
  digitalWrite(33, LOW);           // turn on the red LED on the back of chip

  //Serial.printf("Internal Total heap %d, internal Free Heap %d\n", ESP.getHeapSize(), ESP.getFreeHeap());
  //Serial.printf("SPIRam Total heap   %d, SPIRam Free Heap   %d\n", ESP.getPsramSize(), ESP.getFreePsram());

  int avail_psram = ESP.getFreePsram();
  Serial.print("PSRAM size to store the video "); Serial.println(avail_psram);

  idx_buf_size = max_frames * 10 + 20;

  //vga avi_buf_size = avail_psram - 500 * 1024; 
  avi_buf_size = avail_psram - 500 * 1024; //900 for hd, 500 for vga
  Serial.print("try to allocate "); Serial.println(avi_buf_size);

  psram_avi_buf = (uint8_t*)ps_malloc(avi_buf_size);
  if (psram_avi_buf == 0) Serial.printf("psram_avi allocation failed\n");
  psram_idx_buf = (uint8_t*)ps_malloc(idx_buf_size); // save file in psram
  if (psram_idx_buf == 0) Serial.printf("psram_idx allocation failed\n");

  if (!setupCamera()) {
    Serial.println("Camera Setup Failed!");
    while (true) {
      delay(100);
    }
  }

  for (int j = 0; j < 7; j++) {
    camera_fb_t * fb = esp_camera_fb_get();
    if (!fb) {
      Serial.println("Camera Capture Failed");
    } else {
      Serial.print("Pic, len="); Serial.print(fb->len);
      Serial.printf(", new fb %X\n", (long)fb->buf);
      esp_camera_fb_return(fb);
      delay(50);
    }
  }

  bool wifi_status = init_wifi();

  // Make the bot wait for a new message for up to 60seconds
  //bot.longPoll = 60;
  bot.longPoll = 5;

  client.setInsecure();

  setupinterrupts();

  Serial.print("Sending a message to: ");
  Serial.print(chat_id);
  Serial.print(" at ");
  Serial.println(BOTtoken);

  String stat = "Reboot\nDevice: " + devstr + "\nVer: " + String(vernum) + "\nRssi: " + String(WiFi.RSSI()) + "\nip: " +  WiFi.localIP().toString() + "\n/start";
  Serial.println(stat);

  bot.sendMessage(chat_id, stat, "");

  digitalWrite(33, HIGH);
}

int loopcount = 0;


void loop() {
  loopcount++;

  client.setHandshakeTimeout(120000); // workaround for esp32-arduino 2.02 bug https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot/issues/270#issuecomment-1003795884

  if (reboot_request) {
    String stat = "Rebooting on request\nDevice: " + devstr + "\nVer: " + String(vernum) + "\nRssi: " + String(WiFi.RSSI()) + "\nip: " +  WiFi.localIP().toString() ;
    bot.sendMessage(chat_id, stat, "");
    delay(10000);
    ESP.restart();
  }

  if (reset_request) {
    wm.resetSettings();
    reset_request = false;
  }

  if (picture_ready) {
    picture_ready = false;
    send_the_picture();
  }

  if (video_ready) {
    video_ready = false;
    send_the_video();
  }

  if (millis() > Bot_lasttime + Bot_mtbs )  {

    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("***** WiFi reconnect *****");
      WiFi.reconnect();
      delay(5000);
      if (WiFi.status() != WL_CONNECTED) {
        Serial.println("***** WiFi rerestart *****");
        init_wifi();
      }
    }

    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while (numNewMessages) {
      //Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    Bot_lasttime = millis();
  }


  if (millis() > (TimePhoto_lasttime + TimePhoto_Minutes * 60000) ) {
    if (tim_enabled) {
      for (int j = 0; j < 4; j++) {
        camera_fb_t * newfb = esp_camera_fb_get();
        if (!newfb) {
          Serial.println("Camera Capture Failed");
        } else {
          //Serial.print("Pic, len="); Serial.print(newfb->len);
          //Serial.printf(", new fb %X\n", (long)newfb->buf);
          esp_camera_fb_return(newfb);
          delay(10);
        }
      }

      camera_fb_t * tfb = esp_camera_fb_get();
      if (!tfb) {
        Serial.println("Camera Capture Failed");
      } else {


        currentByte = 0;
        fb_length = tfb->len;
        fb_buffer = tfb->buf;

        Serial.println("\n>>>>> Sending timed photo, bytes=  " + String(fb_length));

        String sent = bot.sendMultipartFormDataToTelegramWithCaption("sendPhoto", "photo", "img.jpg",
                      "image/jpeg", "Timed Photo", chat_id, fb_length,
                      isMoreDataAvailable, getNextByte, nullptr, nullptr);

        Serial.println("done!");
      }
      esp_camera_fb_return(tfb);
      delay(10);
    }
    TimePhoto_lasttime = millis();
  }
}


void send_the_picture() {
  digitalWrite(33, LOW);          // light on
  currentByte = 0;
  fb_length = vid_fb->len;
  fb_buffer = vid_fb->buf;

  Serial.println("\n>>>>> Sending as 512 byte blocks, with jzdelay of 0, bytes=  " + String(fb_length));

  if (active_interupt) {
    String sent = bot.sendMultipartFormDataToTelegramWithCaption("sendPhoto", "photo", "img.jpg",
                  "image/jpeg", "PIR Event!", chat_id, fb_length,
                  isMoreDataAvailable, getNextByte, nullptr, nullptr);
  } else {
    String sent = bot.sendMultipartFormDataToTelegramWithCaption("sendPhoto", "photo", "img.jpg",
                  "image/jpeg", "Telegram Request", chat_id, fb_length,
                  isMoreDataAvailable, getNextByte, nullptr, nullptr);
  }
  esp_camera_fb_return(vid_fb);
  bot.longPoll =  0;
  digitalWrite(33, HIGH);          // light oFF
  if (!avi_enabled) active_interupt = false;
}

void send_the_video() {
  digitalWrite(33, LOW);          // light on
  Serial.println("\n\n\nSending clip with caption");
  Serial.println("\n>>>>> Sending as 512 byte blocks, with a caption, and with jzdelay of 0, bytes=  " + String(psram_avi_ptr - psram_avi_buf));
  avi_buf = psram_avi_buf;

  avi_ptr = 0;
  avi_len = psram_avi_ptr - psram_avi_buf;

  String sent2 = bot.sendMultipartFormDataToTelegramWithCaption("sendDocument", "document", strftime_buf,
                 "image/jpeg", "Intruder alert!", chat_id, psram_avi_ptr - psram_avi_buf,
                 avi_more, avi_next, nullptr, nullptr);

  Serial.println("done!");
  digitalWrite(33, HIGH);          // light off

  bot.longPoll = 5;
  active_interupt = false;
}
