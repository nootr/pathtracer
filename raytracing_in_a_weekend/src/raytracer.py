#!/usr/bin/env python3

## raytracer.py
#
# A simple PoC raytracer which is meant to be ported to C++ to size the code
# down to a business card, so don't expect too much pythonical code here.
#

import random
import time
import logging
import argparse
import math

class Vec(object):
    """Vec

    A Vec object is mainly used to represent a vector, but can also be used to
    represent a color or position.
    """
    def __init__(self, a=0, b=0, c=0):
        self.x = a
        self.y = b
        self.z = c

    def __add__(self, other):
        return Vec(self.x+other.x, self.y+other.y, self.z+other.z)

    def __mul__(self, other):
        try:
            result = Vec(self.x*other.x, self.y*other.y, self.z*other.z)
        except:
            result = Vec(self.x*other, self.y*other, self.z*other)
        return result

    def __mod__(self, other):
        return self.x*other.x + self.y*other.y + self.z*other.z

    def __pow__(self, other):
        return Vec(
                self.y * other.z - self.z * other.y,
                self.z * other.x - self.x * other.z,
                self.x * other.y - self.y * other.x
                )

    def __invert__(self): # This will be `operator!` in C, not ~
        denom = (1 / math.sqrt(self % self))
        return self * denom

    def __repr__(self):
        return '({},{},{})'.format(self.x, self.y, self.z)

class Ray(object):
    """Ray

    Represents a line in 3D space which follows the function p(t) = A + t * B,
    where the ray is a line with its origin at A and normalized direction B
    containing point p at distance t from the origin.
    """
    def __init__(self, origin, direction, world, bounces=4):
        self.origin = origin                # Ray origin
        self.direction = ~direction         # Ray direction (normalized)
        self.world = world                  # List of objects
        self.bounces = bounces              # Number of bounces left
        self.distance_treshold = 0.01       # Distance needed to hit object
        self.steps = 0                      # Stepcounter for tracing
        self.steps_treshold = 10000         # Number of allowed steps
        self.distance = 0                   # Distance traveled along ray
        self.color = Vec(0,0,0)             # Color which is seen
        self.attenuation = 1.0              # Color attenuation

    @property
    def point(self):
        """point

        Returns the point on the ray after the set distance is traveled.
        """
        return self.origin + self.direction * self.distance

    @property
    def closest_object(self):
        """closest_object

        Returns the closest object.
        """
        closest = { 'distance': 1e9, 'object': None }
        for obj in self.world:
            distance = obj.distance(self.point)
            if distance < closest['distance']:
                closest['distance'] = distance
                closest['object'] = obj

        return closest['object']

    def trace(self):
        """trace

        Traces the ray until it hits an object, then returns the hitpoint and
        object.
        """
        while self.steps < self.steps_treshold:
            # Fetch closest object
            obj = self.closest_object
            if not obj:
                logging.error('Could not determine closest object')
                exit(1)

            # Take a step
            self.distance += obj.distance(self.point)
            self.steps += 1

            # Check if the current point is close enough or else take another step
            if obj.distance(self.point) < self.distance_treshold:
                break

        return obj

    def bounce(self):
        """bounce

        Traces the ray until it hits an object. It then adjusts the color and -
        if there are bounces left - calculates a new position and direction,
        changes the attenuation and starts over.
        """
        # Travel to hitpoint
        obj = self.trace()

        # Update color
        self.color = self.color * (1-self.attenuation) + obj.color * self.attenuation

        # Update values
        self.bounces -= 1
        self.attenuation *= obj.attenuation
        self.distance -= 0.1
        self.origin = self.point
        self.distance = 0
        self.direction = obj.bounce(self.direction, self.point)

        # Bounce
        if self.bounces and self.attenuation > 0.05:
            self.bounce()

        # Return color
        return self.color

class WorldObject(object):
    """WorldObject

    An object which contains information about its position, size and the way
    light may bounce off of its surface.
    """
    def __init__(self, position, size, color, attenuation):
        self.position = position
        self.size = size
        self.attenuation = attenuation
        self.color = color

    def distance(self, point):
        raise NotImplemented

    def normal(self, point):
        return ~(point + self.position * -1)

    def bounce(self, vector, point):
        raise NotImplemented

class Sphere(WorldObject):
    def __init__(self, position, radius, color, attenuation=0.3, inverse=False):
        super().__init__(position, radius, color, attenuation)
        self._inverse = inverse

    def distance(self, point):
        delta = self.position + point * -1
        if self._inverse:
            return self.size - math.sqrt(delta % delta)
        else:
            return math.sqrt(delta % delta) - self.size

    def normal(self, point):
        if self._inverse:
            return ~(self.position + point * -1)
        else:
            return ~(point + self.position * -1)

    def bounce(self, vector, point):
        x = random.random()*2 - 1
        y = random.random()*2 - 1
        z = random.random()*2 - 1
        return ~(self.normal(point) * 1.1 + Vec(x,y,z))

class MetalSphere(Sphere):
    def __init__(self, position, radius, color):
        super().__init__(position, radius, color, 0.3)

    def bounce(self, vector, point):
        x = random.random() - 0.5
        y = random.random() - 0.5
        z = random.random() - 0.5
        return Vec(x,y,z)*0.1 + vector + self.normal(point) * (vector %
                self.normal(point)) * -2

class Box(WorldObject):
    def __init__(self, lowerLeft, upperRight, color, attenuation=0.3, inverse=False):
        super().__init__(lowerLeft, None, color, attenuation)
        self._inverse = inverse
        self._upperRight = upperRight

    def distance(self, point):
        lowerLeft = self.position + point * -1
        upperRight = point + (self.position + self._upperRight) * -1
        distance = -min(min(min(-lowerLeft.x, -upperRight.x),
                        min(-lowerLeft.y, -upperRight.y)),
                    min(-lowerLeft.z, -upperRight.z))
        if self._inverse:
            return distance * -1
        else:
            return distance

    def bounce(self, vector, point):
        x = random.random()*2 - 1
        y = random.random()*2 - 1
        z = random.random()*2 - 1
        return ~(self.normal(point) * 1.1 + Vec(x,y,z))

def parse_args():
    parser = argparse.ArgumentParser(description='Raytracer')
    parser.add_argument('filename', action='store', help='Output file')
    parser.add_argument('-d', '--debug', action='store_true',
            help='Enable debug logs')
    return parser.parse_args()

def run(filename):
    start_time = time.time()
    eta = 'unknown'

    h = 600
    samples = 4

    aspect = 4/3
    aperture = 10.0

    origin = Vec(-1, 2, 3)
    goal = Vec(0, 0.5, -1)
    distance = math.sqrt((origin + goal * -1) % (origin + goal * -1))

    w = ~(origin + goal * -1)
    u = ~(Vec(0,1,0) ** w)
    v = w ** u

    lower_left_corner = u * (-aspect) + v * -1 + w * -1
    horizontal = u * 2 * aspect
    vertical = v * 2

    world = [
            Sphere(Vec(0,0,0),1000,Vec(0.5,0.7,1.0),attenuation=0,inverse=True),
            MetalSphere(Vec(1,0.5,-1), 0.5, Vec(0.72,0.43,0.47)),
            Sphere(Vec(0,0.5,-1), 0.5, Vec(0,0,1)),
            Box(Vec(-2.5,0,-2), Vec(2,2,2), Vec(1,0,0)),
            Sphere(Vec(0,-400,-2), 400, Vec(0,1,0)),
            Sphere(Vec(10,0.5,-10), 0.5, Vec(0,0.4,0))
            ]

    with open(filename, 'w') as out:
        out.write('P3 {} {} 255\n'.format(int(h*aspect), h))
        for y in range(h)[::-1]:
            logging.debug('Rendering row: {}/{} (ETA: {})'.format(y, h, eta))
            for x in range(int(h*aspect)):
                color = Vec(0,0,0)
                for s in range(samples):
                    dx = random.random()
                    dy = random.random()
                    lens = (u*(random.random()-0.5) + v*(random.random()-0.5))
                    ray = Ray(origin + lens*(aperture/h), lower_left_corner +
                            horizontal*((x+dx)/(h*aspect)) +
                            vertical*((y+dy)/h) +
                            lens*(-aperture/(h*distance)), world)
                    color = color + ray.bounce()
                color = color * (1.0/samples)

                r = int(255.9 * math.sqrt(color.x))
                g = int(255.9 * math.sqrt(color.y))
                b = int(255.9 * math.sqrt(color.z))
                out.write('{} {} {}\n'.format(r, g, b))
            eta_ = time.localtime(time.time() + (time.time() - start_time)*y/(h-y))
            eta = time.strftime('%H:%M:%S', eta_)
    logging.info('Rendered image in {} seconds'.format(time.time()-start_time))

args = parse_args()

if args.debug:
    logging.basicConfig(level=logging.DEBUG)

run(args.filename)
