import cv2
#from SimpleCV import Image, Camera

camera = cv2.VideoCapture(0)
im = cv2.imread("filename.jpg", cv2.CV_LOAD_IMAGE_COLOR)
retval, im = camera.read()
#cv2.imwrite("filename.jpg",im)
cv2.namedWindow('Display Window')
cv2.imshow('Display Window', im)
cv2.waitKey(0)