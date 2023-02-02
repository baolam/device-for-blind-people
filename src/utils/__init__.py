import time

def delayMicroseconds(uc):
  # microsecond --> 10^6
  time.sleep(uc / 1000000.0)

def delay(uc):
  # Đơn vị là mili giây
  time.sleep(uc / 1000.0)

def pout(p):
  return 'd:{}:o'.format(p)