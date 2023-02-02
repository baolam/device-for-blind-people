import time
import speech_recognition as sr

def callback(recognizer, audio):
  try:
    print("Đang nhận dạng")
    print(recognizer.recognize_google(audio, language='vi-VI'))
  except sr.UnknownValueError:
    print("Google Speech Recognition could not understand audio")
  except sr.RequestError as e:
    print("Could not request results from Google Speech Recognition service; {0}".format(e))

r = sr.Recognizer()
m = sr.Microphone()

with m as source:
  r.adjust_for_ambient_noise(m)

for _ in range(50): time.sleep(0.1)
stop_listening = r.listen_in_background(m, callback, phrase_time_limit=4)

while True: time.sleep(0.1)