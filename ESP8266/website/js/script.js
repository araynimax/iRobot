import socket from "./socket.js";

$(document).ready(function () {
    $('.awesome-spinner').html(
        '<div class="spinner-1"><div></div></div>' +
        '<div class="spinner-2"><div></div></div>' +
        '<div class="spinner-3"><div></div></div>' +
        '<div class="spinner-4"><div></div></div>'
    );

      document.querySelectorAll('.states ons-list ons-list-item ons-button').forEach((el,i)  => {
        el.addEventListener('click', () => {
          if(!socket.sendSocketMessage({setState: i})){
          alert('Keine Verbindung zum Websocket');
          };
        });
      });
});
