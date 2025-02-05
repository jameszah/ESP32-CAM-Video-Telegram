/* 
   
  HTML for a javascript webpage to implement wifimanager, and parameters for ESP32-CAM-Video-Telegram
 
  by James Zahary  Apr 17, 2024
  jamzah.plc@gmail.com

  based on:
  
  https://github.com/jameszah/ESP32-CAM-JAMCAM
  
*/


#include <pgmspace.h>

///////////////////////////////
const char edit_html[] PROGMEM = R"===(
<!doctype html>
<html>
   <head>
   <style>
.slidecontainer {
  width: 100%;
}

.slider {
  -webkit-appearance: none;
  width: 600px;
  height: 25px;
  background: #d3d3d3;
  outline: none;
  opacity: 0.7;
  -webkit-transition: .2s;
  transition: opacity .2s;
}

.slider:hover {
  opacity: 1;
}

.slider::-webkit-slider-thumb {
  -webkit-appearance: none;
  appearance: none;
  width: 10px;
  height: 25px;
  background: #04AA6D;
  cursor: pointer;
}

.slider::-moz-range-thumb {
  width: 10px;
  height: 25px;
  background: #04AA6D;
  cursor: pointer;
}
</style>

      <title>ESP32-CAM-Video-Telegram - Video Editor</title>
      <script>
var baseHost = document.location.origin
var streamUrl = baseHost + ':81/stream_avi'

document.addEventListener('DOMContentLoaded', function (event) {

   const view = document.getElementById('liveFeed')
   const videoButton = document.getElementById('video')


}
)
   const stopStream = () => {}
   
   function initialize() {
            
   const query2 = `${baseHost}/edstatus`
   fetch(query2)
      .then(function (response) {
         return response.json()
      })
      .then(function (state) {
         var x = state.toString()
         //console.log(x)
         //document.getElementById("file_to_edit").innerHTML = state.file_to_edit
         console.log(`request to ${query2} finished`)

         console.log(`file_to_edit: ${state.file_to_edit}`)
         document.getElementById("file_to_edit").value = state.file_to_edit
         file_to_write = state.file_to_edit
         let newText = file_to_write.replace(".avi", "x.avi");
         document.getElementById("file_to_write").value = newText
         
         console.log( document.getElementById("file_to_edit").value)


        //
// +/JamCam0481.0007.avi
    console.log(document.getElementById("file_to_edit").value)
    const query = `${baseHost}/find?f=` + document.getElementById("file_to_edit").value + `&n=0` 
    console.log(`the query --- ${query}`)
    
     //document.getElementById('liveFeed').src = query // does query twice?

fetch(query)
  .then(response => { 
    response.blob()
    .then(blob => { 
    
     document.getElementById('liveFeed').src = URL.createObjectURL(blob); 
     document.getElementById("fsta").value =  0;
     document.getElementById("fcur").value =  response.headers.get("framenum");
     document.getElementById("ftotal").value =  response.headers.get("total");
     document.getElementById("fend").value =  response.headers.get("total");
     document.getElementById("myRange").max = response.headers.get("total");
     
     console.log(`request to ${query} finished, status: ${response.status}`)
     console.log("filename: " + response.headers.get("file"));   
     console.log("filepct: " + response.headers.get("framepct")); 
           })
        })  
      })
   }

      </script>
   </head>
   <body onload="initialize()" style="background-color: white">
      <div id="second">
         <h2>JamCam Selfie VideoCam</h2>
         <h3>Video Recorder With Viewfinder, Streamer, Clock, ... and Snakes!</h3>

      </div>
      <div id="third">
         <img id="liveFeed" style="font-family: monospace;  text-align: center;
            color: white; position: relative"
            alt="There seems to be a problem with the live feed..."/>
         <br>
         <div class="slidecontainer">
  <input type="range" min="0" max="100" value="0" class="slider" id="myRange">
  
  <p>Start: <input type="number" id="fsta" min=0 max=99999 style="color:blue;" value="0"> 
  --- <button id="setstart">Set Start</button> ---
  Current: <input type="number" id="fcur" min=0 max=99999 onchnage="fcurchange()" style="color:blue;" value="0"> 
  --- <button id="setend">Set End</button> ---
  End: <input type="number" id="fend" min=0 max=99999 style="color:blue;" value="0"> </p>
  <p>Total: <input type="number" id="ftotal" min=0 max=99999 readonly style="color:red;" value="0"> 
  Skip: <input type="number" id="fskip" min=0 max=999 style="color:blue;" value="0"> Keep 1 frame, then skip 1 (half size) or 9 (10%) etc </p>
  <p>File: <input type="text" id="file_to_edit" readonly size=25 style="color:red;" value="???"> 
  
  
  New File Name: <input type="text" id="file_to_write" size=25 style="color:blue;" value="???"></p>
  

      </div>
      <div id="four">
         <hr>
         <br>
         <textarea id="freeform" name="freeformname" rows="5" cols="40">
Click Record Status ...
             </textarea>
      </div>
      <a href="https://github.com/jameszah/ESP32-CAM-JAMCAM">https://github.com/jameszah/ESP32-CAM-JAMCAM</a> <br>ver 64
<script>
var slider = document.getElementById("myRange");
var fcur = document.getElementById("fcur");
var fsta = document.getElementById("fsta");
var fend = document.getElementById("fend");
fcur.value = slider.value;

//document.getElementById('liveFeed').src = query // does query twice?

function fcurchange() {
    //if (fcur.value != this.value){
    //fcur.value = this.value;
    const query = `${baseHost}/find?f=/JamCam0481.0007.avi&n=` + fcur.value 

fetch(query)
  .then(response => { 
    response.blob()
    .then(blob => { 
    
     document.getElementById('liveFeed').src = URL.createObjectURL(blob); 
     document.getElementById("fcur").value =  response.headers.get("framenum");
     //document.getElementById("ftotal").value =  response.headers.get("total");
     console.log(`request to ${query} finished, status: ${response.status}`)
     //console.log("filename: " + response.headers.get("file"));   
     //console.log("filepct: " + response.headers.get("framepct")); 
    })
  })  
  //}
}
slider.onchange = function() {
  if (fcur.value != this.value){
    fcur.value = this.value;
    var fn = document.getElementById("file_to_edit").value
    const query = `${baseHost}/find?f=` + fn + `&n=` + this.value 

fetch(query)
  .then(response => { 
    response.blob()
    .then(blob => { 
    
     document.getElementById('liveFeed').src = URL.createObjectURL(blob); 
     document.getElementById("fcur").value =  response.headers.get("framenum");
     //document.getElementById("ftotal").value =  response.headers.get("total");
     console.log(`request to ${query} finished, status: ${response.status}`)
     //console.log("filename: " + response.headers.get("file"));   
     //console.log("filepct: " + response.headers.get("framepct")); 
    })
  })  
  }
}

const setstartButton = document.getElementById('setstart')
setstartButton.onclick = () => {
  fsta.value = fcur.value;
}

const setendButton = document.getElementById('setend')
setendButton.onclick = () => {
  fend.value = fcur.value;
}


</script>
   </body>
</html>
)===";

////////////////////////////////



const char config_html[] PROGMEM = R"===(
<!doctype html>
<html>
   <head>
   <style>
html {
  font-family: Arial, Helvetica, sans-serif; 
  display: inline-block; 
  text-align: left;
}

h1 {
  font-size: 1.8rem; 
  color: white;
  text-align: center;
}

p { 
  font-size: 1.2rem;
}

.topnav { 
  overflow: hidden; 
  background-color: #0A1128;
}



input {
  height: 20px;
  flex: 0 0 200px;
  margin-left: 10px;
}

body {  
  margin: 2%;
}

.content { 
  padding: 1%;
}
   </style>

   <title>Telegram Editor</title>
   
      <script>

var baseHost = document.location.origin


document.addEventListener('DOMContentLoaded', function (event) {

   //const view = document.getElementById('liveFeed')
   //const videoButton = document.getElementById('video')

}
)

function get_photo(){
  const query = `${baseHost}/capture`
  console.log(query)
  fetch (query)
  .then (response => {
    //console.log(response)
    //document.getElementById('liveFeed').src = response
    
    response.blob()
    .then (blob => {
      document.getElementById('liveFeed').src = URL.createObjectURL(blob);   
     })
    
 })
}
  


function net_status() {

  const query2 = `${baseHost}/net_status`
  
  fetch(query2)
      .then(function (response) {
        console.log(response)
         return response.json()
      })
      .then(function (state) {
        console.log(state)
         //var x = state.toString()
         //console.log(`netstatus: ${x}`)
         
         
         document.getElementById("softap").value = state.softap
         //ssidap = state.ssidap
         document.getElementById("ip").value = state.ip

      })
  
}

   function initialize() {
         
   
   const query2 = `${baseHost}/status`
   fetch(query2)
      .then(function (response) {
         return response.json()
      })
      .then(function (state) {
         var x = state.toString()
         
         console.log(`ssidap: ${state.ssidap}`)
         
         document.getElementById("ssidap").value = state.ssidap
         ssidap = state.ssidap
         if (state.ssidapsecret == "true") {
           document.getElementById("SSIDAPsecret").checked = true 
           console.log(`true or secret ssidap`)
         } else {
          document.getElementById("SSIDAPsecret").checked = false
         }
         if (state.send_all_avi == "true") {
           document.getElementById("send_all_avi").checked = true 
           console.log(`true send all avi`)
         } else {
          document.getElementById("send_all_avi").checked = false
         }         
         if (state.raduauto == "true") {
           document.getElementById("RaduAuto").checked = true 
           console.log(`true or radu auto`)
         } else {
          document.getElementById("RaduAuto").checked = false
         }
         document.getElementById("passap").value = state.passap
         document.getElementById("ssid1").value = state.ssid1
         document.getElementById("ssid2").value = state.ssid2
         document.getElementById("ssid3").value = state.ssid3
         document.getElementById("pass1").value = state.pass1
         document.getElementById("pass2").value = state.pass2
         document.getElementById("pass3").value = state.pass3
         document.getElementById("telegramid").value = state.telegramid
         document.getElementById("telegrambot").value = state.telegrambot
         document.getElementById("frame").value = state.frame
         document.getElementById("timezone").value = state.timezone
                   
      })

      net_status()
      get_photo()
      
   }             // end of initialize
  
      </script>
   </head>


<body onload="initialize()" style="background-color: white">

  <div class="topnav">
    <h1>ESP32-CAM-Video-Telegram</h1><h1> Wi-Fi Config Manager</h1>
  </div>
  <div class="content">
    <div class="card-grid">
      <div class="card">
        <form action="/" method="POST">
          <p>
            <label for="ssidap">SSIDAP</label><input type="text" id ="ssidap" name="ssidap" value="deskpir">
            <label for="passap">password</label><input type="text" id ="passap" name="passap" value="12344321"><br>
            <input type="checkbox" id="SSIDAPsecret" name="SSIDAPsecret" >
            <label for="SSIDAPsecret"> Do not broadcast SSIDAP</label><br>
            <label for="ssid1">SSID1 </label><input type="text" id ="ssid1" name="ssid1">
            <label for="pass1">password</label><input type="text" id ="pass1" name="pass1"><br>
            <label for="ssid2">SSID2 </label><input type="text" id ="ssid2" name="ssid2">
            <label for="pass2">password</label><input type="text" id ="pass2" name="pass2"><br>
            <label for="ssid3">SSID3 </label><input type="text" id ="ssid3" name="ssid3">
            <label for="pass3">password</label><input type="text" id ="pass3" name="pass3"><br>           
            <input type="checkbox" id="send_all_avi" name="send_all_avi" >
            <label for="send_all_avi">Send all avi, even with SD</label><br>
            
            <label for="telegramid">Telegram ID </label><input type="text" id ="telegramid" name="telegramid" value="1234567890"><br>
            <label for="telegrambot">Telegram BOT</label><input type="text" id ="telegrambot" name="telegrambot" size=40 value="1234567890:1234asdf1234asdf1234asdf1234asdf123"><br><br>
        
            
            <label for="frame">Frame size:</label>
            <select name="frame" id="frame">
               <option value="VGA">VGA 640x480</option>
               <option value="HD">HD 1280x720</option>
            </select>
  
            <label for="timezone">Timezone </label><input type="text" id ="timezone" name="timezone" value="GMT"><br><br>
            <label for="sip">SoftAP IP </label><textarea id="softap" name="softap" rows="1" cols="50"></textarea><br>
            <label for="rip">Router IP </label><textarea id="ip" name="ip" rows="1" cols="50"></textarea><br>            
            <input type="hidden" id ="x" name="x" ><br>
            <!--<input type ="submit" value ="Submit">-->
            
          </p>
        </form>
        
        <button id="Bupdate">Save Parameters</button>
        <button id="Breboot">Reboot</button><br>
        <button id="Bnet_status">Refresh wifi connection status</button>
        <button id="Bstatus">Refresh stored eprom parameters</button><br><br>
        <button id="Bfileman">Download Files</button><br>
        <button id="Br0">Radu0</button>
        <button id="Br5">Radu5</button>
        <button id="Br10">Radu10</button>
        <button id="Br18">Radu18</button>
        <button id="Br24">Radu24</button>
        <button id="Br35">Rad35</button>
        <button id="Br50">Regular</button><br>
                    <input type="checkbox" id="RaduAuto" name="RaduAuto" >
            <label for="RaduAuto"> Automatic Radu Control</label><br><br>
        <button id="Btest">Send test msg to Telegram</button>
        <button id="Btestpic">Send test Picture to Telegram</button>
        <button id="Btestavi">Send test Avi to Telegram</button><br><br>
        <label for="testpic">Test Picture Below ... </label><br><br>
        http://192.168.4.1/capture or with 192.168.1.???/capture <br>
        http://192.168.4.1/test - msg to telegram<br>
        http://192.168.4.1/photo - photo to telegram<br>
        http://192.168.4.1/clip - clip to telegram<br><br>
        <img id="liveFeed" 
            color: white; position: relative"
            alt="There seems to be a problem with the capture..."/>
        
      </div>
    </div>
  </div>  
          <a href="https://github.com/jameszah/ESP32-CAM-Video-Telegram">https://github.com/jameszah/ESP32-CAM-Video-Telegram</a> <br>
          jameszah - Apr 17, 2024 -- ver 11.16.5<br>
          <a href="https://ko-fi.com/jameszah">Free coffee </a>
   </body>
 <script>


///////////////////
const Br0_Button = document.getElementById('Br0')
Br0_Button.onclick = () => {
   const query2 = `${baseHost}/radu?r=0`
  fetch(query2)
      .then(function (response) {
        console.log(response)
         return response.json()
      })
      .then(function (state) {
        console.log(state)          
      })
}
const Br5_Button = document.getElementById('Br5')
Br5_Button.onclick = () => {
   const query2 = `${baseHost}/radu?r=5`
  fetch(query2)
      .then(function (response) {
        console.log(response)
         return response.json()
      })
      .then(function (state) {
        console.log(state)          
      })
}
const Br10_Button = document.getElementById('Br10')
Br10_Button.onclick = () => {
   const query2 = `${baseHost}/radu?r=10`
  fetch(query2)
      .then(function (response) {
        console.log(response)
         return response.json()
      })
      .then(function (state) {
        console.log(state)          
      })
}
const Br18_Button = document.getElementById('Br18')
Br18_Button.onclick = () => {
   const query2 = `${baseHost}/radu?r=18`
  fetch(query2)
      .then(function (response) {
        console.log(response)
         return response.json()
      })
      .then(function (state) {
        console.log(state)          
      })
}
const Br24_Button = document.getElementById('Br24')
Br24_Button.onclick = () => {
   const query2 = `${baseHost}/radu?r=24`
  fetch(query2)
      .then(function (response) {
        console.log(response)
         return response.json()
      })
      .then(function (state) {
        console.log(state)          
      })
}
const Br35_Button = document.getElementById('Br35')
Br35_Button.onclick = () => {
   const query2 = `${baseHost}/radu?r=35`
  fetch(query2)
      .then(function (response) {
        console.log(response)
         return response.json()
      })
      .then(function (state) {
        console.log(state)          
      })
}
const Br50_Button = document.getElementById('Br50')
Br50_Button.onclick = () => {
   const query2 = `${baseHost}/radu?r=50`
  fetch(query2)
      .then(function (response) {
        console.log(response)
         return response.json()
      })
      .then(function (state) {
        console.log(state)          
      })
}
///////////////////

const Bfileman_Button = document.getElementById('Bfileman')
Bfileman_Button.onclick = () => {
    
    const query = `${baseHost}:8080`

    window.open(query, '_blank').focus();
      
}

const Breboot_Button = document.getElementById('Breboot')
Breboot_Button.onclick = () => {
    const query2 = `${baseHost}/reboot`
  
  fetch(query2)
      .then(function (response) {
        console.log(response)
         return response.json()
      })
      .then(function (state) {
        console.log(state)
          
      })
}

const Btest_Button = document.getElementById('Btest')
Btest_Button.onclick = () => {
    const query2 = `${baseHost}/test`
  
  fetch(query2)
      .then(function (response) {
        console.log(response)
         return response.json()
      })
      .then(function (state) {
        console.log(state)
          
      })
}

const Btestpic_Button = document.getElementById('Btestpic')
Btestpic_Button.onclick = () => {
    const query2 = `${baseHost}/photo`
  
  fetch(query2)
      .then(function (response) {
        console.log(response)
         return response.json()
      })
      .then(function (state) {
        console.log(state)
          
      })
}

const Btestavi_Button = document.getElementById('Btestavi')
Btestavi_Button.onclick = () => {
    const query2 = `${baseHost}/clip`
  
  fetch(query2)
      .then(function (response) {
        console.log(response)
         return response.json()
      })
      .then(function (state) {
        console.log(state)
          
      })
}

const Bstatus_Button = document.getElementById('Bstatus')
Bstatus_Button.onclick = () => {
  initialize()
}

const Bnet_status_Button = document.getElementById('Bnet_status')
Bnet_status_Button.onclick = () => {
  const query = `${baseHost}/net_status`
  console.log(query)

////
  const query2 = `${baseHost}/net_status`
  
  fetch(query2)
      .then(function (response) {
        console.log(response)
         return response.json()
      })
      .then(function (state) {
        console.log(state)
         //var x = state.toString()
         //console.log(`netstatus: ${x}`)
         
         
         document.getElementById("softap").value = state.softap
         //ssidap = state.ssidap
         document.getElementById("ip").value = state.ip

      })
}


const BupdateButton = document.getElementById('Bupdate')
BupdateButton.onclick = () => {
  let jdata = '{'
  jdata  = jdata + ' "ssidap":"' + document.getElementById('ssidap').value + '",'
  jdata  = jdata + ' "passap":"' + document.getElementById('passap').value + '",'
  jdata  = jdata + ' "ssidapsecret":"' + document.getElementById('SSIDAPsecret').checked + '",'
  jdata  = jdata + ' "send_all_avi":"' + document.getElementById('send_all_avi').checked + '",'  
  jdata  = jdata + ' "raduauto":"' + document.getElementById('RaduAuto').checked + '",'  
  jdata  = jdata + ' "ssid1":"' + document.getElementById('ssid1').value + '",'
  jdata  = jdata + ' "pass1":"' + document.getElementById('pass1').value + '",'
  jdata  = jdata + ' "ssid2":"' + document.getElementById('ssid2').value + '",'
  jdata  = jdata + ' "pass2":"' + document.getElementById('pass2').value + '",'
  jdata  = jdata + ' "ssid3":"' + document.getElementById('ssid3').value + '",'
  jdata  = jdata + ' "pass3":"' + document.getElementById('pass3').value + '",'
  jdata  = jdata + ' "telegramid":"' + document.getElementById('telegramid').value + '",'
  jdata  = jdata + ' "telegrambot":"' + document.getElementById('telegrambot').value + '",'
  jdata  = jdata + ' "timezone":"' + document.getElementById('timezone').value + '",'
  jdata  = jdata + ' "frame":"' + document.getElementById('frame').value + '"}'
  console.log (jdata)
  

  const query = `${baseHost}/update`
  console.log(query)
    
  fetch(query, {
    method: 'POST',
    headers: {
      'Accept': 'application/json',
      'Content-Type': 'appication/json'
    },
    body: jdata
    
  })
  .then(response => response.json())
  .then(response => console.log(JSON.stringify(response)))
  
}
</script>


</html>
)===";
   
