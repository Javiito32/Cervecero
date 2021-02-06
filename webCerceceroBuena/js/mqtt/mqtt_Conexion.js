var hostname = "127.0.0.1";
var port = 9001;
var clientId = "Web";

client = new Paho.MQTT.Client(hostname, port, clientId);

// set callback handlers
client.onConnectionLost = onConnectionLost;
client.onMessageArrived = onMessageArrived;

// connect the client
client.connect({onSuccess:onConnect});


// called when the client connects
function onConnect() {
  // Once a connection has been made, make a subscription and send a message.
  console.log("onConnect");
  client.subscribe("webCervecero/sonda/2");
  client.subscribe("webCervecero/arduino/2");
  //message = new Paho.MQTT.Message("Hello");
  //message.destinationName = "World";
  //client.send(message);
}

// called when the client loses its connection
function onConnectionLost(responseObject) {
  if (responseObject.errorCode !== 0) {
    console.log("onConnectionLost:"+responseObject.errorMessage);
  }
}

// called when a message arrives
function onMessageArrived(message) {
  console.log(message.destinationName);
  let payload = JSON.parse(message.payloadString);
  switch (message.destinationName) {
    case "webCervecero/sonda/2":
      
      temp = document.getElementById('temp');
      temp.innerText = payload.temp + " º" + payload.temp_Unit;
      break;

    case "webCervecero/arduino/2":

    
      $('#processProgress').attr('aria-valuenow', payload.porcentaje).css('width', payload.porcentaje + "%");
      $('#progress').text(payload.porcentaje + "%");
      //processProgress.aria-valuenow;
      break;

  }
  //console.log("onMessageArrived:"+message.payloadString);
}