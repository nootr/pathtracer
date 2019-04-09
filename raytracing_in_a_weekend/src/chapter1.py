#!/usr/bin/env python3

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
        denom = (1 / sqrtf(self % self))
        return self * Vec(denom, denom, denom)

    def __repr__(self):
        return '({},{},{})'.format(self.x, self.y, self.z)

def main():
    w = 200
    h = 100
    print('P3 {} {} 255\n'.format(w, h))

    for y in range(h)[::-1]:
        for x in range(w):
            r = int(255.9 * x / w)
            g = int(255.9 * y / h)
            b = int(255.9 * 0.2)
            print('{} {} {}'.format(r, g, b))

main()
