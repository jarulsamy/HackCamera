#!/usr/bin/env python
from utils import (load, take, show, bgr, img, like, dim,
bounds, channels, crop, scale, color, avail, colorPicker)
from proto import alias, sharpen, group, find, edge, center, distance

img = load('samples/colors.png')
b = take()
b, g, r = bgr(img)
Blue = img(b)
show(Blue)
Teal = like(img)
print dim(b)
print bounds(b)
print channels(b)
coord = (0,0,50,50)
closer = crop(img, coord)
bigger = scale(closer, 2.0)
eyedrop = color(img, 0, 30)
pallet = avail(img)
colorPicker(img,0,30)

res1 = alias(img, .3)
res2 = sharpen(img, .3)
blob1 = group(img)
blob3 = find(img, mask)
coords1 = edge(img)
coords2 = center(blob1)
dist = distance(0,3)


print "no tests made"