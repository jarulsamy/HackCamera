import cv2, numpy
camera = cv2.VideoCapture(0)
im = cv2.imread("filename.jpg", cv2.CV_LOAD_IMAGE_COLOR)
retval, im = camera.read()
im = cv2.cvtColor(im,cv2.COLOR_BGR2RGB)
print(im.shape)
a,b,c = cv2.split(im)

z = numpy.zeros_like(a)
a = cv2.merge([z,z,a])
b = cv2.merge([b,z,z])
c = cv2.merge([z,c,z])

cv2.namedWindow('Display Window')
cv2.imshow('Display Window', im)
cv2.imshow("a",a)
cv2.imshow("b",b)
cv2.imshow("c",c)
cv2.waitKey(0)