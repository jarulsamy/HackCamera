#!/usr/bin/env python
import cv2, numpy, scipy 
from PIL import Image

"""
    Example:
    from cameron import *
    cats = imgLoad('cats.jpg')
    selfie = imgTake()
    dim = imgShape(cats)
    catr,catg,catb = imgArr(cats)
    imgr = rgbImg(catr)
    imgShow(imgr,"Red Layer of cats.jpg",True) # True is 'wait'

"""

# loads images
def imgLoad(filename):
    img = cv2.imread(filename, cv2.CV_LOAD_IMAGE_COLOR)
    return img

# takes images
def imgTake():
    camera = cv2.VideoCapture(0)
    retval, img = camera.read()
    return img

# get channel number
def channels(img):
    tup = numpy.asarray(img).shape
    chanNum = tup[2]
    return chanNum

def bounds(img):
	x,y,z = numpy.asarray(img).shape
	return x, y

# split as rgb arrays
def imgArr(img):
    arr = numpy.asarray(img)
    r = arr[:,:,0]
    g = arr[:,:,1]
    b = arr[:,:,2]
    return r, g, b

# grayscale image from array
def rgbImg(arr):
    z = numpy.zeros_like(arr)
    img = cv2.merge([z,z,arr])
    return img


# image, named window
def imgShow(img,name="unitato",wait=True):
    cv2.imshow(name, img)
    if wait == True:
        cv2.waitKey(0)

# tuple containing dimensions
def imgCrop(img,(sx,sy,lx,ly)):
    result = []; rx = -sx; ry = -sy; #offsets

    for y in range(sy,ly):
        for x in range(sx,lx):
            result[ry+y,rx+x] = img[y,x]
    return result

# scaling using interpolation libraries
def imgScale(img,scale):
    width,length,depth = img.shape
    width = width * scale
    height = height * scale
    img.resize((width,height),Image.NEAREST)#BILINEAR|BICUBIC|ANTIALIAS
    return img

# print available colors
def rgbAvail(arr):
	result = []
	length, width = bounds(arr)
	for x in range(length):
		for y in range(width):
			result.extend(arr[x,y])
	result = set(result)
	return result

# mask out things based on color
def maskGrey(img, channel, value, tolerance=5, resultColor=255):
    length, width = bounds(img)
    ch = channels(img)
    result = numpy.zeros_like(img)

    for x in range(length):
        for y in range(width):
            if (abs(img[x,y][channel] - value) < tolerance):
                #if the color is within range
                result[x,y] = resultColor
    return result
def maskColor(img,mask,channel):
    length, width = bounds(img)
    ch = channels(img)
    result = numpy.zeros_like(img)
    for x in range(length):
        for y in range(width):
            if mask[x,y][channel] == 255:
                result[x,y][channel] = img[x,y][channel]
    return result

def posturize(img):
	results = []
	length, width = bounds(img)
	channel = channels(img)
	post = rgbAvail(img)
	for ch in range(channel):
		print ch
		for color in post:
			print color
			mask = maskGrey(img,ch,color,20,255)
			mask = maskColor(image, mask,1)
			results.append(mask)
	return results

def maskImage(img,mask,tolerance=5,wantColor=True):
    length, width = bounds(img)
    mlength, mwidth = bounds(mask)
    # we can't use the edges around the image
    sl = round(mlength / 2); sw = round(mwidth / 2);
    ml = mlength - sl; mw = mwidth - sw;
    for l in range(sl,ml):
        for w in range(sw,mw):
            # the edges are now eliminated
            for il in range(-1,1):
                for iw in range(-1,1):
                    print "hello"
                # compare each part of the mask

def imgDelta(img,axis,layer,overlay=False):
    length, width = imgShape(img)
    for l in range(1,length-1):
        for w in range(1,width-1):
            pixel = img[l,w][layer]
        # exclude one pixel around the image
        result = []
        for a in range(-1,1):
            for b in range(-1,1):
                result[a,b] = pixel - img[l+a,w+b][layer]
        # average the results into the red channel
        for a in range(-1,1):
            for b in range(-1,1):
                result[a,b] = pixel - img[l+a,w+b][layer]
        # average the results into the green channel
        # put back into image or overlay score?
#def img
# pull


if __name__ == "__main__":
	image = imgLoad("colors.png")
	colors = posturize(image)
	for color in colors:
		imgShow(color)
    #print("Printing value at 100, 100{}".format(im[100,100]))



















