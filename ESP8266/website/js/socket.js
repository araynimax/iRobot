let socket;

let startWebSocket = () => {
  console.log("Connecting to websocket @'ws://" + location.host + ":81'..");
  $('.loadingScreen > .text').removeClass('failed');

  socket=new WebSocket("ws://" + location.host || '192.168.2.100' + ":81");

  socket.onopen = () => {
    console.log("Websocket successfully connected to 'ws://" + location.host + ":81'.");
    $('.loadingScreen').addClass("hidden");
  };

  socket.onmessage = event => {
    console.log("Received a message from server", event.data);
    try {
      handleWebsocketMessage(JSON.parse(event.data));
    } catch (ex) {
      console.error("Failed to parse message from server to json!", ex);
    }
  };

  socket.onclose = () => {
    console.log("WebSocket connection closed! Reconnectin in 5s.");
    $('.loadingScreen').removeClass("hidden");
    $('.loadingScreen > .text').addClass('failed');
    setTimeout(startWebSocket, 5000);
  };

  socket.onerror = (err) => {
    console.log("WebSocket error!", err);
  };


}

let sendSocketMessage = arg => {
  if (socket !== undefined && socket.readyState === 1) {
    console.log("Sending a message to the server", arg);
    socket.send(JSON.stringify(arg));
    return 1;
  } else {
    console.error("WebSocket connection is not established!");
    return 0;
  }
}

let handleWebsocketMessage = data => {

  if (data.currentState !== undefined) {
    document.querySelectorAll('.states ons-list ons-list-item ons-button').forEach(el => el.classList.add("button--quiet"));
    document.querySelectorAll('.states ons-list ons-list-item ons-button')[data.currentState].classList.remove("button--quiet");
  }

  for (let key in data) {
    if ($('#robot-info #' + key).length) {
      $('#robot-info #' + key).text(data[key]);
    } else {
      $('#robot-info').append('<ons-list-header>' + key + '</ons-list-header>');
      $('#robot-info').append('<ons-list-item><span id="' + key + '">' + data[key] + '</span></ons-list-item>');
    }
  }
};

startWebSocket();

export default {
  sendSocketMessage,
};
