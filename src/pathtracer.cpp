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
    Vec operator^(Vec r) {return Vec(y*r.z-z*r.y,z*r.x-x*r.z,x*r.y-y*r.x);}
};

struct Ray {
  Vec d, o; // Direction, origin

  Ray(Vec a, Vec b) { o = a; d = !b; }
  Vec p(float t) { return o + d*t; }
};

/* Utils */
float min(float l, float r) { return l < r ? l : r; }
float randomVal() { return (float) rand() / RAND_MAX; }

// Rectangle CSG equation. Returns minimum signed distance from
// space carved by lowerLeft vertex and opposite rectangle
// vertex upperRight.
float BoxTest(Vec position, Vec lowerLeft, Vec upperRight, Vec& normal) {
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
float SphereTest(Vec p, Vec c, float r, Vec& normal) {
  Vec delta = p + c * -1;
  normal = !delta;
  float distance = sqrtf(delta%delta);
  return distance - r;
}

#define HIT_NONE   0
#define HIT_SKY    1
#define HIT_EARTH  2
#define HIT_SPHERE 3
#define HIT_BOX    4

Ray march(Ray r, int& object) {
  float d = 0;
  Vec reflection, n = 1;

  while(d < 1e9) {
    // Determine closest object
    Vec m = r.p(d);
    float closest = 1e9;

    float sky = -SphereTest(m,Vec(0,0,0),1e3,n);
    if (sky < closest) {
      object = HIT_SKY;
      closest = sky;
    }

    float earth = SphereTest(m,Vec(0,-999,-1),999,n);
    if (earth < closest) {
      object = HIT_EARTH;
      closest = earth;
    }

    float sphere = SphereTest(m,Vec(1,0.5,-1),0.5,n);
    if (sphere < closest) {
      object = HIT_SPHERE;
      closest = sphere;
      reflection = r.d + (r.d^n)*n*-2;
    }

    float box = BoxTest(m,Vec(-1.5,0,-1.5),Vec(-0.5,1,-0.5),n);
    if (box < closest) {
      object = HIT_BOX;
      closest = box;
    }

    // Check if close enough
    if (closest < 0.01) break;

    // Jump forward and repeat
    d += closest;
  }

  return Ray(r.p(d)+n*0.9, reflection);
}

Vec trace(Ray r, int bounce) {
  int object = HIT_NONE;
  Ray reflection = march(r, object);

  if (object == HIT_SKY) {
    float t = 0.5*(r.d.y + 1.0);
    return Vec(1,1,1)*(1-t) + Vec(0.5,0.7,1)*t;
  }
  if (object == HIT_EARTH) {
    return Vec(0,0.7,0);
  }
  if (object == HIT_SPHERE) {
    //if(bounce > 0) return trace(reflection,bounce-1);
    return Vec(0,0,1);
  }
  if (object == HIT_BOX) {
    return Vec(0.7,0.1,0.2);
  }

  return Vec(0,0,0);
}

void test() {
  fprintf(stderr, "Function/class tests:\n");

  Vec a(0,1,0);
  Vec b(1,0,0);
  Vec c = a^b;
  Vec d = b^a;

  fprintf(stderr, "Cross-product 1.. ");
  if ((c.x != 0) || (c.y != 0) || (c.z != -1)) {
    fprintf(stderr, "Not OK!\n");
  } else {
    fprintf(stderr, "OK\n");
  }

  fprintf(stderr, "Cross-product 2.. ");
  if ((d.x != 0) || (d.y != 0) || (d.z != 1)) {
    fprintf(stderr, "Not OK!\n");
  } else {
    fprintf(stderr, "OK\n");
  }
}

int main() {
  test();
  int w = 200;
  int h = 100;

  printf("P6 %d %d 255 ", w, h);

  Vec lower_left_corner(-2,-1,-1);
  Vec    hor(4,0,0);
  Vec    ver(0,2,0);
  Vec origin(0,1,1);

  for (int y = h-1; y >= 0; y--) {
    for (int x = 0; x < w; x++) {
      float u = float(x)/float(w);
      float v = float(y)/float(h);
      Ray r(origin, lower_left_corner + hor*u + ver*v);
      Vec col = trace(r,1);

      printf("%c%c%c",
          int(col.x*255.9),
          int(col.y*255.9),
          int(col.z*255.9)
          );
    }
  }
}
