from pyfirmata import Arduino
from pyfirmata import util

from ..utils import pout
from ..utils import delayMicroseconds
from ..utils import delay

'''
  int PUL1 = 7; // Sliding
  int DIR1 = 6; //
  int ENA1 = 5; //
  //
  int PUL2 = 4; // Rotating
  int DIR2 = 3; //
  int ENA2 = 2; //
  //
  Sơ đồ chân điều khiển
'''

LOW = 0
HIGH = 1

class Control:
  def __init__(self, arduino : Arduino, 
    PUL1 = 7, DIR1 = 6, ENA1 = 5, PUL2 = 4, DIR2 = 3, ENA2 = 2, inButton = 8):
    self.arduino = arduino
    # Phần chương trình này nằm ở một điều kiện riêng
    it = util.Iterator(arduino)
    it.start()

    # Khai báo cho động cơ 1
    self.PUL1 = arduino.get_pin(pout(PUL1))
    self.DIR1 = arduino.get_pin(pout(DIR1))
    self.ENA1 = arduino.get_pin(pout(ENA1))

    # Khai báo cho động cơ 2
    self.PUL2 = arduino.get_pin(pout(PUL2))
    self.DIR2 = arduino.get_pin(pout(DIR2))
    self.ENA2 = arduino.get_pin(pout(ENA2))

    # Khai báo nút bấm
    self.inButton = arduino.get_pin('d:{}:i'.format(inButton))
    # Trạng thái hoạt động của máy
    self.ready = 0

  def forward(self, step, dl=200):
    '''
      for (int i = 0; i < 800; i++) //Forward
      {
        digitalWrite(DIR1, LOW);
        digitalWrite(ENA1, HIGH);
        digitalWrite(PUL1, HIGH);
        delayMicroseconds(200);
        digitalWrite(PUL1, LOW);
        delayMicroseconds(200);
      }
    '''
    for i in range(step):
      self.DIR1.write(LOW)
      self.ENA1.write(HIGH)
      self.PUL1.write(HIGH)
      delayMicroseconds(dl)
      self.PUL1.write(LOW)
      delayMicroseconds(dl)

  def backward(self, step, dl=200):
    '''
      for (int i = 0; i < 800; i++) //Backward
      {
        digitalWrite(DIR1, HIGH);
        digitalWrite(ENA1, HIGH);
        digitalWrite(PUL1, HIGH);
        delayMicroseconds(200);
        digitalWrite(PUL1, LOW);
        delayMicroseconds(200);
      }
    '''
    for i in range(step):
      self.DIR1.write(HIGH)
      self.ENA1.write(HIGH)
      self.PUL1.write(HIGH)
      delayMicroseconds(dl)
      self.PUL1.write(LOW)
      delayMicroseconds(dl)

  def left(self, step, dl=200):
    '''
      for (int i = 0; i < 800; i++) // Rotate left
      {
        digitalWrite(DIR2, HIGH);
        digitalWrite(ENA2, HIGH);
        digitalWrite(PUL2, HIGH);
        delayMicroseconds(200);
        digitalWrite(PUL2, LOW);
        delayMicroseconds(200);
      }
    '''
    for i in range(step):
      self.DIR2.write(HIGH)
      self.ENA2.write(HIGH)
      self.PUL2.write(HIGH)
      delayMicroseconds(dl)
      self.PUL2.write(LOW)
      delayMicroseconds(dl)

  def right(self, step, dl=200):
    '''
      for (int i = 0; i < 800; i++) // Rotate right
      {
        digitalWrite(DIR2, LOW);
        digitalWrite(ENA2, HIGH);
        digitalWrite(PUL2, HIGH);
        delayMicroseconds(200);
        digitalWrite(PUL2, LOW);
        delayMicroseconds(200);
      }
    '''
    for i in range(step):
      self.DIR2.write(LOW)
      self.ENA2.write(HIGH)
      self.PUL2.write(HIGH)
      delayMicroseconds(dl)
      self.PUL2.write(LOW)
      delayMicroseconds(dl)
  
  def loop(self, inputChar):
    Nextword = 0
    Rolling = 0
    inputString = [''] * 50
    stringIndex = 1
    moveStep = 1
    spacePos = [0] * 10
    spaceIndex = 1
    lastIndex = 1
    spaceMark = 1

    # Phục vụ quá trình hiển thị mã
    Column0 = [0] * 100
    Column1 = [0] * 100

    # Khai báo bắt đầu
    for char in inputChar:
      if char != '$':
        inputString[stringIndex] = char
        # -----
        # Chương trình LD
        # -----

        if char == ' ':
          spacePos[spaceIndex] = stringIndex
          spaceIndex+=1
        
        stringIndex+=1
    
    Column0[0] = 0
    Column1[1] = 0

    Nextword = self.inButton.read()
    if Nextword == 1:
      self.ready = 1
      
    while self.ready == 1 and spaceMark <= spaceIndex:
      # -----
      # Chương trình LCD
      # -----

      if Rolling != 0:
        # -----
        # LCD
        # -----

        for j in range(moveStep, 1, -1):
          self.forward(4000, dl=100)
          moveStep-=1
          delay(200)

          # rotate column back to 0
          # column1 first
          if Column1[moveStep] != Column0[moveStep]:
            if Column1[moveStep] > Column0[moveStep]:
              for m in range(0, Column1[moveStep] - Column0[moveStep]):
                self.right(800, dl=100)
            # -----
            elif Column1[moveStep] < Column0[moveStep]:
              for m in range(0, Column0[moveStep]-Column1[moveStep]):
                self.left(800, dl=100)
          
          delay(200)
          # -----
          self.forward(4000, dl=100)
          # -----
          delay(200)

          if Column0[moveStep] != Column1[moveStep - 1]:
            if Column0[moveStep] > Column1[moveStep - 1]:
              for m in range(0, Column0[moveStep]-Column1[moveStep - 1]):
                self.right(800, dl=100)
            elif Column0[moveStep] < Column1[moveStep - 1]:
              for m in range(0, Column1[moveStep - 1]-Column0[moveStep]):
                self.left(800, dl=100)
        
        Rolling = 0

      # -----
      # Chương trình LCD
      # -----

      # //
      for k in range(lastIndex, spacePos[spaceMark]):
        # ----
        # Chương trình LCD
        # ----

        # self.decode(inputString[lastIndex])
        key = inputString[lastIndex]
        if key == 'A':
          Column0[moveStep] = 4
          Column1[moveStep] = 0
        if key == 'B':
          Column0[moveStep] = 6
          Column1[moveStep] = 0
        if key == 'C':
          Column0[moveStep] = 4
          Column1[moveStep] = 4
        if key == 'D':
          Column0[moveStep] = 4
          Column1[moveStep] = 6
        if key == 'E':
          Column0[moveStep] = 4
          Column1[moveStep] = 2
        if key == 'F':
          Column0[moveStep] = 6
          Column1[moveStep] = 4
        if key == 'G':
          Column0[moveStep] = 6
          Column1[moveStep] = 6
        if key == 'H':
          Column0[moveStep] = 6
          Column1[moveStep] = 2
        if key == 'I':
          Column0[moveStep] = 2
          Column1[moveStep] = 4
        if key == 'K':
          Column0[moveStep] = 5
          Column1[moveStep] = 0
        if key == 'L':
          Column0[moveStep] = 7
          Column1[moveStep] = 0
        if key == 'M':
          Column0[moveStep] = 5
          Column1[moveStep] = 4
        if key == 'N':
          Column0[moveStep] = 5
          Column1[moveStep] = 6
        if key == 'O':
          Column0[moveStep] = 5
          Column1[moveStep] = 2
        if key == 'P':
          Column0[moveStep] = 7
          Column1[moveStep] = 4
        if key == 'Q':
          Column0[moveStep] = 7
          Column1[moveStep] = 6
        if key == 'R':
          Column0[moveStep] = 7
          Column1[moveStep] = 2
        if key == 'S':
          Column0[moveStep] = 3
          Column1[moveStep] = 4
        if key == 'T':
          Column0[moveStep] = 3
          Column1[moveStep] = 6
        if key == 'U':
          Column0[moveStep] = 5
          Column1[moveStep] = 1
        if key == 'V':
          Column0[moveStep] = 7
          Column1[moveStep] = 1
        if key == 'W':
          Column0[moveStep] = 2
          Column1[moveStep] = 7
        if key == 'X':
          Column0[moveStep] = 5
          Column1[moveStep] = 5
        if key == 'Y':
          Column0[moveStep] = 5
          Column1[moveStep] = 7
        if key == 'Z':
          Column0[moveStep] = 5
          Column1[moveStep] = 3

        # Rotate Column0
        # Compare with preious
        if Column0[moveStep] != Column1[moveStep - 1]:
          # Smaller dots
          if Column0[moveStep] < Column1[moveStep - 1]:
            for n in range(0, Column1[moveStep - 1]-Column0[moveStep]):
              self.right(800, dl=100)
          # Greater dots
          elif Column0[moveStep] > Column1[moveStep - 1]:
            for n in range(0, Column0[moveStep]-Column1[moveStep - 1]):
              self.left(800, dl=100)
            
        # ---
        delay(200)
        # Move 1 step
        self.backward(4000, dl=100)
        # ---
        delay(200)
        # Rotate Column1
        if Column1[moveStep] != Column0[moveStep]:
          if Column1[moveStep] > Column0[moveStep]:
            for n in range(0, Column1[moveStep]-Column0[moveStep]):
              self.left(800, dl=100)
          #
          elif Column1[moveStep] < Column0[moveStep]:
            for n in range(0, Column0[moveStep]-Column1[moveStep]):
              self.right(800, dl=100)
        #
        delay(200)
        # Move 1 step
        self.backward(4000, dl=100)

        # Increase plate counting
        moveStep += 1
        lastIndex += 1
        Rolling = 1

      # 
      self.ready = 0
      lastIndex = spacePos[spaceMark] + 1
      spaceMark += 1