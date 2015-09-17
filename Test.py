from Myro import *
from Graphics import *

#init("com13")

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
    
sim = Simulation("sim",5,5, Color("White"))
sim.setup()

    
                
if getWindow != None:
    win = getWindow()
    win.onKeyPress(handleKeyPress)
    win.onKeyRelease(handleKeyRelease)
    