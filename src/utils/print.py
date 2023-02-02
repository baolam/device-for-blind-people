import datetime

def _print(sen):
  now = datetime.datetime.now()
  r = '{}:{}:{} - {}'.format(now.hour, now.minute, now.second, sen)
  print(r)