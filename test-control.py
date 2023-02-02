from serial import Serial
from src.control.ControlVer2 import ControlVer2

arduino = Serial("COM3")
con = ControlVer2(arduino)

try:
  while True:
    n = input("Test: ")
    con.write(arduino, n)
except KeyboardInterrupt as e:
  con.srv = False
  arduino.close()