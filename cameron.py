#!/usr/bin/env python
import cv2, numpy, scipy


def imgLoad(filename): #loads images
	img = cv2.imread(filename, cv2.CV_LOAD_IMAGE_COLOR)
	return img

def imgTake(): #takes images
	camera = cv2.VideoCapture(0)
	retval, img = camera.read()
	return img

def imgShape(img): #get array shape
	arr = numpy.asarray(img)
	shape = arr.shape
	return arr.shape

def imgArr(img): #split as rgb arrays
	arr = numpy.asarray(img)
	r = arr[:,:,0]
	g = arr[:,:,1]
	b = arr[:,:,2]
	return r, g, b

def rgbImg(arr): # grayscale image from array
	z = numpy.zeros_like(arr)
	img = cv2.merge([z,z,arr])
	return img

def imgShow(img,name): # image, named window
	cv2.imshow(name,img)

def imgCrop(img,(sx,sy,lx,ly)): # tuple containing dimensions
	result[]; rx = -sx; ry = -sy; #offsets
	for y in range(sy,ly):
		for x in range(sx,lx):
			result[ry+y,rx+x] = img[y,x]
	return result


def imgScale(img,scale):

def scale1(im,scale):
    im = cv2.cv.fromarray(im)
    width,length,depth = im.shape
    for a in range(depth):
        for b in range(length):
            for c in range(width):
    size = cv2.cv.GetSize(im)
    thumbnail = cv2.cv.CreateImage( ( size[0] * scale, size[1] * scale))
    cv2.cv.Resize(im, thumbnail)
    #im = scipy.misc.imresize(im,scale)
    return(thumbnail)
z = numpy.zeros_like(im)
cv2.namedWindow('Display Window')
cv2.waitKey(0)
#print("Printing value at 100, 100{}".format(im[100,100]))
