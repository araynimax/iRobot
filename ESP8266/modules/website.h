const char html[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="de">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, height=device-height, initial-scale=1">
<title>ESP8266 Web Server</title>
<style type="text/css">
body {font-family: Verdana; font-size: 12px; color: black; background-color: white}
h0 {font-family: Verdana; font-size: 16px; color: blue; background-color: white}
button {font-family: Verdana; font-size: 12px; margin: 12px; color: white}
</style>

<script language="JavaScript">
'use strict'
 var ip = location.host;
 let connection = null;
 function start(){
  connection=new WebSocket("ws://"+ip+":81");
  //connection=new WebSocket("ws://192.168.2.100:81");
  connection.onopen = function(){
    connection.send('Message from Browser to ESP8266 yay its Working!! ' + new Date());
    connection.send('ping');
  };
 connection.onerror = function(error){
   console.log('WebSocket Error ', error);
 };
 connection.onclose = function(){
   console.log('Websocket closed!');
   check();//reconnect now
 };
 connection.onmessage = function(e){
   if (connection.readyState === 1) {   //Verbindung wurde hergestellt
     let jsObj = JSON.parse(e.data);
       document.getElementById("light_left").innerHTML = jsObj.light_left;
       document.getElementById("light_right").innerHTML = jsObj.light_right;
       document.getElementById("bumper_left").innerHTML = !jsObj.bumper_left;
       document.getElementById("bumper_right").innerHTML = !jsObj.bumper_right;
   }
  };
 }
 function check(){
    if(!connection || connection.readyState == 3) start();
  }

 start();
 setInterval(check, 1000);

function sendCMD(Value){
  connection.send(Value);
}

</script>

</head>

<body>
  <h0>ESP</h0>
  <p>Data received from the MEGA128</p>
  Light links:    <span id="light_left">...</span><br>
  Licht rechts: <span id="light_right">...</span><br>
    Bumper Left:    <span id="bumper_left">...</span><br>
 Bumper Right <span id="bumper_right">...</span><br>
</body>
</html>
)=====";
/**/
