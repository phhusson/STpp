import sys, pygame
import re
from math import cos, sin, pi

pygame.init()

size = width, height = 1024, 768
speed = [2, 2]
black = 0, 0, 0
white = 255, 255, 255

screen = pygame.display.set_mode(size, pygame.HWSURFACE | pygame.DOUBLEBUF )
screen_array = pygame.PixelArray(screen)

serial = open("/dev/ttyACM0", "r")

def to_xy(rho, theta):
    #Assume height is shortest
    rho = int(rho, 16)
    theta = int(theta, 16)
    rho = rho*(height/2)/4000
    x = width/2 + rho*cos(theta*pi/180)
    y = height/2 + rho*sin(theta*pi/180)
    if x < 0 or x >= width:
        x = 0
    if y < 0 or y >= height:
        y = 0
    return int(x), int(y)

while 1:
    for event in pygame.event.get():
        if event.type == pygame.QUIT: sys.exit()

    screen.fill(black)
    got = {}
    for i in range(1, 360):
        line = serial.readline()
        res = re.match('#(0x[0-9a-f]{8}),(0x[0-9a-f]{8})(.*)', line)
        if not res:
            line = serial.readline()
            res = re.match('#(0x[0-9a-f]{8}),(0x[0-9a-f]{8})(.*)', line)

        if res:
            xy = to_xy(res.group(2), res.group(1))
            screen_array[xy] = white
            got[int(res.group(1), 16)] = True

    print got

    pygame.display.flip()
    print "New frame !!!!!!!!"
