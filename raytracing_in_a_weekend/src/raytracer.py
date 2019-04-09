#!/usr/bin/env python3

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

    def __invert__(self): # This will be `operator!` in C, not ~
        denom = (1 / math.sqrt(self % self))
        return self * Vec(denom, denom, denom)

    def __repr__(self):
        return '({},{},{})'.format(self.x, self.y, self.z)

class Ray(object):
    """Ray

    Represents a line in 3D space which follows the function p(t) = A + t * B,
    where the ray is a line with its origin at A and normalized direction B
    containing point p at distance t from the origin.
    """
    def __init__(self, origin, direction):
        self.origin = origin
        self.direction = ~direction

    def point(self, distance):
        return self.origin + distance * self.direction

def generate_sky_color(ray):
    """generate_sky_color

    Generates a lerp (linear blend) in the form of (1-t)*start + t*end.
    """
    t = 0.5 * (ray.direction.y + 1.0)
    return Vec(1,1,1) * (1 - t) + Vec(0.5, 0.7, 1.0) * t

def main():
    w = 200
    h = 100
    print('P3 {} {} 255\n'.format(w, h))

    lower_left_corner = Vec(-2, -1, -1)
    horizontal = Vec(4, 0, 0)
    vertical = Vec(0, 2, 0)
    origin = Vec(0, 0, 0)

    for y in range(h)[::-1]:
        for x in range(w):
            ray = Ray(origin, lower_left_corner + horizontal*(x/w) +
                    vertical*(y/h))
            color = generate_sky_color(ray)

            r = int(255.9 * color.x)
            g = int(255.9 * color.y)
            b = int(255.9 * color.z)
            print('{} {} {}'.format(r, g, b))

main()
