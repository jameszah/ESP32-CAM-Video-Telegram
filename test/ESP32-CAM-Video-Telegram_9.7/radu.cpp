#include "esp_camera.h"
#include <Arduino.h>
/*

 Code from raduprv at github for low-light use of the ov2640 sensor
 copied Oct 27, 2022

 It configured the registers of the camera to keep the sensor active 
 or the shutter open, for much longer time to collect light and display
 a nearly dark image -- the time to take the picture goes from 80 ms (HD)
 to 500 or 5000 ms for a very dark scene.
 
https://github.com/raduprv/esp32-cam_ov2640-timelapse
https://github.com/raduprv/esp32-cam_ov2640-timelapse/blob/main/LICENSE

The exposure is done manually based on the light level, and the image 
quality is far better than the default settings.

*/

extern int framesize;

void radu() {

  camera_fb_t * fb = NULL;
  sensor_t * s = esp_camera_sensor_get();
  int light = 0;
  int day_switch_value = 140;

  int y;

  Serial.println("Before the radu");
  y = millis();
  fb = esp_camera_fb_get();
  esp_camera_fb_return(fb);
  Serial.printf("Get took %d ms\n",millis()-y);

  y = millis();
  fb = esp_camera_fb_get();
  esp_camera_fb_return(fb);
  Serial.printf("Get took %d ms\n",millis()-y);

  y = millis();
  fb = esp_camera_fb_get();
  esp_camera_fb_return(fb);
  Serial.printf("Get took %d ms\n",millis()-y);

  y = millis();
  fb = esp_camera_fb_get();
  esp_camera_fb_return(fb);
  Serial.printf("Get took %d ms\n",millis()-y);


  s->set_whitebal(s, 1);       // 0 = disable , 1 = enable
  s->set_awb_gain(s, 1);       // 0 = disable , 1 = enable
  s->set_wb_mode(s, 2);        // 0 to 4 - if awb_gain enabled (0 - Auto, 1 - Sunny, 2 - Cloudy, 3 - Office, 4 - Home)
  //  s->set_exposure_ctrl(s, 1);  // 0 = disable , 1 = enable
  // s->set_aec2(s, 0);           // 0 = disable , 1 = enable
  //s->set_ae_level(s, 2);       // -2 to 2
  //s->set_aec_value(s, 1200);    // 0 to 1200
  s->set_gain_ctrl(s, 0);      // 0 = disable , 1 = enable
  s->set_agc_gain(s, 0);       // 0 to 30
  s->set_gainceiling(s, (gainceiling_t)6);  // 0 to 6
  s->set_bpc(s, 1);            // 0 = disable , 1 = enable
  s->set_wpc(s, 1);            // 0 = disable , 1 = enable
  s->set_raw_gma(s, 1);        // 0 = disable , 1 = enable
  s->set_lenc(s, 0);           // 0 = disable , 1 = enable
  s->set_hmirror(s, 0);        // 0 = disable , 1 = enable
  s->set_vflip(s, 0);          // 0 = disable , 1 = enable
  s->set_dcw(s, 0);            // 0 = disable , 1 = enable
  s->set_colorbar(s, 0);       // 0 = disable , 1 = enable
  s->set_framesize(s, (framesize_t)framesize);           // framesize gets reset somewhere

  s->set_reg(s, 0xff, 0xff, 0x01); //banksel

  light = s->get_reg(s, 0x2f, 0xff);
  Serial.print("First light is ");
  Serial.println(light);
  Serial.print("Old 0x0 value is ");
  Serial.println(s->get_reg(s, 0x0, 0xff));

  //light=120+cur_pic*10;
  //light=0+cur_pic*5;

  if (light < day_switch_value)
  {
    //here we are in night mode
    if (light < 45)s->set_reg(s, 0x11, 0xff, 1); //frame rate (1 means longer exposure)
    s->set_reg(s, 0x13, 0xff, 0); //manual everything
    s->set_reg(s, 0x0c, 0x6, 0x8); //manual banding

    s->set_reg(s, 0x45, 0x3f, 0x3f); //really long exposure (but it doesn't really work)
  }
  else
  {
    //here we are in daylight mode

    s->set_reg(s, 0x2d, 0xff, 0x0); //extra lines
    s->set_reg(s, 0x2e, 0xff, 0x0); //extra lines

    s->set_reg(s, 0x47, 0xff, 0x0); //Frame Length Adjustment MSBs

    if (light < 150)
    {
      s->set_reg(s, 0x46, 0xff, 0xd0); //Frame Length Adjustment LSBs
      s->set_reg(s, 0x2a, 0xff, 0xff); //line adjust MSB
      s->set_reg(s, 0x2b, 0xff, 0xff); //line adjust
      s->set_reg(s, 0x45, 0xff, 0xff); //exposure (doesn't seem to work)
    }
    else if (light < 160)
    {
      s->set_reg(s, 0x46, 0xff, 0xc0); //Frame Length Adjustment LSBs
      s->set_reg(s, 0x2a, 0xff, 0xb0); //line adjust MSB
      s->set_reg(s, 0x2b, 0xff, 0xff); //line adjust
      s->set_reg(s, 0x45, 0xff, 0x10); //exposure (doesn't seem to work)
    }
    else if (light < 170)
    {
      s->set_reg(s, 0x46, 0xff, 0xb0); //Frame Length Adjustment LSBs
      s->set_reg(s, 0x2a, 0xff, 0x80); //line adjust MSB
      s->set_reg(s, 0x2b, 0xff, 0xff); //line adjust
      s->set_reg(s, 0x45, 0xff, 0x10); //exposure (doesn't seem to work)
    }
    else if (light < 180)
    {
      s->set_reg(s, 0x46, 0xff, 0xa8); //Frame Length Adjustment LSBs
      s->set_reg(s, 0x2a, 0xff, 0x80); //line adjust MSB
      s->set_reg(s, 0x2b, 0xff, 0xff); //line adjust
      s->set_reg(s, 0x45, 0xff, 0x10); //exposure (doesn't seem to work)
    }
    else if (light < 190)
    {
      s->set_reg(s, 0x46, 0xff, 0xa6); //Frame Length Adjustment LSBs
      s->set_reg(s, 0x2a, 0xff, 0x80); //line adjust MSB
      s->set_reg(s, 0x2b, 0xff, 0xff); //line adjust
      s->set_reg(s, 0x45, 0xff, 0x90); //exposure (doesn't seem to work)
    }
    else if (light < 200)
    {
      s->set_reg(s, 0x46, 0xff, 0xa4); //Frame Length Adjustment LSBs
      s->set_reg(s, 0x2a, 0xff, 0x80); //line adjust MSB
      s->set_reg(s, 0x2b, 0xff, 0xff); //line adjust
      s->set_reg(s, 0x45, 0xff, 0x10); //exposure (doesn't seem to work)
    }
    else if (light < 210)
    {
      s->set_reg(s, 0x46, 0xff, 0x98); //Frame Length Adjustment LSBs
      s->set_reg(s, 0x2a, 0xff, 0x60); //line adjust MSB
      s->set_reg(s, 0x2b, 0xff, 0xff); //line adjust
      s->set_reg(s, 0x45, 0xff, 0x10); //exposure (doesn't seem to work)
    }
    else if (light < 220)
    {
      s->set_reg(s, 0x46, 0xff, 0x80); //Frame Length Adjustment LSBs
      s->set_reg(s, 0x2a, 0xff, 0x20); //line adjust MSB
      s->set_reg(s, 0x2b, 0xff, 0xff); //line adjust
      s->set_reg(s, 0x45, 0xff, 0x10); //exposure (doesn't seem to work)
    }
    else if (light < 230)
    {
      s->set_reg(s, 0x46, 0xff, 0x70); //Frame Length Adjustment LSBs
      s->set_reg(s, 0x2a, 0xff, 0x20); //line adjust MSB
      s->set_reg(s, 0x2b, 0xff, 0xff); //line adjust
      s->set_reg(s, 0x45, 0xff, 0x10); //exposure (doesn't seem to work)
    }
    else if (light < 240)
    {
      s->set_reg(s, 0x46, 0xff, 0x60); //Frame Length Adjustment LSBs
      s->set_reg(s, 0x2a, 0xff, 0x20); //line adjust MSB
      s->set_reg(s, 0x2b, 0xff, 0x80); //line adjust
      s->set_reg(s, 0x45, 0xff, 0x10); //exposure (doesn't seem to work)
    }
    else if (light < 253)
    {
      s->set_reg(s, 0x46, 0xff, 0x10); //Frame Length Adjustment LSBs
      s->set_reg(s, 0x2a, 0xff, 0x0); //line adjust MSB
      s->set_reg(s, 0x2b, 0xff, 0x40); //line adjust
      s->set_reg(s, 0x45, 0xff, 0x10); //exposure (doesn't seem to work)
    }
    else
    {
      s->set_reg(s, 0x46, 0xff, 0x0); //Frame Length Adjustment LSBs
      s->set_reg(s, 0x2a, 0xff, 0x0); //line adjust MSB
      s->set_reg(s, 0x2b, 0xff, 0x0); //line adjust
      s->set_reg(s, 0x45, 0xff, 0x0); //exposure (doesn't seem to work)
      s->set_reg(s, 0x10, 0xff, 0x0); //exposure (doesn't seem to work)
    }

    s->set_reg(s, 0x0f, 0xff, 0x4b); //no idea
    s->set_reg(s, 0x03, 0xff, 0xcf); //no idea
    s->set_reg(s, 0x3d, 0xff, 0x34); //changes the exposure somehow, has to do with frame rate

    s->set_reg(s, 0x11, 0xff, 0x0); //frame rate
    s->set_reg(s, 0x43, 0xff, 0x11); //11 is the default value
  }

  //Serial.println("Getting first frame at");
  //Serial.println(millis());
  
  Serial.println("mid radu");
  y = millis();
  fb = esp_camera_fb_get();
  //skip_frame();
  Serial.printf("Get took %d ms\n",millis()-y);
  esp_camera_fb_return(fb);
  
  //Serial.println("Got first frame at");
  //Serial.println(millis());

  if (light == 0)
  {
    s->set_reg(s, 0x47, 0xff, 0x40); //Frame Length Adjustment MSBs
    s->set_reg(s, 0x2a, 0xf0, 0xf0); //line adjust MSB
    s->set_reg(s, 0x2b, 0xff, 0xff); //line adjust
  }
  else if (light == 1)
  {
    s->set_reg(s, 0x47, 0xff, 0x40); //Frame Length Adjustment MSBs
    s->set_reg(s, 0x2a, 0xf0, 0xd0); //line adjust MSB
    s->set_reg(s, 0x2b, 0xff, 0xff); //line adjust
  }
  else if (light == 2)
  {
    s->set_reg(s, 0x47, 0xff, 0x40); //Frame Length Adjustment MSBs
    s->set_reg(s, 0x2a, 0xf0, 0xb0); //line adjust MSB
    s->set_reg(s, 0x2b, 0xff, 0xff); //line adjust
  }
  else if (light == 3)
  {
    s->set_reg(s, 0x47, 0xff, 0x40); //Frame Length Adjustment MSBs
    s->set_reg(s, 0x2a, 0xf0, 0x70); //line adjust MSB
    s->set_reg(s, 0x2b, 0xff, 0xff); //line adjust
  }
  else if (light == 4)
  {
    s->set_reg(s, 0x47, 0xff, 0x40); //Frame Length Adjustment MSBs
    s->set_reg(s, 0x2a, 0xf0, 0x40); //line adjust MSB
    s->set_reg(s, 0x2b, 0xff, 0xff); //line adjust
  }
  else if (light == 5)
  {
    s->set_reg(s, 0x47, 0xff, 0x20); //Frame Length Adjustment MSBs
    s->set_reg(s, 0x2a, 0xf0, 0x80); //line adjust MSB
    s->set_reg(s, 0x2b, 0xff, 0xff); //line adjust
  }
  else if (light == 6)
  {
    s->set_reg(s, 0x47, 0xff, 0x20); //Frame Length Adjustment MSBs
    s->set_reg(s, 0x2a, 0xf0, 0x40); //line adjust MSB
    s->set_reg(s, 0x2b, 0xff, 0xff); //line adjust
  }
  else if (light == 7)
  {
    s->set_reg(s, 0x47, 0xff, 0x20); //Frame Length Adjustment MSBs
    s->set_reg(s, 0x2a, 0xf0, 0x30); //line adjust MSB
    s->set_reg(s, 0x2b, 0xff, 0xff); //line adjust
  }
  else if (light == 8)
  {
    s->set_reg(s, 0x47, 0xff, 0x20); //Frame Length Adjustment MSBs
    s->set_reg(s, 0x2a, 0xf0, 0x20); //line adjust MSB
    s->set_reg(s, 0x2b, 0xff, 0xff); //line adjust
  }
  else if (light == 9)
  {
    s->set_reg(s, 0x47, 0xff, 0x20); //Frame Length Adjustment MSBs
    s->set_reg(s, 0x2a, 0xf0, 0x10); //line adjust MSB
    s->set_reg(s, 0x2b, 0xff, 0xff); //line adjust
  }
  else if (light == 10)
  {
    s->set_reg(s, 0x47, 0xff, 0x10); //Frame Length Adjustment MSBs
    s->set_reg(s, 0x2a, 0xf0, 0x70); //line adjust MSB
    s->set_reg(s, 0x2b, 0xff, 0xff); //line adjust
  }
  else if (light <= 12)
  {
    s->set_reg(s, 0x47, 0xff, 0x10); //Frame Length Adjustment MSBs
    s->set_reg(s, 0x2a, 0xf0, 0x60); //line adjust MSB
    s->set_reg(s, 0x2b, 0xff, 0xff); //line adjust
  }
  else if (light <= 14)
  {
    s->set_reg(s, 0x47, 0xff, 0x10); //Frame Length Adjustment MSBs
    s->set_reg(s, 0x2a, 0xf0, 0x40); //line adjust MSB
    s->set_reg(s, 0x2b, 0xff, 0xff); //line adjust
  }
  else if (light <= 18)
  {
    s->set_reg(s, 0x47, 0xff, 0x08); //Frame Length Adjustment MSBs
    s->set_reg(s, 0x2a, 0xf0, 0xb0); //line adjust MSB
    s->set_reg(s, 0x2b, 0xff, 0xff); //line adjust
  }
  else if (light <= 20)
  {
    s->set_reg(s, 0x47, 0xff, 0x08); //Frame Length Adjustment MSBs
    s->set_reg(s, 0x2a, 0xf0, 0x80); //line adjust MSB
    s->set_reg(s, 0x2b, 0xff, 0xff); //line adjust
  }
  else if (light <= 23)
  {
    s->set_reg(s, 0x47, 0xff, 0x08); //Frame Length Adjustment MSBs
    s->set_reg(s, 0x2a, 0xf0, 0x60); //line adjust MSB
    s->set_reg(s, 0x2b, 0xff, 0xff); //line adjust
  }
  else if (light <= 27)
  {
    s->set_reg(s, 0x47, 0xff, 0x04); //Frame Length Adjustment MSBs
    s->set_reg(s, 0x2a, 0xf0, 0xd0); //line adjust MSB
    s->set_reg(s, 0x2b, 0xff, 0xff); //line adjust
  }
  else if (light <= 31)
  {
    s->set_reg(s, 0x47, 0xff, 0x04); //Frame Length Adjustment MSBs
    s->set_reg(s, 0x2a, 0xf0, 0x80); //line adjust MSB
    s->set_reg(s, 0x2b, 0xff, 0xff); //line adjust
  }
  else if (light <= 35)
  {
    s->set_reg(s, 0x47, 0xff, 0x04); //Frame Length Adjustment MSBs
    s->set_reg(s, 0x2a, 0xf0, 0x60); //line adjust MSB
    s->set_reg(s, 0x2b, 0xff, 0xff); //line adjust
  }
  else if (light <= 40)
  {
    s->set_reg(s, 0x47, 0xff, 0x02); //Frame Length Adjustment MSBs
    s->set_reg(s, 0x2a, 0xf0, 0x70); //line adjust MSB
    s->set_reg(s, 0x2b, 0xff, 0xff); //line adjust
  }
  else if (light < 45)
  {
    s->set_reg(s, 0x47, 0xff, 0x02); //Frame Length Adjustment MSBs
    s->set_reg(s, 0x2a, 0xf0, 0x40); //line adjust MSB
    s->set_reg(s, 0x2b, 0xff, 0xff); //line adjust
  }
  //after this the frame rate is higher, so we need to compensate
  else if (light < 50)
  {
    s->set_reg(s, 0x47, 0xff, 0x04); //Frame Length Adjustment MSBs
    s->set_reg(s, 0x2a, 0xf0, 0xa0); //line adjust MSB
    s->set_reg(s, 0x2b, 0xff, 0xff); //line adjust
  }
  else if (light < 55)
  {
    s->set_reg(s, 0x47, 0xff, 0x04); //Frame Length Adjustment MSBs
    s->set_reg(s, 0x2a, 0xf0, 0x70); //line adjust MSB
    s->set_reg(s, 0x2b, 0xff, 0xff); //line adjust
  }
  else if (light < 65)
  {
    s->set_reg(s, 0x47, 0xff, 0x04); //Frame Length Adjustment MSBs
    s->set_reg(s, 0x2a, 0xf0, 0x30); //line adjust MSB
    s->set_reg(s, 0x2b, 0xff, 0xff); //line adjust
  }
  else if (light < 75)
  {
    s->set_reg(s, 0x47, 0xff, 0x02); //Frame Length Adjustment MSBs
    s->set_reg(s, 0x2a, 0xf0, 0x80); //line adjust MSB
    s->set_reg(s, 0x2b, 0xff, 0xff); //line adjust
  }
  else if (light < 90)
  {
    s->set_reg(s, 0x47, 0xff, 0x02); //Frame Length Adjustment MSBs
    s->set_reg(s, 0x2a, 0xf0, 0x50); //line adjust MSB
    s->set_reg(s, 0x2b, 0xff, 0xbf); //line adjust
  }
  else if (light < 100)
  {
    s->set_reg(s, 0x47, 0xff, 0x02); //Frame Length Adjustment MSBs
    s->set_reg(s, 0x2a, 0xf0, 0x20); //line adjust MSB
    s->set_reg(s, 0x2b, 0xff, 0x8f); //line adjust
  }
  else if (light < 110)
  {
    s->set_reg(s, 0x47, 0xff, 0x02); //Frame Length Adjustment MSBs
    s->set_reg(s, 0x2a, 0xf0, 0x10); //line adjust MSB
    s->set_reg(s, 0x2b, 0xff, 0x7f); //line adjust
  }
  else if (light < 120)
  {
    s->set_reg(s, 0x47, 0xff, 0x01); //Frame Length Adjustment MSBs
    s->set_reg(s, 0x2a, 0xf0, 0x10); //line adjust MSB
    s->set_reg(s, 0x2b, 0xff, 0x5f); //line adjust
  }
  else if (light < 130)
  {
    s->set_reg(s, 0x47, 0xff, 0x00); //Frame Length Adjustment MSBs
    s->set_reg(s, 0x2a, 0xf0, 0x0); //line adjust MSB
    s->set_reg(s, 0x2b, 0xff, 0x2f); //line adjust
  }
  else if (light < 140)
  {
    s->set_reg(s, 0x47, 0xff, 0x00); //Frame Length Adjustment MSBs
    s->set_reg(s, 0x2a, 0xf0, 0x0); //line adjust MSB
    s->set_reg(s, 0x2b, 0xff, 0x0); //line adjust
  }

  if (light < day_switch_value)s->set_reg(s, 0x43, 0xff, 0x40); //magic value to give us the frame faster (bit 6 must be 1)

  //fb = esp_camera_fb_get();

  s->set_reg(s, 0xff, 0xff, 0x00); //banksel
  s->set_reg(s, 0xd3, 0xff, 0x8); //clock

  s->set_reg(s, 0x42, 0xff, 0x2f); //image quality (lower is bad)
  s->set_reg(s, 0x44, 0xff, 3); //quality

  //s->set_reg(s,0x96,0xff,0x10);//bit 4, disable saturation


  //s->set_reg(s,0xbc,0xff,0xff);//red channel adjustment, 0-0xff (the higher the brighter)
  //s->set_reg(s,0xbd,0xff,0xff);//green channel adjustment, 0-0xff (the higher the brighter)
  //s->set_reg(s,0xbe,0xff,0xff);//blue channel adjustment, 0-0xff (the higher the brighter)

  //s->set_reg(s,0xbf,0xff,128);//if the last bit is not set, the image is dim. All other bits don't seem to do anything but ocasionally crash the camera

  //s->set_reg(s,0xa5,0xff,0);//contrast 0 is none, 0xff is very high. Not really useful over 20 or so at most.

  //s->set_reg(s,0x8e,0xff,0x30);//bits 5 and 4, if set make the image darker, not very useful
  //s->set_reg(s,0x91,0xff,0x67);//really weird stuff in the last 4 bits, can also crash the camera

  //no sharpening
  s->set_reg(s, 0x92, 0xff, 0x1);
  s->set_reg(s, 0x93, 0xff, 0x0);

  Serial.println("After the radu");
  
  y = millis();
  fb = esp_camera_fb_get();
  Serial.printf("Get took %d ms\n",millis()-y);
  esp_camera_fb_return(fb);
  
  y = millis();
  fb = esp_camera_fb_get();
  esp_camera_fb_return(fb);
  Serial.printf("Get took %d ms\n",millis()-y);
  
  y = millis();
  fb = esp_camera_fb_get();
  esp_camera_fb_return(fb);
  Serial.printf("Get took %d ms\n",millis()-y);
  
}
