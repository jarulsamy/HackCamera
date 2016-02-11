#!/usr/local/bin/python
import cv2
 
# Read image
src = cv2.imread("samples/unitato.jpg", cv2.IMREAD_GRAYSCALE)
 
# Set threshold and maxValue
thresh = 0
maxValue = 255
 
# Basic threshold example
th, dst = cv2.threshold(src, thresh, maxValue, cv2.THRESH_BINARY);
print(th,dst)

cv2.imshow("Keypoints", th)
cv2.waitKey(0)

cv2.imshow("Keypoints", dst)
cv2.waitKey(0)