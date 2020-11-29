#import serial 
#arduino = serial.Serial('com1',9600)
#print("Enter  0: left or 1: right or 2: down and 3 up to move the snake")
#while 1:
#    detafromUser = input()
#    if detafromUser == '0':
#        arduino.write(b'0')
#        print("Move Left")
#    if detafromUser == '1':
#        arduino.write(b'1')
#        print("Move right")
#    if detafromUser == '2':
#        arduino.write(b'2')
#        print("Move Down")
#    if detafromUser == '3':
#        arduino.write(b'3')
#        print("Move Up")
#arduino.close()      
import serial 
arduino = serial.Serial('com1',9600)
import speech_recognition as sr 
print("Say right, left, up or down to control the snake:")
mic_name = "Microphone (Realtek High Defini"
sample_rate = 48000
chunk_size = 2048
r = sr.Recognizer() 
mic_list = sr.Microphone.list_microphone_names() 
for i, microphone_name in enumerate(mic_list): 
    if microphone_name == mic_name: 
        device_id = i 
with sr.Microphone(device_index = device_id) as source: #, sample_rate = sample_rate,  chunk_size = chunk_size
    r.adjust_for_ambient_noise(source, duration=0.5) 
while 1:
    with sr.Microphone(device_index = device_id, sample_rate = sample_rate,  
                            chunk_size = chunk_size) as source:              
        print("Say Something")
        audio = r.listen(source) 
        try: 
            text = r.recognize_google(audio) 
            print("you said: " + text)                     
            if text == 'left'or text == 'lift'or text == 'list'or text == 'next':
                arduino.write(b'0')
                print("Move Left")
            if text == 'right'or text == 'lights':
                arduino.write(b'1')
                print("Move Right")
            if text == 'down':
                arduino.write(b'2')
                print("Move Down")
            if text == 'up' or text == 'off':
                arduino.write(b'3')
                print("Move Up")
        except sr.UnknownValueError: 
            print("Google Speech Recognition could not understand audio") 
          
        except sr.RequestError as e: 
            print("Could not request results from Google Speech Recognition service; {0}".format(e))
            
arduino.close()

  
