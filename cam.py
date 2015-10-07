import cv2, numpy, scipy
#camera = cv2.VideoCapture(0)
im = cv2.imread("RGB", cv2.CV_LOAD_IMAGE_COLOR)

#retval, im = camera.read()
#first = im[1,:]
#second = im[:,1]
third = numpy.asarray(im) 
print(third.shape)

red = third[:,:,0]
green = third[:,:,1]
blue = third[:,:,2]
      #third[:,:])
#im = cv2.cvtColor(im,cv2.COLOR_BGR2RGB)
#print("this shape is{}".format(im.shape))
#a,b,c = cv2.split(im)
def scale1(im,scale):
    im = cv2.cv.fromarray(im)
    width,length,depth = im.shape 
    image = 
    for b in range(length):
        for c in range(width):
            if c%2 is 0:
                if b%2 is 0:
                    
                
    size = cv2.cv.GetSize(im)
    thumbnail = cv2.cv.CreateImage( ( size[0] * scale, size[1] * scale))
    cv2.cv.Resize(im, thumbnail)
    #im = scipy.misc.imresize(im,scale)
    return(thumbnail)
z = numpy.zeros_like(im)
a = cv2.merge([z,z,red])
b = cv2.merge([z,z,green])
c = cv2.merge([z,z,blue])
a = scale1(a,.5)
b = scale1(b,.5)
c = scale1(c,.5)

cv2.namedWindow('Display Window')
cv2.imshow('Display Window', im)

cv2.imshow("Red",red)
cv2.imshow("Green",green)
cv2.imshow("Blue",blue)
cv2.waitKey(0)
#print("Printing value at 100, 100{}".format(im[100,100]))
