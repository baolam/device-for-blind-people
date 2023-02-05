$(document).ready(() => {
  let ip = prompt("Nhập địa chỉ server: ");

  while(ip.length == 0)
    ip = prompt("Nhập địa chỉ server: ");
  
    const user = io(`http://${ip}:3000/user`);

  user.on("notification", (d) => {
    alert(d);
  });
  
  $('#myModal').modal('show');

  var SpeechRecognition = SpeechRecognition || webkitSpeechRecognition;
  const recognition = new SpeechRecognition();

  // Cấu hình bộ nhận dạng
  recognition.lang = 'vi-VN';
  recognition.continious = true;
  recognition.interimResults = true;

  recognition.onresult = function (event) {
    let interim_transcript = "", final_transcript = "";
    for (let i = event.resultIndex; i < event.results.length; ++i) {
      if (! event.results[i].isFinal) {
        final_transcript += event.results[i][0].transcript;;
      } else {
        interim_transcript += event.results[i][0].transcript;
      }
    }
    if (interim_transcript.length === 0)
      return;
    $('#script').val(interim_transcript);
  }

  recognition.onspeechend = function()
  {
    recognition.stop();
  }

  recognition.onerror = function(event) {
    console.log(event.error);
  }

  $('#start').click(() => {
    recognition.start();
  });

  $('#end').click(() => {
    recognition.stop();
  });

  $('#send').click(() => {
    let infor = $('#script').val();
    $('#script').val('');
    user.emit("result", infor);
  });
});