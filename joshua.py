#!/usr/bin/env python
import cv2, numpy, scipy, pdb
from PIL import Image
import cameron



def imgLoad(filename):
    img = cv2.imread(filename, cv2.CV_LOAD_IMAGE_COLOR)
    return img

# takes images
def imgTake():
    camera = cv2.VideoCapture(0)
    retval, img = camera.read()
    return img

def maskColor(img,color,tolerance=5,wantColor=True):
    length, width = imgShape(img)
    result = numpy.zeros_like(img)

    for x in range(length):
        for y in range(width):
            if (abs(img[x,y] - color) < tolerance):
                #if the color is within range
                if wantColor:
                    result[x,y] = img[x,y]
                else: #otherwise, return white
                    result[x,y] = (255,255,255)
    return result

def imgDim(RGB):
    x = numpy.asarray(RGB).shape
    return x
def getColor(img, x , y):
    dim = imgDim(img)
    arr = numpy.asarray(img)
    
    r = arr[y , x, 0]
    g = arr[y , x, 1]
    b = arr[y , x, 2]
    return r , g , b
RGB = imgLoad("colors.png")
ranY, ran, z = imgDim(RGB)

def jgetColor(img):
    x = numpy.asarray(img)
    print x[400,400,2]
#print getColor(RGB, 10, 100)
#print getColor(RGB, 600, 600)
#print getColor(RGB, 1000, 1000)

#jgetColor(RGB)
RGB = imgLoad("colors.png")
#ran, ranY,z = imgShape(RGB)
#mask = cameron.maskColor(RGB,1,200)
#cameron.imgShow(mask,"I'm not crazy
mask = numpy.zeros_like(RGB)
print mask.shape
for x in range (1 , ran-1):
    for y in range(1, ranY-1):
        r,g,b = getColor(RGB,x,y)
        if r < 255:
            mask[y,x,0] = r
cameron.imgShow(mask, "uniato", True)