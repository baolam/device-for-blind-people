# -*- coding: utf-8 -*-

import socketio
import threading
import subprocess
import requests
import shlex

from serial import Serial
from src.control.ControlVer2 import ControlVer2
from src.utils.accent import remove_accents
from src.utils.print import _print

NAMESPACE = "/rasp"
# arduino = Serial("COM3")
arduino = Serial()
client = socketio.Client()
con = ControlVer2(arduino)

def run_srv():
  shell_cmd = "node index.js"
  cmd = shlex.split(shell_cmd)
  subprocess.run(cmd)

def csrv_running():
  status = requests.get("http://localhost:3000/test")
  return status.status_code == 200

def receive(infor):
  ip = infor["ip"]
  _print("Địa chỉ ip là {}".format(ip))
  con.sh_config(ip)

def recognize(infor):
  _print("Câu đầu vào {}".format(infor))
  infor = remove_accents(infor)
  code = con.update_sen(infor)

  if code == False:
    client.emit("notification", "Câu của bạn đã được lưu vào file, sẽ được nạp xuống thiết bị thực thi sau khi quá trình hiện chữ hoàn tất", namespace=NAMESPACE)

def connect():
  client.on("receive-value", namespace=NAMESPACE, handler=receive)
  client.on("recognize", namespace=NAMESPACE, handler=recognize)

  # Đợi kết nối server
  _print("Đang cố gắng kết nối đến server")  
  while not csrv_running():
    pass
  _print("Kết nối thành công đến server")

  client.connect("http://localhost:3000", namespaces=[NAMESPACE])
  # _print("Kết thúc dịch vụ io")


threading.Thread(name="Server", target=run_srv) \
  .start()

threading.Thread(name="Connect IO Server", target=connect) \
  .start()

try:
  while True:
    pass
except KeyboardInterrupt as e:
  _print('Tiến hành thoát chương trình...')
  arduino.close()
  con.srv = False
  client.disconnect()
  _print("Hoàn tất")