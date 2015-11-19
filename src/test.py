#!/usr/bin/env python
from utils import (load, take, show, bgr, image, like, bounds,
channels, crop, scale, color, avail, colorPicker)
from proto import alias, sharpen, group, find, edge, center, distance
from PIL import Image

print "# fast stuff"
img = load('samples/abstract/colors.png')
#b = take()
show(img)
b, g, r = bgr(img)
img = image(b,b,b)
test = like(img)
bound = bounds(b)
channel = channels(b)
coord = (0,0,50,50)
closer = crop(img, coord)
bigger = scale(closer, 2.0)
eyedrop = color(img, 0, 30)
pallet = avail(img)
colorPicker(img,0,30)

print "# slow stuff"
res1 = alias(img, .3)
res2 = sharpen(img, .3)
blob1 = group(img)
mask = Image.new("RGB", (50, 10), "white")
blob3 = find(img,mask,(3,3))
coords1 = edge(img)
coords2 = center(blob1)
dist = distance(0,3)

print "# yay, got to the end!"