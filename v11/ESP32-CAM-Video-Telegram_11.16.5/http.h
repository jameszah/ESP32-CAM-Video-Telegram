/*

  HTTP for a javascript webpage to implement wifimanager, and parameters for ESP32-CAM-Video-Telegram

  by James Zahary  Apr 17, 2024
  jamzah.plc@gmail.com

  based on:

  https://github.com/jameszah/ESP32-CAM-JAMCAM

*/

#include "html.h" // contains the web page for index

char file_to_edit[50];
extern int new_radu;

static esp_err_t edit_handler(httpd_req_t *req) {
  esp_err_t res = ESP_OK;
  char  buf[120];
  size_t buf_len;
  char  new_res[20];

  Serial.print("edit_handler, core ");  Serial.print(xPortGetCoreID());
  Serial.print(", priority = "); Serial.println(uxTaskPriorityGet(NULL));


  buf_len = httpd_req_get_url_query_len(req) + 1;
  if (httpd_req_get_url_query_str(req, buf, buf_len) == ESP_OK) {
    Serial.printf("Query => %s\n", buf);
    char param[32];

    if (httpd_query_key_value(buf, "f", file_to_edit, sizeof(file_to_edit)) == ESP_OK) {
      Serial.printf( "Found URL query parameter => f=>%s<\n", file_to_edit);

    }

  }


  httpd_resp_send(req, edit_html, strlen(edit_html));

  return res;;
}

static esp_err_t reboot_handler(httpd_req_t *req) {
  esp_err_t res = ESP_OK;
  start_record = 0;

  Serial.print("reboot_handler, core ");  Serial.print(xPortGetCoreID());
  Serial.print(", priority = "); Serial.println(uxTaskPriorityGet(NULL));

  char x[30] = " { \"res\":\"thanks\"} ";
  //httpd_resp_set_type(req, "appication/json");
  httpd_resp_sendstr(req, x);

  //httpd_resp_send(req, page_html, strlen(page_html));
  delay(100);
  ESP.restart();
  return res;;
}


static esp_err_t time_handler(httpd_req_t *req) {
  esp_err_t res = ESP_OK;
  char  buf[120];
  size_t buf_len;
  char  new_res[20];

  Serial.print("time_handler, core ");  Serial.print(xPortGetCoreID());
  Serial.print(", priority = "); Serial.println(uxTaskPriorityGet(NULL));


  buf_len = httpd_req_get_url_query_len(req) + 1;
  if (httpd_req_get_url_query_str(req, buf, buf_len) == ESP_OK) {
    Serial.printf("Time => %s\n", buf);
    char param[32];
    if (httpd_query_key_value(buf, "t", param, sizeof(param)) == ESP_OK) {

      int x = atoi(param);

      Serial.printf( "Found URL query parameter => t=%d\n", x);


      Serial.printf("Char >%s<\n", timezone);
      setenv("TZ", timezone, 1);
      tzset();
      struct timeval tv;
      tv.tv_sec = x;
      tv.tv_usec = 0;
      settimeofday(&tv, NULL);
    }
  }

  httpd_resp_send(req, config_html, strlen(config_html));
  return res;;
}


//////////////////////////////
oneframe find_a_frame (char * avi_file_name, int frame_pct) ; // from avi.cpp file


static esp_err_t find_handler(httpd_req_t *req) {
  esp_err_t res = ESP_OK;
  char  buf[120];
  size_t buf_len;
  char  new_res[20];

  oneframe x;

  size_t _jpg_buf_len = 0;
  uint8_t * _jpg_buf = NULL;
  char * part_buf[64];
  int frame_pct;
  char filename[50];

  Serial.print("find_handler, core ");  Serial.print(xPortGetCoreID());
  Serial.print(", priority = "); Serial.println(uxTaskPriorityGet(NULL));

  buf_len = httpd_req_get_url_query_len(req) + 1;
  if (httpd_req_get_url_query_str(req, buf, buf_len) == ESP_OK) {
    //Serial.printf("Query => %s\n", buf);
    char param[32];
    if (httpd_query_key_value(buf, "f", filename, sizeof(filename)) == ESP_OK) {
      Serial.printf( "Found URL query parameter => f=>%s<\n", filename);

    }
    if (httpd_query_key_value(buf, "n", param, sizeof(param)) == ESP_OK) {
      int nn = atoi(param);
      if (nn >= 0 && nn <= 30000 ) {
        frame_pct = nn;
        Serial.printf( "Found URL query parameter => n=%d\n", frame_pct);
      }
    }
  }

  //uint8_t* the_frame = find_a_frame ( "/JamCam0090.0001.avi", 12);

  x = find_a_frame ( filename, frame_pct);
  //the_frame = x.the_frame;

  _jpg_buf_len = x.the_frame_length;
  _jpg_buf = x.the_frame;


  if (x.the_frame == NULL) {
    Serial.printf("no frame\n");

    httpd_resp_send(req, config_html, strlen(config_html));
  } else {

    res = httpd_resp_set_type(req, "image/jpeg");
    if (res != ESP_OK) {
      return res;
    }

    if (res == ESP_OK) {
      char fname[50];
      char frame_num_char[8];
      char frame_pct_char[8];
      char frame_total_char[8];

      sprintf(fname, "inline; filename=frame_%d.jpg", frame_pct);
      sprintf(frame_num_char, "%d", x.the_frame_number);
      sprintf(frame_total_char, "%d", x.the_frame_total);

      sprintf(frame_pct_char, "%d", frame_pct);

      httpd_resp_set_hdr(req, "Content-Disposition", fname);
      httpd_resp_set_hdr(req, "FrameNum", frame_num_char);
      httpd_resp_set_hdr(req, "Total", frame_total_char);
      httpd_resp_set_hdr(req, "FramePct", frame_pct_char);
      httpd_resp_set_hdr(req, "File", filename);


    }
    if (res == ESP_OK) {
      res = httpd_resp_send(req, (const char *)_jpg_buf, _jpg_buf_len);
    }

    free (x.the_frame);
  }
  return res;;
}

static esp_err_t index_handler(httpd_req_t *req) {
  esp_err_t res = ESP_OK;
  Serial.print("index_handler, core ");  Serial.print(xPortGetCoreID());
  Serial.print(", priority = "); Serial.println(uxTaskPriorityGet(NULL));

  Serial.printf("Page len %d\n", strlen(config_html));
  httpd_resp_send(req, config_html, strlen(config_html));
  return res;;
}

extern bool test_send;
static esp_err_t testpic_handler(httpd_req_t *req) {
  esp_err_t res = ESP_OK;
  Serial.print("testpic_handler, core ");  Serial.print(xPortGetCoreID());
  Serial.print(", priority = "); Serial.println(uxTaskPriorityGet(NULL));

  test_send = true;

  char x[30] = " { \"res\":\"thanks\"} ";
  //httpd_resp_set_type(req, "appication/json");
  httpd_resp_sendstr(req, x);

  //Serial.printf("Page len %d\n", strlen(config_html));
  //httpd_resp_send(req, config_html, strlen(config_html));
  return res;;
}

extern bool test_send_avi;
static esp_err_t testavi_handler(httpd_req_t *req) {
  esp_err_t res = ESP_OK;
  Serial.print("testavi_handler, core ");  Serial.print(xPortGetCoreID());
  Serial.print(", priority = "); Serial.println(uxTaskPriorityGet(NULL));

  test_send_avi = true;

  char x[30] = " { \"res\":\"thanks\"} ";
  //httpd_resp_set_type(req, "appication/json");
  httpd_resp_sendstr(req, x);

  //Serial.printf("Page len %d\n", strlen(config_html));
  //httpd_resp_send(req, config_html, strlen(config_html));
  return res;;
}

static esp_err_t capture_handler(httpd_req_t *req) {
  esp_err_t res = ESP_OK;
  Serial.print("capture_handler, core ");  Serial.print(xPortGetCoreID());
  Serial.print(", priority = "); Serial.println(uxTaskPriorityGet(NULL));

  char fname[100];
  int file_number = 0;

  file_number++;
  sprintf(fname, "inline; filename=capture_%d.jpg", file_number);

  camera_fb_t * fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("Camera Capture Failed");
  } else {


    httpd_resp_set_type(req, "image/jpeg");
    httpd_resp_set_hdr(req, "Content-Disposition", fname);

    res = httpd_resp_send(req, (const char *)fb->buf, fb->len);
    esp_camera_fb_return(fb);
  }

  return res;;
}

static esp_err_t update_handler(httpd_req_t *req) {
  size_t buf_len;

  char buf[1000];
  int ret;
  int remaining = req->content_len;

  esp_err_t res = ESP_OK;
  Serial.print("config_handler, core ");  Serial.print(xPortGetCoreID());

  buf_len = httpd_req_get_url_query_len(req) + 1;
  Serial.printf("buf_len %d\n", buf_len);
  ret = httpd_req_recv(req, buf, remaining + 1 );
  //ret = ret + 1;
  Serial.printf("ret %d remaining %d \n", ret, remaining);
  Serial.printf("recv => %.*s\n", ret, buf);

  Serial.printf("Query => %.*s\n", ret, buf);
  //#define ZERO_COPY(STR)    ((char*)STR.c_str())
  //DynamicJsonDocument doc(900);
  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, buf);
  serializeJsonPretty(doc, Serial);

  if (!error) {

    if (doc.containsKey("ssidap")) {
      String abc = doc["ssidap"].as<String>();
      const char* aschar = doc["ssidap"];
      Serial.printf(">%s<\n", aschar);
      Serial.println(abc);
    }

    if (doc.containsKey("ssidap")) {
      const char* x = doc["ssidap"];
      Serial.printf(">%s<\n", x);
      if (strlen(x) > 0 && strlen(x) < 30) {
        strcpy(ssidap, x);
      } else {
        strcpy(ssidap, "");
      }
      if (doc.containsKey("passap")) {
        const char* x = doc["passap"];
        Serial.printf(">%s<\n", x);
        if (strlen(x) > 0 && strlen(x) < 30) {
          strcpy(passap, x);
        } else {
          strcpy(passap, "");
        }
      }

      if (doc.containsKey("ssidapsecret")) {
        const char* x = doc["ssidapsecret"];
        Serial.printf(">%s<\n", x);
        if (strlen(x) > 0 && strlen(x) < 30) {
          strcpy(ssidapsecret, x);
        } else {
          strcpy(ssidapsecret, "");
        }
      }

      if (doc.containsKey("raduauto")) {
        const char* x = doc["raduauto"];
        Serial.printf(">%s<\n", x);
        if (strlen(x) > 0 && strlen(x) < 30) {
          strcpy(raduauto, x);
        } else {
          strcpy(raduauto, "");
        }
      }
      
      if (doc.containsKey("send_all_avi")) {
        const char* x = doc["send_all_avi"];
        Serial.printf(">%s<\n", x);
        if (strlen(x) > 0 && strlen(x) < 30) {
          strcpy(send_all_avi, x);
        } else {
          strcpy(send_all_avi, "");
        }
      }


      if (doc.containsKey("ssid1")) {
        const char* x = doc["ssid1"];
        Serial.printf(">%s<\n", x);
        if (strlen(x) > 0 && strlen(x) < 30) {
          strcpy(ssid1, x);
        } else {
          strcpy(ssid1, "");
        }
      }

      if (doc.containsKey("pass1")) {
        const char* x = doc["pass1"];
        Serial.printf(">%s<\n", x);
        if (strlen(x) > 0 && strlen(x) < 30) {
          strcpy(pass1, x);
        } else {
          strcpy(pass1, "");
        }
      }
      if (doc.containsKey("ssid2")) {
        const char* x = doc["ssid2"];
        Serial.printf(">%s<\n", x);
        if (strlen(x) > 0 && strlen(x) < 30) {
          strcpy(ssid2, x);
        } else {
          strcpy(ssid2, "");
        }
      }
      if (doc.containsKey("pass2")) {
        const char* x = doc["pass2"];
        Serial.printf(">%s<\n", x);
        if (strlen(x) > 0 && strlen(x) < 30) {
          strcpy(pass2, x);
        } else {
          strcpy(pass2, "");
        }
      }
      if (doc.containsKey("ssid3")) {
        const char* x = doc["ssid3"];
        Serial.printf(">%s<\n", x);
        if (strlen(x) > 0 && strlen(x) < 30) {
          strcpy(ssid3, x);
        } else {
          strcpy(ssid3, "");
        }
      }
      if (doc.containsKey("pass3")) {
        const char* x = doc["pass3"];
        Serial.printf(">%s<\n", x);
        if (strlen(x) > 0 && strlen(x) < 30) {
          strcpy(pass3, x);
        } else {
          strcpy(pass3, "");
        }
      }
      if (doc.containsKey("telegramid")) {
        const char* x = doc["telegramid"];
        Serial.printf(">%s<\n", x);
        if (strlen(x) > 0 && strlen(x) < 30) {
          strcpy(telegramid, x);
        } else {
          strcpy(telegramid, "");
        }
      }
      if (doc.containsKey("telegrambot")) {
        const char* x = doc["telegrambot"];
        Serial.printf(">%s<\n", x);
        if (strlen(x) > 0 && strlen(x) < 60) {
          strcpy(telegrambot, x);
        } else {
          strcpy(telegrambot, "");
        }
      }
      if (doc.containsKey("frame")) {
        const char* x = doc["frame"];
        Serial.printf(">%s<\n", x);
        if (strlen(x) > 0 && strlen(x) < 10) {
          strcpy(frame, x);
        } else {
          strcpy(frame, "");
        }
      }
      if (doc.containsKey("timezone")) {
        const char* x = doc["timezone"];
        Serial.printf(">%s<\n", x);
        if (strlen(x) > 0 && strlen(x) < 60) {
          strcpy(timezone, x);
        } else {
          strcpy(timezone, "");
        }
      }
    }
  }

  do_eprom_write();
  Serial.printf("Page len %d\n", ret);
  char x[30] = " { \"res\":\"thanks\"} ";
  //httpd_resp_set_type(req, "appication/json");
  httpd_resp_sendstr(req, x);

  init_soft();
  init_wifi();


  return res;;
}


static esp_err_t config_handler(httpd_req_t *req) {
  esp_err_t res = ESP_OK;
  Serial.print("config_handler, core ");  Serial.print(xPortGetCoreID());
  Serial.print(", priority = "); Serial.println(uxTaskPriorityGet(NULL));

  size_t buf_len;
  char  new_res[20];

  size_t _jpg_buf_len = 0;
  uint8_t * _jpg_buf = NULL;
  char * part_buf[64];
  int frame_pct;
  char filename[50];

  char buf[1000];
  int ret;
  int remaining = req->content_len;
  Serial.printf("remaining  %d\n", remaining);


  buf_len = httpd_req_get_url_query_len(req) + 1;
  Serial.printf("buf_len %d\n", buf_len);
  ret = httpd_req_recv(req, buf, remaining + 1 );
  ret = ret + 1;
  Serial.printf("ret %d remaining %d \n", ret, remaining);
  Serial.printf("recv => %.*s\n", ret, buf);
  //if (httpd_req_get_url_query_str(req, buf, buf_len) == ESP_OK) {
  Serial.printf("Query => %.*s\n", ret, buf);
  char param[70];

  if (httpd_query_key_value(buf, "ssidap", param, sizeof(param)) == ESP_OK) {
    Serial.printf( "Found URL query parameter => ssidap=   >%s<  \n", param);
    //strcpy(destinationArray, sourceArray);
    if (strlen(param) > 0 && strlen(param) < 30) {
      strcpy(ssidap, param);
    } else {
      strcpy(ssidap, "");
    }
  } else {
    Serial.printf("no ssidap\n");
  }
  if (httpd_query_key_value(buf, "passap", param, sizeof(param)) == ESP_OK) {
    Serial.printf( "Found URL query parameter => passap=   >%s<  \n", param);
    if (strlen(param) > 0 && strlen(param) < 30) {
      strcpy(passap, param);
    } else {
      strcpy(passap, "");
    }
  } else {
    Serial.printf("no ssidap\n");
  }
  if (httpd_query_key_value(buf, "ssid1", param, sizeof(param)) == ESP_OK) {
    Serial.printf( "Found URL query parameter => ssid1=   >%s<  \n", param);
    if (strlen(param) > 0 && strlen(param) < 30) {
      strcpy(ssid1, param);
    } else {
      strcpy(ssid1, "");
    }
  } else {
    Serial.printf("no ssidap\n");
  }
  if (httpd_query_key_value(buf, "pass1", param, sizeof(param)) == ESP_OK) {
    Serial.printf( "Found URL query parameter => pass1=   >%s<  \n", param);
    if (strlen(param) > 0 && strlen(param) < 30) {
      strcpy(pass1, param);
    } else {
      strcpy(pass1, "");
    }

  } else {
    Serial.printf("no ssidap\n");
  }
  if (httpd_query_key_value(buf, "ssid2", param, sizeof(param)) == ESP_OK) {
    Serial.printf( "Found URL query parameter => ssid2=   >%s<  \n", param);
    if (strlen(param) > 0 && strlen(param) < 30) {
      strcpy(ssid2, param);
    } else {
      strcpy(ssid2, "");
    }

  } else {
    Serial.printf("no ssidap\n");
  }
  if (httpd_query_key_value(buf, "pass2", param, sizeof(param)) == ESP_OK) {
    Serial.printf( "Found URL query parameter => pass2=   >%s<  \n", param);
    if (strlen(param) > 0 && strlen(param) < 30) {
      strcpy(pass2, param);
    } else {
      strcpy(pass2, "");
    }

  } else {
    Serial.printf("no ssidap\n");
  }
  if (httpd_query_key_value(buf, "ssid3", param, sizeof(param)) == ESP_OK) {
    Serial.printf( "Found URL query parameter => ssid3=   >%s<  \n", param);
    if (strlen(param) > 0 && strlen(param) < 30) {
      strcpy(ssid3, param);
    } else {
      strcpy(ssid3, "");
    }

  } else {
    Serial.printf("no ssidap\n");
  }
  if (httpd_query_key_value(buf, "pass3", param, sizeof(param)) == ESP_OK) {
    Serial.printf( "Found URL query parameter => pass3=   >%s<  \n", param);
    if (strlen(param) > 0 && strlen(param) < 30) {
      strcpy(pass3, param);
    } else {
      strcpy(pass3, "");
    }

  } else {
    Serial.printf("no ssidap\n");
  }

  if (httpd_query_key_value(buf, "telegramid", param, sizeof(param)) == ESP_OK) {
    Serial.printf( "Found URL query parameter => telegramid=   >%s<  \n", param);
    if (strlen(param) > 8 && strlen(param) < 15) {
      strcpy(telegramid, param);
    } else {
      strcpy(telegramid, "");
    }

  } else {
    Serial.printf("no ssidap\n");
  }
  if (httpd_query_key_value(buf, "telegrambot", param, sizeof(param)) == ESP_OK) {
    Serial.printf( "Found URL query parameter => telegrambot=   >%s<  \n", param);

    if (strlen(param) > 45 && strlen(param) < 55) {
      Serial.printf(">>>%c<<<\n", param[10]);
      String x = param;
      x.replace("%3A", ":");
      x.toCharArray(param, 55);
      //devstr.toCharArray(devname, 12);
      //memset (param + 10, ':', 1); //param[10] = ":";
      strcpy(telegrambot, param);
    } else {
      strcpy(telegrambot, "");
    }

  } else {
    Serial.printf("no ssidap\n");
  }

  if (httpd_query_key_value(buf, "frame", param, sizeof(param)) == ESP_OK) {
    Serial.printf( "Found URL query parameter => frame=   >%s<  \n", param);
    if (strlen(param) > 1 && strlen(param) < 9) {
      strcpy(frame, param);
    } else {
      strcpy(frame, "");
    }
  } else {
    Serial.printf("no ssidap\n");
  }

  if (httpd_query_key_value(buf, "timezone", param, sizeof(param)) == ESP_OK) {
    Serial.printf( "Found URL query parameter => timezone=   >%s<  \n", param);
    if (strlen(param) > 1 && strlen(param) < 40) {
      strcpy(timezone, param);
    } else {
      strcpy(timezone, "");
    }
  } else {
    Serial.printf("no ssidap\n");
  }
  do_eprom_write();

  Serial.printf("Page len %d\n", ret);
  httpd_resp_send(req, buf, ret);
  return res;;
}


static esp_err_t test_handler(httpd_req_t *req) {
  esp_err_t res = ESP_OK;
  Serial.print("test_handler, core ");  Serial.print(xPortGetCoreID());
  Serial.print(", priority = "); Serial.println(uxTaskPriorityGet(NULL));

  send_a_telegram_test = true;

  String x = " {\"hi\":" + String(1);  // first parameter with curly and no comma
  x = x + "}";

  const char* str = x.c_str();

  httpd_resp_send(req, str,  strlen(str));
  return res;
}

static esp_err_t net_status_handler(httpd_req_t *req) {
  esp_err_t res = ESP_OK;
  Serial.print("status_handler, core ");  Serial.print(xPortGetCoreID());
  Serial.print(", priority = "); Serial.println(uxTaskPriorityGet(NULL));


  char mysoftap[50];
  sprintf(mysoftap, "Wifi:%s %s", ssidap, WiFi.softAPIP().toString().c_str());
  char myip[50];
  if (WiFi.status() != WL_CONNECTED ) {
    sprintf(myip, "No Wifi to Internet!" );
  } else {
    sprintf(myip, "%s", WiFi.localIP().toString().c_str());
  }
  String x = " {\"hi\":" + String(1);  // first parameter with curly and no comma
  x = x + ",\"softap\":" + "\"" + String(mysoftap) + "\"" ;
  x = x + ",\"ip\":" + "\"" + String(myip) + ", rssi: " + String(WiFi.RSSI()) + "\"" ;
  x = x + ",\"ver\":" + "\"" + String(vernum) + "\"" ;
  x = x + "}";

  const char* str = x.c_str();

  httpd_resp_send(req, str,  strlen(str));
  return res;
}

static esp_err_t status_handler(httpd_req_t *req) {
  esp_err_t res = ESP_OK;
  Serial.print("status_handler, core ");  Serial.print(xPortGetCoreID());
  Serial.print(", priority = "); Serial.println(uxTaskPriorityGet(NULL));

  String x = " {\"hi\":" + String(1);  // first parameter with curly and no comma
  x = x + ",\"ssidap\":" + "\"" + String(ssidap) + "\"" ;
  x = x + ",\"passap\":" + "\"" + String(passap) + "\"" ;
  x = x + ",\"ssidapsecret\":" + "\"" + String(ssidapsecret) + "\"" ;
  x = x + ",\"send_all_avi\":" + "\"" + String(send_all_avi) + "\"" ;
  x = x + ",\"raduauto\":" + "\"" + String(raduauto) + "\"" ;
  x = x + ",\"ssid1\":" + "\"" + String(ssid1) + "\"" ;
  x = x + ",\"pass1\":" + "\"" + String(pass1) + "\"" ;
  x = x + ",\"ssid2\":" + "\"" + String(ssid2) + "\"" ;
  x = x + ",\"pass2\":" + "\"" + String(pass2) + "\"" ;
  x = x + ",\"ssid3\":" + "\"" + String(ssid3) + "\"" ;
  x = x + ",\"pass3\":" + "\"" + String(pass3) + "\"" ;
  x = x + ",\"telegramid\":" + "\"" + String(telegramid) + "\"" ;
  x = x + ",\"telegrambot\":" + "\"" + String(telegrambot) + "\"" ;
  x = x + ",\"frame\":" + "\"" + String(frame) + "\"" ;
  x = x + ",\"timezone\":" + "\"" + String(timezone) + "\"" ;
  x = x + "}";


  const char* str = x.c_str();

  httpd_resp_send(req, str,  strlen(str));
  return res;
}

static esp_err_t edstatus_handler(httpd_req_t *req) {
  esp_err_t res = ESP_OK;
  Serial.print("edstatus_handler, core ");  Serial.print(xPortGetCoreID());
  Serial.print(", priority = "); Serial.println(uxTaskPriorityGet(NULL));

  delay(111);

  String x = " {\"hi\":\"x\" ";

  x = x + ",\"file_to_edit\":" + "\"" + String(file_to_edit) + "\"" ;

  x = x + "}";

  const char* str = x.c_str();

  httpd_resp_send(req, str,  strlen(str));
  return res;
}

//////////
static esp_err_t radu_handler(httpd_req_t *req) {
  esp_err_t res = ESP_OK;
  char  buf[120];
  size_t buf_len;
  char  new_res[20];

  char * part_buf[64];

  Serial.print("radu_handler, core ");  Serial.print(xPortGetCoreID());
  Serial.print(", priority = "); Serial.println(uxTaskPriorityGet(NULL));

  buf_len = httpd_req_get_url_query_len(req) + 1;
  if (httpd_req_get_url_query_str(req, buf, buf_len) == ESP_OK) {
    //Serial.printf("Query => %s\n", buf);
    char param[32];

    if (httpd_query_key_value(buf, "r", param, sizeof(param)) == ESP_OK) {
      int nn = atoi(param);
      if (nn >= 0 && nn <= 100 ) {
        Serial.printf( "Found URL query parameter => n=%d\n", nn);
        new_radu = nn;
      }
    }
  }


  if (res == ESP_OK) {
    char x[30] = " { \"res\":\"thanks\"} ";
    //httpd_resp_set_type(req, "appication/json");
    httpd_resp_sendstr(req, x);

  }



  return res;;
}
//////////

void startHttpServer() {

  httpd_config_t config2 = HTTPD_DEFAULT_CONFIG();
  config2.server_port = 80;
  config2.max_uri_handlers = 16;

  httpd_uri_t index_uri = {
    .uri       = "/",
    .method    = HTTP_GET,
    .handler   = index_handler,
    .user_ctx  = NULL
  };
  httpd_uri_t testpic_uri = {
    .uri       = "/photo",
    .method    = HTTP_GET,
    .handler   = testpic_handler,
    .user_ctx  = NULL
  };
  httpd_uri_t testavi_uri = {
    .uri       = "/clip",
    .method    = HTTP_GET,
    .handler   = testavi_handler,
    .user_ctx  = NULL
  };
  httpd_uri_t config_uri = {
    .uri       = "/",
    .method    = HTTP_POST,
    .handler   = config_handler,
    .user_ctx  = NULL
  };
  httpd_uri_t update_uri = {
    .uri       = "/update",
    .method    = HTTP_POST,
    .handler   = update_handler,
    .user_ctx  = NULL
  };
  httpd_uri_t capture_uri = {
    .uri       = "/capture",
    .method    = HTTP_GET,
    .handler   = capture_handler,
    .user_ctx  = NULL
  };
  httpd_uri_t status_uri = {
    .uri       = "/status",
    .method    = HTTP_GET,
    .handler   = status_handler,
    .user_ctx  = NULL
  };
  httpd_uri_t edstatus_uri = {
    .uri       = "/edstatus",
    .method    = HTTP_GET,
    .handler   = edstatus_handler,
    .user_ctx  = NULL
  };
  httpd_uri_t test_uri = {
    .uri       = "/test",
    .method    = HTTP_GET,
    .handler   = test_handler,
    .user_ctx  = NULL
  };
  httpd_uri_t net_status_uri = {
    .uri       = "/net_status",
    .method    = HTTP_GET,
    .handler   = net_status_handler,
    .user_ctx  = NULL
  };
  httpd_uri_t time_uri = {
    .uri       = "/time",
    .method    = HTTP_GET,
    .handler   = time_handler,
    .user_ctx  = NULL
  };
  httpd_uri_t reboot_uri = {
    .uri       = "/reboot",
    .method    = HTTP_GET,
    .handler   = reboot_handler,
    .user_ctx  = NULL
  };
  httpd_uri_t find_uri = {
    .uri       = "/find",
    .method    = HTTP_GET,
    .handler   = find_handler,
    .user_ctx  = NULL
  };
  httpd_uri_t radu_uri = {
    .uri       = "/radu",
    .method    = HTTP_GET,
    .handler   = radu_handler,
    .user_ctx  = NULL
  };
  httpd_uri_t edit_uri = {
    .uri       = "/edit",
    .method    = HTTP_GET,
    .handler   = edit_handler,
    .user_ctx  = NULL
  };
  Serial.printf("Starting web server on port: '%d'\n", config2.server_port);
  if (httpd_start(&index_httpd, &config2) == ESP_OK) {
    httpd_register_uri_handler(index_httpd, &index_uri);
    httpd_register_uri_handler(index_httpd, &testpic_uri);
    httpd_register_uri_handler(index_httpd, &testavi_uri);
    httpd_register_uri_handler(index_httpd, &status_uri);
    httpd_register_uri_handler(index_httpd, &edstatus_uri);
    httpd_register_uri_handler(index_httpd, &test_uri);
    httpd_register_uri_handler(index_httpd, &net_status_uri);
    httpd_register_uri_handler(index_httpd, &capture_uri);
    httpd_register_uri_handler(index_httpd, &update_uri);
    httpd_register_uri_handler(index_httpd, &config_uri);
    httpd_register_uri_handler(index_httpd, &edit_uri);
    //httpd_register_uri_handler(index_httpd, &ota_uri);
    httpd_register_uri_handler(index_httpd, &time_uri);
    httpd_register_uri_handler(index_httpd, &reboot_uri);
    httpd_register_uri_handler(index_httpd, &find_uri);
    httpd_register_uri_handler(index_httpd, &radu_uri);

  }

  Serial.printf("after index RAM  %7d / %7d, SPI %7d / %7d\n ", ESP.getFreeHeap(), ESP.getHeapSize(),  ESP.getFreePsram(), ESP.getPsramSize() );


}
