import cv2, numpy, scipy 
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

def getColor(img, x , y):
    r, g, b = cameron.imgArr(img)
    r = r[x , y]
    g = g[x , y]
    b = b[x, y]
    return r , g , b
RGB = imgLoad("colors.png")
def jgetColor(img):
    x = numpy.asarray(img)
    print x[400,400,2]
print getColor(RGB, 10, 100)
#print getColor(RGB, 600, 600)
#print getColor(RGB, 1000, 1000)

#jgetColor(RGB)

ran, ranY,z = cameron.imgShape(RGB)
mask = cameron.maskColor(RGB,1,200)
cameron.imgShow(mask,"I'm not crazy")
"""
for x in range (1 , ran-1):
    for y in range(1, ranY-1):
        try:
            r,g,b = getColor(RGB,x,y)
            if r < 255:
                print x, y, r, g , b
        except:
            print "I broke{},{}".format(x,y)
 """