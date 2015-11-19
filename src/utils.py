#!/usr/bin/env python
import threading, numpy, pdb, sys, urllib2, cv2 #scipy, pygame
from PIL import Image
#from Myro import *
#from Graphics import *

# I/O
def load(filename):
	img = Image.open(filename)
	result = img.getdata()
	print img.format
	print img.size
	print img.mode
	return img
def take():
	camera = cv2.VideoCapture(0)
	retval, img = camera.read()
	return img
def show(img):
	img.show()

# conversions
def bgr(img):
	r, g, b, a = img.split()
	return b, g, r
def image(b,g,r):
	img = Image.merge("RGB", (b, g, r))
	return img
def like(img):
	return numpy.zeros_like(img)

# metadata
def bounds(RGB):
	return RGB.size
def channels(RGB):
	mode = 3
	if RGB.mode == 'L':
		mode = 4
	return mode
# transforms
def crop(img,coord):
	img.crop(coord)
	return img

def scale(img,scale):
	width,length = bounds(img)
	width = int(width * scale)
	length = int(length * scale)
	img.resize((width,length),Image.NEAREST)#BILINEAR|BICUBIC|ANTIALIAS
	return img

# colors
def color(img, x , y):
	img = img.load()
	return img[x,y]
def avail(img):
	return img.getcolors()

def colorPicker(given,x,y):
	box = (x-25,x+25,y-25,y+25)
	blank = crop(given,box)
	show(blank)
	given = given.load()
	print given[x,y]
	return given[x,y]


"""
# Kaicong notes
1. Username/Password variables to fill in.
2. Test visiting these two sites.
3. grab continuous images for processing.
## original
  * http://%s:81/livestream.cgi?user=%s&pwd=%s&streamid=3&audio=1&filename=
## 1602w
  * JPEG http://IPADDRESS/snapshot.cgi?user=[USERNAME]&pwd=[PASSWORD]
  * MJPEG http://IPADDRESS/videostream.cgi
  * MJPEG http://IPADDRESS/videostream.cgi?rate=0&user=[USERNAME]&pwd=[PASSWORD]
  * MJPEG http://IPADDRESS/videostream.cgi?user=[USERNAME]&pwd=[PASSWORD]&resolution=32&rate=0
## SIP 1602w
  * MJPEG http://IPADDRESS/videostream.cgi?user=[USERNAME]&pwd=[PASSWORD]&resolution=32&rate=0
  * JPEG http://IPADDRESS/img/snapshot.cgi?size=2
"""

class KaicongInput():
    
    def __init__(self, callback, domain, uri_format, packet_size, user="admin", pwd="123456"):
        """ domain:   Camera IP address or web domain 
                      (e.g. 385345.kaicong.info)
        """
        self.callback = callback
        self.running = False
        self.packet_size = packet_size
        self.uri = uri_format % (domain, user, pwd)
        self.stream = None
    
    def connect(self):
        print "Opening url: %s" % self.uri
        self.stream = urllib2.urlopen(self.uri)
        
        if not self.stream:
            raise Exception("Error connecting")
    
    def handle(self, data):
        pass
        
    def shutdown(self):
        self.running = False
    
    def read(self):
        result = None
        
        # Loop for things like Video, where multiple reads required to
        # retrieve a frame.
        while not result:
            result = self.handle(self.stream.read(self.packet_size))
        return result
    
    def run(self):
        try:
            if self.stream:
                self.stream.close()
        
            self.connect()
            self.running = True
            
            while self.running:
                result = self.handle(self.stream.read(self.packet_size))
                if result:
                    self.callback(result)
        
        finally:
            if self.stream:
                self.stream.close()
class KaicongOutput():
    
    def __init__(self, domain, uri_format, user="admin", pwd="123456"):
        """ domain:   Camera IP address or web domain 
                      (e.g. 385345.kaicong.info)
        """
        self.running = False
        self.uri = uri_format.format(domain, user, pwd)
class KaicongVideo(KaicongInput):
	PACKET_SIZE = 1024
	URI = "http://%s:81/livestream.cgi?user=%s&pwd=%s&streamid=3&audio=1&filename="
	
	def __init__(self, domain, callback, user="admin", pwd="123456"):
		KaicongInput.__init__(
			self, 
			callback,
			domain, 
			KaicongVideo.URI, 
			KaicongVideo.PACKET_SIZE, 
			user, 
			pwd
		)
		self.bytes = ''
	
	def handle(self, data):
		self.bytes += data
		a = self.bytes.find('\xff\xd8')
		b = self.bytes.find('\xff\xd9')
		if a!=-1 and b!=-1:
			jpg = self.bytes[a:b+2]
			self.bytes = self.bytes[b+2:]
			return jpg
	def show_video(jpg):    
		img = cv2.imdecode(numpy.fromstring(jpg, dtype=numpy.uint8),cv2.CV_LOAD_IMAGE_COLOR)
		cv2.imshow('i',img)
		# Note: waitKey() actually pushes the image out to screen
		if cv2.waitKey(1) ==27:
			exit(0)  
	
	#video = KaicongVideo(sys.argv[1], show_video)
	#video.run()
class KaicongMotor(KaicongOutput):
	
	# These are the original state commands to send to the Kaicong camera.
	CMDLIST = {
		"PTZ_UP": 0,
		"PTZ_UP_STOP": 1,
		"PTZ_DOWN": 2,
		"PTZ_DOWN_STOP": 3,
		"PTZ_LEFT": 4,
		"PTZ_LEFT_STOP": 5,
		"PTZ_RIGHT": 6,
		"PTZ_RIGHT_STOP": 7,
		"PTZ_LEFT_UP": 90,
		"PTZ_RIGHT_UP": 91,
		"PTZ_LEFT_DOWN": 92,
		"PTZ_RIGHT_DOWN": 93,
		"PTZ_STOP": 1,
		"PTZ_CENTER": 25,
		"PTZ_VPATROL": 26,
		"PTZ_VPATROL_STOP": 27,
		"PTZ_HPATROL": 28,
		"PTZ_HPATROL_STOP": 29,
		"IO_ON": 94, # TODO: What does this do?
		"IO_OFF": 95, # and this one?
	}
	
	# This table converts a vector-style direction to its command
	MOVELIST = {
		"00": "PTZ_STOP",
		
		"0+": "PTZ_UP",
		"0-": "PTZ_DOWN",
		
		"+0": "PTZ_RIGHT",
		"-0": "PTZ_LEFT",
		
		"++": "PTZ_RIGHT_UP",
		"+-": "PTZ_RIGHT_DOWN",
		"-+": "PTZ_LEFT_UP",
		"--": "PTZ_LEFT_DOWN",
	}
	
	URI = "http://{0}:81/decoder_control.cgi?loginuse={1}&loginpas={2}&command=%d&onestep=0"
	def __init__(self, domain, user="admin", pwd="123456"):
		KaicongOutput.__init__(
			self, 
			domain, 
			KaicongMotor.URI, 
			user, 
			pwd
		)
		
		self.state = '00'
		
	def _to_symbol(self, v):
		if v > 0:
			return '+'
		if v < 0:
			return '-'
		else:
			return '0'
	
	def send_command(self, cmdstr): 
		stream = urllib2.urlopen(self.uri % (KaicongMotor.CMDLIST[cmdstr]))
		result = stream.read()
		assert "ok" in result
		stream.close()
		
	def move(self, xy):
		move_symbol = self._to_symbol(xy[0]) + self._to_symbol(xy[1])
		cmdstr = KaicongMotor.MOVELIST[move_symbol]
		if cmdstr != self.state:
			self.send_command(cmdstr)
		self.state = cmdstr
	def checkKeys():
		keys = pygame.key.get_pressed()
		x = 0
		y = 0
		if keys [pygame.K_a]:
			x = -1
		if keys [pygame.K_s]:
			y = -1
		if keys [pygame.K_d]:
			x = 1
		if keys [pygame.K_w]:
			y = 1
			
		motor.move([x, y])
	
	def startKaicong():
		pygame.init()
		screen = pygame.display.set_mode((320, 240))
		
		motor = KaicongMotor(sys.argv[1])
		while 1:
			for event in pygame.event.get():
				if event.type == pygame.QUIT:
					 sys.exit()
			checkKeys()

def handleKeyPress(win, event):
    key=event.key
    offSet=20
    try:
        if str(key)=="Left":       
            if getRobot != None:
                motors(-5,5)
        elif str(key)=="Right":
            if getRobot != None:
                motors(5,-5)
        elif str(key)=="Up":
            if getRobot != None:
                leftValue,rightValue = getLine()
                print (leftValue,rightValue)
                if (leftValue == 1 and rightValue == 1):
                    stop()
                    motors(-1,-1)
                    wait(1)
                else:
                    motors(.5,.5)
        elif str(key)=="Down":
            if getRobot != None:
                leftValue,rightValue = getLine()
                print (leftValue,rightValue)
                if (leftValue == 1 and rightValue == 1):
                    stop()
                    motors(1,1)
                    wait(1)
                else:
                    motors(-.5,-.5)
    except(ValueError):
        pass
def handleKeyRelease(win, event):
    stop()
def runScribler():
    init("com13")
    sim = Simulation("sim",5,5, Color("White"))
    sim.setup()
    if getWindow != None:
        win = getWindow()
        win.onKeyPress(handleKeyPress)
        win.onKeyRelease(handleKeyRelease)