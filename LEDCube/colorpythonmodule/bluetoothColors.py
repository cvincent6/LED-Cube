#!usr/bin/Python
#import serial
from time import sleep


class testSerial:
    def write(self, string):
        print('[SERIAL DEBUG]: ' + string)

#Send the color over the given serial connection
#Default color: 0x000000 (black/off)
#Serial x String [boolean] -> Void
##Serial: a serial connection
##color: newline terminated 3 byte hex code for color ('0x######\n')
##verbose: does the function print what it's doing? (Default to false)
def sendColor(serial, color='0x000000\n', verbose=False):
    serial.write(color)
    if(verbose):
        print('Sent color:' + color)

#Return a hex string for a byte. if the byte is one character long, add a 0
def hexLong(x):
    if(len(x) < 2):
        return ('0' + x).upper()
    else:
        return x.upper()

#Convert the input hex values to a valid color string
# Hex x Hex x Hex -> String
def rgb_to_color(r,g,b):
    r = hexLong(r)
    g = hexLong(g)
    b = hexLong(b)
    return '0x'+str(r)+str(g)+str(b)+'\n'


#Make the box transition from the first color to the second color
# Serial x Color x Color x timestep x boolean -> void
def transition(serial, iColor, nColor, tStep=0.5, steps=50, verbose=False):

    #Convert input initial color to int values
    ir = abs(int(iColor[2:4],16))
    ig = abs(int(iColor[4:6],16))
    ib = abs(int(iColor[6:8],16))

    #Convert input new color to int values
    nr = abs(int(nColor[2:4],16))
    ng = abs(int(nColor[4:6],16))
    nb = abs(int(nColor[6:8],16))
    
    #Find the difference in intial vs new values
    dr = nr - ir
    dg = ng - ig
    db = nb - ib
    # Define the steps
    rStep = dr/float(steps)
    gStep = dg/float(steps)
    bStep = db/float(steps)
    if(verbose):
        print (rStep, gStep, bStep)
    
    #loop through and perform transition
    for i in range(steps+1):
        r = int(ir + rStep*i)
        g = int(ig + gStep*i)
        b = int(ib + bStep*i)
        
        #Pull of the added '0x's
        if(verbose):
            print (r,g,b)
        # Create a string version of the number
        rH = hex(r)
        gH = hex(g)
        bH = hex(b)

        if(verbose):
            print (rH,gH,bH)

        if(r < 0):
            r = hex(r)[3:5]
        else:
            r = hex(r)[2:4]
        
        if(g < 0):
            g = hex(g)[3:5]
        else:
            g = hex(g)[2:4]

        if(b < 0):
            b = hex(b)[3:5]
        else:
            b = hex(b)[2:4]
        # Generate the proper string
        color = rgb_to_color(r,g,b)
        # send it out
        sendColor(serial, color, verbose)
        # wait
        sleep(tStep)

    sendColor(serial, nColor, verbose)

#Uncomment for debugging.
t = testSerial()
c1 = '0x000000\n'
c2 = '0xFFFFFF\n'
