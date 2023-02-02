import socketio
import threading
import subprocess
import requests

from serial import Serial
from src.control.ControlVer2 import ControlVer2
from src.utils.accent import remove_accents

NAMESPACE = "/rasp"
arduino = Serial("COM3")
client = socketio.Client()
con = ControlVer2(arduino)

def run_srv():
  subprocess.run("node .")

def csrv_running():
  status = requests.get("http://localhost:3000/test")
  return status.status_code == 200

def receive(infor):
  ip = infor["ip"]
  print("Địa chỉ ip là {}".format(ip))
  con.sh_config(ip)

def recognize(infor):
  print("Câu đầu vào {}".format(infor))
  infor = remove_accents(infor)
  con.update_sen(infor)

def connect():
  client.on("receive-value", namespace=NAMESPACE, handler=receive)
  client.on("recognize", namespace=NAMESPACE, handler=recognize)

  # Đợi kết nối server
  print("Đang cố gắng kết nối đến server")  
  while not csrv_running():
    pass
  print("Kết nối thành công đến server")

  client.connect("http://localhost:3000", namespaces=[NAMESPACE])
  # print("Kết thúc dịch vụ io")


threading.Thread(name="Server", target=run_srv) \
  .start()

threading.Thread(name="Connect IO Server", target=connect) \
  .start()

try:
  while True:
    pass
except KeyboardInterrupt as e:
  print('Tiến hành thoát chương trình...')
  arduino.close()
  con.srv = False
  client.disconnect()
  print("Hoàn tất")