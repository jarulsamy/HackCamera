#!/usr/bin/env python
from utils import (load, take, show, bgr, image, like, bounds,
channels, crop, scale, color, avail, colorPicker)
#http://homepages.inf.ed.ac.uk/rbf/HIPR2/log.htm
#https://en.wikipedia.org/wiki/Unsharp_masking

# processing
def alias(img,amount):
	results = []
	length, width = bounds(img)
	channel = channels(img)
	post = avail(img)
	for ch in range(channel):
		print ch
		for color in post:
			print color
			mask = maskGrey(img,ch,color,20,255)
			mask = maskColor(image, mask,1)
			results.append(mask)
	return results
def sharpen(img,amount):#axis,layer,overlay=False
	length, width = shape(img)
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

	image = load("colors.png")
	colors = posturize(image)
	for color in colors:
		show(color)
	#print("Printing value at 100, 100{}".format(im[100,100]))

# blobbing
def group(img):#img,color,wantColor=True, channel, value, tolerance=5, resultColor=255,maskimg,color,tolerance=5,wantColor=True
	length, width = shape(img)
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
	length, width = shape(img)
	result = numpy.zeros_like(img)
	for x in range(length):
		for y in range(width):
			if (abs(img[x,y] - color) < tolerance):
				#if the color is within range
				if wantColor:
					result[x,y] = img[x,y]
				else: #otherwise, return white
					result[x,y] = (255,255,255)
	length, width = bounds(img)
	ch = channels(img)
	result = numpy.zeros_like(img)
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
	for x in range(length):
		for y in range(width):
			if (abs(img[x,y][channel] - value) < tolerance):
				#if the color is within range
				result[x,y] = resultColor
	length, width = bounds(img)
	ch = channels(img)
	result = numpy.zeros_like(img)
	for x in range(length):
		for y in range(width):
			if mask[x,y][channel] == 255:
				result[x,y][channel] = img[x,y][channel]
	return result
def find(img,mask):
	length, width = shape(img)
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

	image = load("colors.png")
	colors = posturize(image)
	for color in colors:
		show(color)
	#print("Printing value at 100, 100{}".format(im[100,100]))

# metadata
def edge(img):#img,axis,layer,overlay=False
	avails = set(list(np.flatten(image)))
	newImage = blank(hyper(image))
	for a in range(0,len(avails)):
		avail = avails[a]
		for x in range(0,maxX):
			for y in range(0,maxY):
				if avail == image[x][y]:
					newImage[a][x][y] == (255,255,255)
	#(x,y) (Xmin_max/2 Ymin_max/2) middle
	for img in newImg:
		show(img)
		result = 2
	point = area(x,y)# rgb
	for x,y in area:
		print "hi"
		#grab everything similar to that point
def center(blob):
	# Read image
	im = cv2.imread("blob.jpg", cv2.IMREAD_GRAYSCALE)
	# Set up the detector with default parameters.
	detector = cv2.SimpleBlobDetector()
	# Detect blobs.
	keypoints = detector.detect(im)
	# Draw detected blobs as red circles.
	# cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS ensures the size of the circle corresponds to the size of blob
	im_with_keypoints = cv2.drawKeypoints(im, keypoints, np.array([]), (0,0,255), cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS)
	 	# Show keypoints
	cv2.imshow("Keypoints", im_with_keypoints)
	cv2.waitKey(0)
def distance(point):
	print "hello"