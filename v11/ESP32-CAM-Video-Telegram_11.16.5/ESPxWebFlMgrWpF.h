// mods by James Zahary Dec 28, 2021 https://github.com/jameszah/ESPxWebFlMgr
// based on https://github.com/holgerlembke/ESPxWebFlMgr

// inline guard. Still broken by design?
#ifndef ESPxWebFlMgrWpF_h
#define ESPxWebFlMgrWpF_h

static const char ESPxWebFlMgrWpFormIntro[] PROGMEM = 
R"==x==(
Edit your WiFi name and password
<form><textarea id="tect" rows="10" cols="60">
)==x==";


static const char ESPxWebFlMgrWpFormExtro1[] PROGMEM = 
R"==x==(</textarea></form><button title="Save file" onclick="sved(')==x==";

// noot sure what the <script> part is for.
static const char ESPxWebFlMgrWpFormExtro2[] PROGMEM = 
R"==x==(');" >Save</button>&nbsp;<button title="Abort editing" onclick="abed();">Done editing</button>

<script id="info">
function sved(filename) {
  var content = document.getElementById('tect').value;
  // utf-8
  content = unescape(encodeURIComponent(content));

  var xhr = new XMLHttpRequest();

  xhr.open("POST", "/r", true);

  var boundary = '-----whatever';
  xhr.setRequestHeader("Content-Type", "multipart/form-data; boundary=" + boundary);

  var body = "" +
    '--' + boundary + '\r\n' +
    'Content-Disposition: form-data; name="uploadfile"; filename="' + filename + '"' + '\r\n' +
    'Content-Type: text/plain' + '\r\n' +
    '' + '\r\n' +
    content + '\r\n' +
    '--' + boundary + '--\r\n' +        // \r\n fixes upload delay in ESP8266WebServer
    '';

  // ajax does not do xhr.setRequestHeader("Content-length", body.length);

  xhr.onreadystatechange = function () {
    var DONE = this.DONE || 4;
    if (this.readyState === DONE) {
      getfileinsert();
    }
  }

  xhr.send(body);
}

function abed() {
  history.back();
  window.close();
}

document.getElementById('o3').innerHTML = "File:";

</script>   )==x==";



#endif
