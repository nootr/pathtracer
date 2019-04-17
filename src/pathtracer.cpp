/* Postcard pathtracer by Joris Hartog
 * Based on http://fabiensanglard.net/postcard_pathtracer
 *
 * Renders my office desk with or without my favorite plant Charlie besides it.
 * The following shapes need to be created:
 * * White room minus windows
 * * Blue border(s)
 * * White desk(s)
 * * Black computer screen(s)
 * * Grey cabinet
 * * Brownish grey carpet
 *
 * This means we need CSG equations for:
 * * A sphere (desk cutout)
 * * A box (the rest)
 *
 * ```
 * >>> def testBox(a, c1, c2):
 * ...   c1x = a[0] - c1[0]
 * ...   c1y = a[1] - c1[1]
 * ...   c2x = c2[0] - a[0]
 * ...   c2y = c2[1] - a[1]
 * ...   return min(c1x, c1y, c2x, c2y)
 * ...
 * >>> testBox((2,3), (1,1), (3,2))
 * -1
 * >>> testBox((2,1.5), (1,1), (3,2))
 * 0.5
 * >>> testBox((0,0), (1,1), (3,2))
 * -1
 * >>> def testCutout(a, c1, c2, c3, c4):
 * ...   return min(testBox(a, c1, c2), -testBox(a, c3, c4))
 * ...
 * >>> testCutout((0,0), (0,1), (4,4), (2,0), (5,2.5))
 * -1
 * >>> testCutout((2,5), (0,1), (4,4), (2,0), (5,2.5))
 * -1
 * >>> testCutout((2,3), (0,1), (4,4), (2,0), (5,2.5))
 * 0.5
 * >>> testCutout((3,2), (0,1), (4,4), (2,0), (5,2.5))
 * -0.5
 * >>> testCutout((3,0.5), (0,1), (4,4), (2,0), (5,2.5))
 * -0.5
 * ```
 *
 *  TL;DR
 *  You can carf piece Y out of a shape X by using `min(testX, -testY)`.
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

// These will be used later
#define R return
#define O operator
typedef float F;typedef int I;

/* Classes */
struct Vec {
    float x, y, z;

    Vec(float v = 0) { x = y = z = v; }
    Vec(float a, float b, float c = 0) { x = a; y = b; z = c;}

    Vec operator+(Vec r) { return Vec(x + r.x, y + r.y, z + r.z); }
    Vec operator*(Vec r) { return Vec(x * r.x, y * r.y, z * r.z); }
    float operator%(Vec r) { return x * r.x + y * r.y + z * r.z; }
    Vec operator!() {return *this * (1 / sqrtf(*this % *this) );}
};

struct Ray {
  Vec A, B;

  Ray(Vec& o, Vec d) { A = o; B = d; }
  Vec direction() { return B; }    // Deze twee regels
  Vec origin() { return A; }       // zijn hopelijk overbodig.
  Vec point(float t) { return A + B*t; }
};

/* Utils */
float min(float l, float r) { return l < r ? l : r; }
float randomVal() { return (float) rand() / RAND_MAX; }

// Rectangle CSG equation. Returns minimum signed distance from
// space carved by lowerLeft vertex and opposite rectangle
// vertex upperRight.
float BoxTest(Vec position, Vec lowerLeft, Vec upperRight) {
  lowerLeft = position + lowerLeft * -1;
  upperRight = upperRight + position * -1;
  return -min(
    min(
      min(lowerLeft.x, upperRight.x),
      min(lowerLeft.y, upperRight.y)
    ),
    min(lowerLeft.z, upperRight.z));
}

// Signed distance point(p) to sphere(c,r)
float SphereTest(Vec p, Vec c, float r) {
  Vec delta = c + p * -1;
  float distance = sqrtf(delta%delta);
  return distance - r;
}

#define HIT_NONE  0
#define HIT_SKY   1
#define HIT_EARTH 2

float march(Ray r, int& object, Vec& reflection) {
  float d = 0;

  while(d < 1e9) {
    // Determine closest object
    Vec n = r.point(d);
    float closest = 1e9;

    float sky = -SphereTest(n,Vec(0,0,0),1000);
    if (sky < closest) {
      object = HIT_SKY;
      closest = sky;
    }

    float earth = SphereTest(n,Vec(0,-999,-1),999);
    if (earth < closest) {
      object = HIT_EARTH;
      closest = earth;
    }

    // Check if close enough
    if (closest < 0.01) break;

    // Jump forward and repeat
    d += closest;
  }

  return d - 0.1;
}

Vec trace(Ray r, int bounce) {
  int object = HIT_NONE;
  Vec reflection = Vec(0,0,0);
  float distance = march(r, object, reflection);

  if (object == HIT_SKY) {
    float t = 0.5*(r.direction().y + 1.0);
    return Vec(1,1,1)*(1-t) + Vec(0.5,0.7,1)*t;
  }
  if (object == HIT_EARTH) {
    return Vec(0,0.7,0);
  }
  return Vec(0,0,0);
}

int main() {
  int w = 200;
  int h = 100;

  printf("P6 %d %d 255 ", w, h);

  Vec lower_left_corner(-2,-1,-1);
  Vec    hor(4,0,0);
  Vec    ver(0,2,0);
  Vec origin(0,1,0);

  for (int y = h-1; y >= 0; y--) {
    for (int x = 0; x < w; x++) {
      float u = float(x)/float(w);
      float v = float(y)/float(h);
      Ray r(origin, lower_left_corner + hor*u + ver*v);
      Vec col = trace(r,3);

      printf("%c%c%c",
          int(col.x*255.9),
          int(col.y*255.9),
          int(col.z*255.9)
          );
    }
  }
}
