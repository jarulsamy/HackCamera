#!/usr/bin/env python
from utils import (load, take, show, bgr, image, like, bounds,
channels, crop, scale, color, avail, colorPicker)
#http://homepages.inf.ed.ac.uk/rbf/HIPR2/log.htm
#https://en.wikipedia.org/wiki/Unsharp_masking
#http://www.effbot.org/imagingbook/image.htm
#http://pythonvision.org/basic-tutorial/
import cv2
import numpy as np
from PIL import Image

# processing
def alias(img,amount): #cheating with a mask
	length, width = bounds(img)
	channel = channels(img)
	post = avail(img)
	mask = [[0,0,0],[255,255,255],[0,0,0],
			[255,255,255],[255,255,255],[255,255,255],
			[0,0,0],[255,255,255],[0,0,0]]
	result = find(img,mask,(3,3))
	return result
def sharpen(img,amount): #not actually sharpening
	result = like(img)
	length, width = bounds(img)
	img = img.load()
	result = result.load()
	for left in range(1,length-1):
		for top in range(1,width-1):
			pixel = [0,0,0,0]
			for l in range(-1,1):
				for w in range(-1, 1):
					pixel[0] += img[left+l,top+w][0]
					pixel[1] += img[left+l,top+w][1]
					pixel[2] += img[left+l,top+w][2]
			result[left,top] = tuple(pixel)
	return result

# blobbing
def group(img):#automatically
	length, width = bounds(img)
	result = like(img).load()
	img = img.load()
	color = [255,255,255,255]
	tolerance = [2,2,2,2]
	for inc in range(1,127): # this is really really slow / iterative
		for x in range(length):
			for y in range(width):
				matches = 0
				if (abs(img[x,y][0] - color[0]) < tolerance):
					matches += 1
				if (abs(img[x,y][1] - color[1]) < tolerance):
					matches += 1
				if (abs(img[x,y][2] - color[2]) < tolerance):
					matches += 1
				if matches > 3:
					result[x,y] = tuple(color)
	return result
def find(img,mask,dims): # this isn't actually blobbing anything
	length, width = bounds(img)
	height, breadth = dims
	result = like(img)
	img = img.load()
	height = int(height / 2)
	breadth = int(breadth / 2)
	for l in range(height,length-height):
		for w in range(breadth,width-breadth):
			pixel = img[l,w]
	return result

# metadata
def edge(img):
	avails = avail(img)
	result = like(img).load()
	length,width = bounds(img)
	img = img.load()
	for a in range(0,len(avails)):
		av = avails[a]
		for x in range(0,length):
			for y in range(0,width):
				if av == img[x,y]:
					result[x,y] == av
	return result
def center(blob):
	im = cv2.imread("samples/abstract/colors.png", cv2.IMREAD_GRAYSCALE)
	detector = cv2.SimpleBlobDetector()
	keypoints = detector.detect(im)
	im_with_keypoints = cv2.drawKeypoints(im, keypoints, np.array([]), (0,0,255), cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS)
	cv2.imshow("Keypoints", im_with_keypoints)
	cv2.waitKey(0)
def distance(img,point):
	result = (0,0,0) # 3d grid
	return result