import threading
import os
import time
from serial import Serial

FORWARD = "TF"
BACKWARD = "TB"
LEFT = "TL"
RIGHT = "TR"
TESTCO = "TG"
SENS = "SE"
RESP = "RE"
START = "ST"
LOAD_SEN = "LO"
REQUEST_CONFIG = "RC"

class ControlVer2:
  FILE = "./static/project.txt"

  def __init__(self, arduino : Serial):
    self.arduino = arduino

    self.srv = True
    self.ard = False
    threading.Thread(name="Arduino service", target=self.read) \
      .start()
    
    # Câu hiện tại
    self.current_sens = ""
    self.ip = None

    # Cập nhật câu đã lưu
    self.load_sen()
  
  @staticmethod
  def write(arduino, command):
    print("Lệnh gửi sang arduino {}".format(command))
    arduino.write(bytes(command, "utf-8"))

  def read(self):
    while self.srv:
      command = self.arduino.readline().decode("utf-8") \
        .replace("\r\n", "")
      
      if command == RESP:
        # Gửi chuỗi câu sang cho thiết bị
        print("Đang nạp câu mới")
        self.write(self.arduino, self.current_sens + '#')
      
      if command == START:
        print("Arduino đã sẵn sàng")
        self.ard = True

      if command == LOAD_SEN:
        if self.current_sens != "":
          print("Tải câu đã lưu trữ {}".format(self.current_sens))
          self.update_sen(self.current_sens)

      if command == REQUEST_CONFIG:
        print("Yêu cầu lấy cài đặt")
        if self.ard and self.ip != None:
          self.write(self.arduino, self.ip + '#')

      if command[0] == 'D':
        print("Phản hồi từ thiết bị {}".format(command[1:]))

    # print("Kết thúc dịch vụ Arduino")
  
  def update_sen(self, sen : str):
    '''
      Gọi phương thức này để cập nhật câu mới
    '''
    self.current_sens = sen.upper()
    self.write(self.arduino, SENS + '#')

    with open(self.FILE, "w", encoding="utf-8") as f:
      f.write(self.current_sens)
  
  def sh_config(self, ip):
    '''
      Gọi hàm này để hiển thị dữ liệu trên LCD
    '''
    self.ip = ip
  
  def load_sen(self):
    if os.path.exists(self.FILE):
      with open(self.FILE, "r", encoding="utf-8") as f:
        sen = f.readline().replace("\r\n", "")
        # self.update_sen(sen)
        self.current_sens = sen
        print("Đọc câu đã lưu trữ: {}".format(sen))
      