import sys, pygame
import re
from math import cos, sin, pi, sqrt

pygame.init()

size = width, height = 1500, 1000
speed = [2, 2]
black = 0, 0, 0

screen = pygame.display.set_mode(size, pygame.HWSURFACE | pygame.DOUBLEBUF )
screen_array = pygame.PixelArray(screen)

serial = open("/dev/ttyACM1", "r")

class Struct:
    def __init__(self, **entries): 
        self.__dict__.update(entries)

def to_xy(rho, theta):
    #Assume height is shortest
    theta = -theta+180
    x = rho*cos(theta*pi/180)
    y = rho*sin(theta*pi/180)

    return int(x), int(y)

def to_screen((x, y)):
    x = x*(height/2)/2500
    y = y*(height/2)/2500

    x += width/2
    y += height/2

    if x < 0 or x >= width:
        x = y = 0
    if y < 0 or y >= height:
        x = y = 0
    return int(x), int(y)

def connex(infos):
    matrix = {}
    nextComposante = 1
    matrix[0] = 0
    composantes = {}
    composantes[0] = { 'start': 0, 'end': -1 }
    for theta in infos:
        ok = True
        if theta > 0:
            #10cm delta ?
            previous = theta - 1
            while not infos.has_key(previous):
                print "Missing " + str(previous)
                previous = previous - 1
                if previous < 0:
                    previous = previous + 360
            if abs(infos[theta]-infos[previous]) < 150:
                matrix[theta] = matrix[previous]
            else:
                composantes[nextComposante-1]['end'] = theta - 1
                composantes[nextComposante-1] = Struct(**composantes[nextComposante-1])
                composantes[nextComposante] = { 'start' : theta, 'end': -1 }
                matrix[theta] = nextComposante
                nextComposante = nextComposante + 1

    if abs(infos[359] - infos[0]) < 150:
        matrix[0] = matrix[359]
        i = 0
        while matrix[i] == 0:
            matrix[i] = matrix[359]
            i = i + 1

        composantes[0].start = composantes[nextComposante-1]['start']
        del composantes[nextComposante-1]
    else:
        composantes[nextComposante-1] = Struct(**composantes[nextComposante-1])
        composantes[nextComposante-1].end = 359
    
    objs = []
    for i in composantes:
        try:
            #if abs(composantes[i].end - composantes[i].start) > 3:
            start = composantes[i].start
            end = composantes[i].end
            start_xy = to_xy(infos[start], start)
            end_xy = to_xy(infos[end], end)
            obj = { 'src': start_xy, 'dst': end_xy }
            dist = sqrt((start_xy[0]-end_xy[0])**2 + (start_xy[1]-end_xy[1])**2)
            obj["different"] = False
            if dist > 60 and dist < 120:
                print "Object @" + str(start) + " size is " + str(dist)
                obj["different"] = True
                
            objs.append(Struct(**obj))
        except:
            print "Composante " + str(i) + " failed"
            pass

    
    return objs



while 1:
    for event in pygame.event.get():
        if event.type == pygame.QUIT: sys.exit()

    screen.fill(black)
    center = width/2, height/2
    found = 255, 255, 255
    warning = 255, 0, 0

    infos = {}

    for i in range(0, 360):
        line = serial.readline()
        res = re.match('#(0x[0-9a-f]{8}),(0x[0-9a-f]{8})(.*)', line)
        if not res:
            line = serial.readline()
            res = re.match('#(0x[0-9a-f]{8}),(0x[0-9a-f]{8})(.*)', line)

        if res:
            infos[int(res.group(1), 16)] = int(res.group(2), 16)
            xy = to_xy(int(res.group(2), 16), int(res.group(1), 16))
            if res.group(3).startswith('!'):
                pygame.draw.line(screen, warning, center, to_screen(xy))
            else:
                pygame.draw.line(screen, found, center, to_screen(xy))
            #screen_array[xy] = white

    objects = connex(infos)
    object_color1 = 0, 255, 0
    object_color2 = 255, 255, 0
    for i in objects:
        if i.different:
            pygame.draw.line(screen, object_color2, to_screen(i.src), to_screen(i.dst))
        else:
            pygame.draw.line(screen, object_color1, to_screen(i.src), to_screen(i.dst))


    pygame.display.flip()
    print "New frame !!!!!!!!"
