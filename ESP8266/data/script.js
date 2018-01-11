$(document).ready(function () {
    $('.awesome-spinner').html(
        '<div class="spinner-1"><div></div></div>' +
        '<div class="spinner-2"><div></div></div>' +
        '<div class="spinner-3"><div></div></div>' +
        '<div class="spinner-4"><div></div></div>'
    );

    $('ons-range.ultrasonic_servo').change((elem) => {
      socket.send(elem.target.value);
    })
});
