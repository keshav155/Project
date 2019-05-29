# Import OpenCV2 for image processing
import cv2
import requests
# Import numpy for matrices calculations
import numpy as np
import RPi.GPIO as gpio
import smbus
import time
import sys
bus = smbus.SMBus(1)
address = 0x04

def send_notification(state):
    requests.post("https://maker.ifttt.com/trigger/"+state+"/with/key/3JrlvrtLyc-iwMhzhx6WE")
# Create Local Binary Patterns Histograms for face recognization
recognizer = cv2.face.LBPHFaceRecognizer_create()
# Load the trained mode
recognizer.read('trainer/trainer.yml')
proceed = False
proceed2 = False
proceed3 = False
# Load prebuilt model for Frontal Face
cascadePath = "haarcascade_frontalface_default.xml"

# Create classifier from prebuilt model
faceCascade = cv2.CascadeClassifier(cascadePath);

# Set the font style
font = cv2.FONT_HERSHEY_SIMPLEX

# Initialize and start the video frame capture
cam = cv2.VideoCapture(0)

def main5():
    bus.write_byte(address, 87)
# Loop
while True:
    # Read the video frame
    try:
        ret, im =cam.read()

        # Convert the captured frame into grayscale
        gray = cv2.cvtColor(im,cv2.COLOR_BGR2GRAY)

        # Get all face from the video frame
        faces = faceCascade.detectMultiScale(gray, 1.2,5)

        # For each face in faces
        for(x,y,w,h) in faces:

            # Create rectangle around the face
            cv2.rectangle(im, (x-20,y-20), (x+w+20,y+h+20), (0,255,0), 4)

            # Recognize the face belongs to which ID
            Id = recognizer.predict(gray[y:y+h,x:x+w])

            # Check the ID if exist 
            if(Id[0] == 1 and Id[1] < 120):
                Id = "Keshav"
                send_notification("owner")
                proceed = True
            #If not exist, then it is Unknown
            elif(Id[0] == 2 and Id[1] < 120):
                Id = "Arya"
                send_notification("guest")
                proceed2 = True
            elif(Id[0] != 1 or 2):
                Id = "Unknown"
                proceed3 = True
            else:
                proceed4  = True

            # Put text describe who is in the picture
            cv2.rectangle(im, (x-22,y-90), (x+w+22, y-22), (0,255,0), -1)
            cv2.putText(im, str(Id), (x,y-40), font, 2, (255,255,255), 3)

        # Display the video frame with the bounded rectangle
        cv2.imshow('im',im) 

        # If 'q' is pressed, close program
        if cv2.waitKey(10) & 0xFF == ord('q'):
            break
    except KeyboardInterrupt:
        print('Interrupted')
        main4()
        sys.exit(0)
        

# Stop the camera
cam.release()

# Close all windows
cv2.destroyAllWindows()

def main():
    gpio.setmode(gpio.BCM)
    gpio.setup(17, gpio.OUT)
    status = False
    while True:
        gpio.output(17, status)
        status = not status
        bus.write_byte(address, 1)
        time.sleep(1)
def main2():
        bus.write_byte(address, 2)

def main3():
    bus.write_byte(address, 0)

def main4():
    bus.write_byte(address, 8)

if __name__ == '__main__':
    try:
        if(proceed):
            print("owner access granted")
            main()
        elif(proceed2):
            print("guest detected")
            main2()
        else:
            print("No guests/auhorized personnel were detected")
            main4()

            
            
    except KeyboardInterrupt or OSError:
            print('Check your connections')
            main()
            gpio.cleanup()
            sys.exit(0)


