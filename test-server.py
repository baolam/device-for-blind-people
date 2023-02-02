import socketio
import threading

NAMESPACE = "/rasp"
client = socketio.Client()

def receive(infor):
  print(infor)

def recognize(infor):
  print(infor)

def connect():
  client.on("receive-value", namespace=NAMESPACE, handler=receive)
  client.on("recognize", namespace=NAMESPACE, handler=recognize)
  client.connect("http://localhost:3000", namespaces=[NAMESPACE])

threading.Thread(name="Connect IO Server", target=connect) \
  .start()

try:
  while True:
    pass
except KeyboardInterrupt as e:
  print(e)