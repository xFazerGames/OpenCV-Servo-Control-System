# OpenCV Servo Control System

## Description

This Python script uses OpenCV for object tracking and serial communication to control an external device based on the position of a detected object. The object of interest is tracked based on its blue color in a video feed, and the corresponding movement commands are sent via a serial connection to control a device (e.g., robot or motor).

## Requirements

- Python 3.x
- OpenCV (`cv2`) for image and video processing
- NumPy for array manipulation
- PySerial for serial communication with external devices

### Installation

To install the required libraries, run:

```
pip install opencv-python numpy pyserial
```
### Code Explanation
#### 1. Importing Libraries

```
import cv2
import numpy as np
import serial
```
- `cv2`: This library is used for image and video processing. OpenCV provides several tools for working with real-time video, image manipulation, and computer vision.
- `numpy`: This is a powerful library for numerical operations. It's used here for creating arrays and handling image data.
- `serial`: This library allows us to communicate with an external device over a serial connection (e.g., Arduino or a motor controller).
#### 2. Setting Up Serial Communication

```
COM = '/dev/cu.usbserial-14120'
BAUD = 9600
ser = serial.Serial(COM, BAUD)
```
- `COM`: This is the serial port where the external device (e.g., motor controller) is connected. In this case, the port is set to `/dev/cu.usbserial-14120`, which is a typical path for macOS, but this could change depending on your operating system and device.
- `BAUD`: The baud rate for serial communication, set to 9600 bps, which is the standard rate for many devices.
- `ser`: This is the initialized serial connection. It is used to send data (movement commands) to the external device.

#### 3. Setting Up Camera 

```
cap = cv2.VideoCapture(2)
```

- `cap`: This initializes the camera capture. `2` represents the index of the camera, which might differ depending on your system. For example, `0` is often the default camera on a laptop.

#### 4. Defining the Blue Color Range (HSV)

```
azulBajo = np.array([90, 100, 20], np.uint8)
azulAlto = np.array([120, 255, 255], np.uint8)
```
- `azulBajo` and `azulAlto`: These are the lower and upper bounds for the color blue in the HSV (Hue, Saturation, Value) color space. This range is used to isolate the blue color from the image.
- `azulBajo`: The lower bounds for the blue color.
- `azulAlto`: The upper bounds for the blue color.
Using HSV is often preferred for color-based segmentation, as it separates color intensity from color information.

#### 5. Main Loop: Processing Video Frames

```
while True:
    ret, frame = cap.read()
    if ret:
        frame = cv2.flip(frame, 1)
        frameHSV = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
        mascara = cv2.inRange(frameHSV, azulBajo, azulAlto)
        contornos, _ = cv2.findContours(mascara, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
        cv2.drawContours(frame, contornos, -1, (255, 0, 0), 4)
```
- `while True`: This creates an infinite loop to continuously process frames from the camera.
- `cap.read()`: This reads the next frame from the camera.
- `cv2.flip(frame, 1)`: Flips the frame horizontally to create a mirror image, which can be useful for user-facing cameras.
- `cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)`: Converts the captured frame from the default BGR color format to HSV format.
- `cv2.inRange(frameHSV, azulBajo, azulAlto)`: This creates a binary mask, where the pixels that fall within the specified blue color range are set to 255 (white) and all other pixels are set to 0 (black).
- `cv2.findContours(mascara, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)`: Finds the contours of the blue areas in the mask. Contours represent the boundaries of objects in an image.
- `cv2.drawContours(frame, contornos, -1, (255, 0, 0), 4)`: Draws the contours on the original frame (in red color).

#### 6. Contour Detecton and Centroid Calculation

```
for c in contornos:
    area = cv2.contourArea(c)
    if area > 6000:
        M = cv2.moments(c)
        if M["m00"] == 0:
            M["m00"] = 1
        x = int(M["m10"] / M["m00"])
        y = int(M['m01'] / M['m00'])
        cv2.circle(frame, (x, y), 7, (0, 0, 255), -1)
        font = cv2.FONT_HERSHEY_SIMPLEX
        cv2.putText(frame, '{},{}'.format(x, y), (x + 10, y), font, 1.2, (0, 0, 255), 2, cv2.LINE_AA)
        nuevoContorno = cv2.convexHull(c)
        cv2.drawContours(frame, [nuevoContorno], 0, (255, 0, 0), 3)
```

- This loop iterates over all the detected contours.
- `cv2.contourArea(c)`: Calculates the area of each contour.
- `if area > 6000`: Only processes contours with an area larger than 6000 pixels to avoid tracking noise or small objects.
- `cv2.moments(c)`: Calculates the moments of a contour, which are used to find the centroid.
x, y: These are the coordinates of the centroid of the contour.
- `cv2.circle(frame, (x, y), 7, (0, 0, 255), -1)`: Draws a red circle at the calculated centroid.
- `cv2.putText(frame, '{},{}'.format(x, y), (x + 10, y), font, 1.2, (0, 0, 255), 2, cv2.LINE_AA)`: Displays the centroid's coordinates on the frame.
- `cv2.convexHull(c)`: Calculates the convex hull of the contour (the smallest convex boundary that encloses the contour).
- `cv2.drawContours(frame, [nuevoContorno], 0, (255, 0, 0), 3)`: Draws the convex hull on the frame.

#### 7. Movement Commands Based on Centroid Position

```
if x < 200:
    print("Moving left 100%")
    ser.write(b"izq1\n")
elif 420 > x >= 200:
    print("Moving left 60%")
    ser.write(b"izq2\n")
elif 520 > x >= 420:
    print("Moving left 30%")
    ser.write(b"izq3\n")
elif 520 <= x < 650:
    print("Moving to center")
    ser.write(b"ctr\n")
elif 650 <= x < 860:
    print("Moving right 30%")
    ser.write(b"der3\n")
elif 860 <= x < 1080:
    print("Moving right 60%")
    ser.write(b"der2\n")
elif x >= 1080:
    print("Moving right 100%")
    ser.write(b"der1\n")
```
- Based on the x-coordinate of the centroid, the external device receives a command to move left, right, or to the center.
- The device moves at different speeds depending on the position of the centroid in the x-axis of the frame:
- `izq1`: Move left 100%
- `izq2`: Move left 60%
- `izq3`: Move left 30%
- `ctr`: Move to center
- `der1`: Move right 100%
- `der2`: Move right 60%
- `der3`: Move right 30%

#### 8. Exiting the Program 

```
if cv2.waitKey(1) & 0xFF == ord('s'):
    ser.close()
    break
```

- This allows the user to exit the program by pressing the 's' key. It also closes the serial connection before exiting the loop.

#### 9. Cleanup

```
cap.release()
cv2.destroyAllWindows()
```
- Releases the video capture object and closes all OpenCV windows.
