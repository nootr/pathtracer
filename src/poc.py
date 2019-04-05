#!/usr/bin/env python3

# Note: This is a PoC for a golfcode-like C program, so expect no pythonicness
# here. Thanks.
from math import sqrt as sqrtf
import random

class Vec(object):
    def __init__(self, a=0, b=0, c=0):
        self.x = a
        self.y = b
        self.z = c

    def __add__(self, other):
        return Vec(self.x+other.x, self.y+other.y, self.z+other.z)

    def __mul__(self, other):
        return Vec(self.x*other.x, self.y*other.y, self.z*other.z)

    def __mod__(self, other):
        return self.x*other.x + self.y*other.y + self.z*other.z

    def __invert__(self, other): # This will be `operator!` in C, not ~
        return self * (1 / sqrtf(self % self) )

# Override python's min() with restrictions to make porting easier
def min(l, r):
    if l < r:
        return l
    return r

def randomVal():
    return random.uniform(0, 1)

# CSG equations; returns negative if p is inside shape
def distanceCube(position, lowerLeft, upperRight):
    lowerLeft = lowerLeft + (position * Vec(-1,-1,-1))
    upperRight = position + (upperRight * Vec(-1,-1,-1))
    return -min(min(min(-lowerLeft.x, -upperRight.x),
                   min(-lowerLeft.y, -upperRight.y)),
               min(-lowerLeft.z, -upperRight.z))

def distanceSphere(position, center, radius):
    delta = center + (position * Vec(-1,-1,-1))
    distance = sqrtf(delta % delta)
    return radius - distance

# Let's test the CSG equations
print('Inside cube: {}'.format(distanceCube(Vec(2,2,2), Vec(1,1,1), Vec(3,3,3))))
print('Outside cube: {}'.format(distanceCube(Vec(0,0,0), Vec(1,1,1), Vec(3,3,3))))
print('Far outside cude: {}'.format(distanceCube(Vec(10,10,10), Vec(1,1,1), Vec(3,3,3))))

# Fetch distance of closest object
def closestObject(position):
    obj = 'None'
    dist = 1e9

    roomDist = -min( # A combination of:
                #  * the room
                distanceCube(position, Vec(-25,0,-30), Vec(40,20,30)),
                #  * "outside"
                distanceCube(position, Vec(-30,5,-25), Vec(-24,10,25))
                )
    if roomDist < dist:
        dist = roomDist
        obj = "WALL"

    sunDist = position.x + 29
    if sunDist < dist:
        dist = sunDist
        obj = "SUN"

    return obj, dist

# Let's test the closestObject function
print('Close to the wall: {}'.format(closestObject(Vec(39,19,29))))
print('Close to the light: {}'.format(closestObject(Vec(-28,7,0))))
print('In window: {}'.format(closestObject(Vec(-25,7,0))))


