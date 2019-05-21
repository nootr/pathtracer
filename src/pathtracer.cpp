#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <thread>

#define R return
#define O operator
typedef float F;typedef int I;typedef std::thread T;

struct Vec {
    float x, y, z;

    Vec(float v = 0) { x = y = z = v; }

    Vec(float a, float b, float c = 0) {
      x = a;
      y = b;
      z = c;
    }

    Vec operator+(Vec r) { return Vec(x + r.x, y + r.y, z + r.z); }

    Vec operator*(Vec r) { return Vec(x * r.x, y * r.y, z * r.z); }

    float operator%(Vec r) { return x * r.x + y * r.y + z * r.z; }

    Vec operator!() {
      return *this * (1 / sqrtf(*this % *this)
      );

    }
};

float min(float l, float r) { return l < r ? l : r; }

float r() { return (float) rand() / RAND_MAX; }

float SphereTest(Vec position, Vec center, float radius) {
  Vec delta = position + center * -1;
  return sqrtf(delta % delta) - radius;
}

float BoxTest(Vec position, Vec lowerLeft, Vec upperRight) {
  lowerLeft = position + lowerLeft * -1;
  upperRight = upperRight + position * -1;
  return -min(
          min(
                  min(lowerLeft.x, upperRight.x),
                  min(lowerLeft.y, upperRight.y)),
          min(lowerLeft.z, upperRight.z));
}

float CilinderTest(Vec position, Vec bottom, float height, float width) {
  Vec delta = position + bottom * -1;
  delta.y = 0;

  Vec down = position + bottom * -1;
  Vec up = bottom + Vec(0, height, 0) + position * -1;

  return -min(
      width - sqrtf(delta % delta),
      min(down.y, up.y)
      );
}

#define HIT_NONE 0
#define HIT_SUN 1
#define HIT_WALL 2
#define HIT_COUCH 3
#define HIT_LAMP 4
#define HIT_TV 5
#define HIT_KNOB 6

float QueryDatabase(Vec position, int &hitType) {
  Vec dup = position; // Used to duplicate window
  while (dup.z > 1 && dup.z < 16) dup.z -= 8;

  float distance = BoxTest(position, Vec(2.2, 2.6, -8.8), Vec(6.8, 5, -8.6));
  hitType = HIT_TV;

  float roomDist = -min(// Room
                        BoxTest(dup, Vec(-1, 0, -9), Vec(10, 12, 21)),
                        // Window
                        BoxTest(dup, Vec(9, 3, -6), Vec(13, 10, 0)));
  if (roomDist < distance) distance = roomDist, hitType = HIT_WALL;

  // Window
  roomDist = BoxTest(dup, Vec(10.7, 3, -6), Vec(11, 10, 0));
  if (roomDist < 1) {
    roomDist = BoxTest(dup, Vec(10.7, 3, -6), Vec(10.8, 6.7, -5.4));
    if (roomDist < distance) distance = roomDist, hitType = HIT_WALL;

    roomDist = BoxTest(dup, Vec(10.9, 6.5, -6), Vec(11, 10, -5.4));
    if (roomDist < distance) distance = roomDist, hitType = HIT_WALL;

    roomDist = BoxTest(dup, Vec(10.7, 3, -0.6), Vec(10.8, 6.7, 0));
    if (roomDist < distance) distance = roomDist, hitType = HIT_WALL;

    roomDist = BoxTest(dup, Vec(10.9, 6.5, -0.6), Vec(11, 10, 0));
    if (roomDist < distance) distance = roomDist, hitType = HIT_WALL;

    roomDist = BoxTest(dup, Vec(10.7, 3, -6), Vec(10.8, 3.6, 0));
    if (roomDist < distance) distance = roomDist, hitType = HIT_WALL;

    roomDist = BoxTest(dup, Vec(10.9, 9.4, -6), Vec(11, 10, 0));
    if (roomDist < distance) distance = roomDist, hitType = HIT_WALL;

    roomDist = BoxTest(dup, Vec(10.7, 3, -3.2), Vec(10.8, 6.5, -2.8));
    if (roomDist < distance) distance = roomDist, hitType = HIT_WALL;

    roomDist = BoxTest(dup, Vec(10.9, 6.5, -3.2), Vec(11, 10, -2.8));
    if (roomDist < distance) distance = roomDist, hitType = HIT_WALL;

    roomDist = BoxTest(dup, Vec(10.7, 6.3, -6), Vec(10.8, 6.7, 0));
  }
  if (roomDist < distance) distance = roomDist, hitType = HIT_WALL;

  // Locker
  roomDist = BoxTest(position, Vec(2, 0, -8.8), Vec(7, 2.5, -7.2));
  if (roomDist < 1) {
    roomDist = BoxTest(position, Vec(2, 0.5, -8.8), Vec(2.1, 2.5, -7.2));
    if (roomDist < distance) distance = roomDist, hitType = HIT_WALL;

    roomDist = BoxTest(position, Vec(4.45, 0.5, -8.8), Vec(4.55, 2.5, -7.2));
    if (roomDist < distance) distance = roomDist, hitType = HIT_WALL;

    roomDist = BoxTest(position, Vec(6.9, 0.5, -8.8), Vec(7, 2.5, -7.2));
    if (roomDist < distance) distance = roomDist, hitType = HIT_WALL;

    roomDist = BoxTest(position, Vec(2, 2.3, -8.8), Vec(7, 2.5, -7.2));
    if (roomDist < distance) distance = roomDist, hitType = HIT_WALL;

    roomDist = BoxTest(position, Vec(2, 0.5, -8.8), Vec(7, 0.6, -7.2));
    if (roomDist < distance) distance = roomDist, hitType = HIT_WALL;

    roomDist = BoxTest(position, Vec(2, 0, -7.4), Vec(2.1, 1, -7.2));
    if (roomDist < distance) distance = roomDist, hitType = HIT_WALL;

    roomDist = BoxTest(position, Vec(6.9, 0, -7.4), Vec(7, 1, -7.2));
    if (roomDist < distance) distance = roomDist, hitType = HIT_WALL;

    roomDist = BoxTest(position, Vec(2, 0, -8.8), Vec(2.1, 1, -8.6));
    if (roomDist < distance) distance = roomDist, hitType = HIT_WALL;

    roomDist = BoxTest(position, Vec(6.9, 0, -8.8), Vec(7, 1, -8.6));
  }
  if (roomDist < distance) distance = roomDist, hitType = HIT_WALL;

  roomDist = min(
      -min(
        -BoxTest(position, Vec(4.1, 1.3, -7.2), Vec(4.3, 1.5, -7.17)),
        -SphereTest(position, Vec(4.2, 1.4, -7.2), 0.05)
        ),
      -min(
        -BoxTest(position, Vec(4.7, 1.3, -7.2), Vec(4.9, 1.5, -7.17)),
        -SphereTest(position, Vec(4.8, 1.4, -7.2), 0.05)
        )
      );
  if (roomDist < distance) distance = roomDist, hitType = HIT_KNOB;

  roomDist = min(min(
      -min(
      -BoxTest(position, Vec(7.5, 6.5, -8.5), Vec(9.5, 6.8, -6.5)),
      -SphereTest(position, Vec(8.5, 8, -7.5), 1.5)
      ),
      CilinderTest(position, Vec(8.5, 0, -7.5), 6.5, 0.03)
      ),
      CilinderTest(position, Vec(8.5, 0, -7.5), 0.1, 0.6)
      );
  if (roomDist < distance) distance = roomDist, hitType = HIT_LAMP;

  roomDist = min(
      BoxTest(position, Vec(2.14, 0.64, -8.8), Vec(4.41, 2.26, -7.2)),
      BoxTest(position, Vec(4.59, 0.64, -8.8), Vec(6.86, 2.26, -7.2))
      );
  if (roomDist < distance) distance = roomDist, hitType = HIT_WALL;

  roomDist = -min(
    -min(
      BoxTest(position, Vec(2.5, 0.2, 3), Vec(9.7, 2.7, 7)),
      BoxTest(position, Vec(3.5, 1, 2.7), Vec(8.7, 3, 7))
      ),
    BoxTest(position, Vec(2.9, 2, 2), Vec(8.8, 4, 6))
    );
  if (roomDist < distance) distance = roomDist, hitType = HIT_COUCH;

  float sun = 11 - position.x;
  if (sun < distance) distance = sun, hitType = HIT_SUN;

  return distance;
}

int RayMarching(Vec origin, Vec direction, Vec &hitPos, Vec &hitNorm) {
  int hitType = HIT_NONE;
  int noHitCount = 0;
  float d; // distance from closest object in world.

  // Signed distance marching
  for (float total_d=0; total_d < 100; total_d += d)
    if ((d = QueryDatabase(hitPos = origin + direction * total_d, hitType)) < .01
            || ++noHitCount > 99)
      return hitNorm =
         !Vec(QueryDatabase(hitPos + Vec(.01, 0), noHitCount) - d,
              QueryDatabase(hitPos + Vec(0, .01), noHitCount) - d,
              QueryDatabase(hitPos + Vec(0, 0, .01), noHitCount) - d)
         , hitType; // Weird return statement where a variable is also updated.
  return 0;
}

Vec Trace(Vec origin, Vec direction) {
  Vec sampledPosition, normal, color, attenuation = 1;
  Vec lightDirection(!Vec(1,1,3)); // Directional light

  for (int bounceCount = 3; bounceCount--;) {
    int hitType = RayMarching(origin, direction, sampledPosition, normal);
    float incidence = normal % lightDirection;
    if (hitType >= HIT_TV) { // TV or knob
      direction = direction + normal * ( normal % direction * -2);
      origin = sampledPosition + direction * 0.1;
      if (hitType == HIT_TV) {
        attenuation = attenuation * 0.05; // Attenuation via distance traveled.
      } else {
        attenuation = attenuation * 0.6; // Attenuation via distance traveled.
        direction = !(direction + Vec(r(),r(),r())*0.2);
      }
    }
    if (hitType >= HIT_WALL && hitType <= HIT_LAMP) {
      float p = 6.283185 * r();
      float c = r();
      float s = sqrtf(1 - c);
      float g = normal.z < 0 ? -1 : 1;
      float u = -1 / (g + normal.z);
      float v = normal.x * normal.y * u;
      direction = Vec(v,
                      g + normal.y * normal.y * u,
                      -normal.y) * (cosf(p) * s)
                  +
                  Vec(1 + g * normal.x * normal.x * u,
                      g * v,
                      -g * normal.x) * (sinf(p) * s) + normal * sqrtf(c);
      origin = sampledPosition + direction * .1;
      attenuation = attenuation * (hitType == HIT_LAMP?0.01:0.2);
      if (incidence > 0 &&
          RayMarching(sampledPosition + normal * .1,
                      lightDirection,
                      sampledPosition,
                      normal) == HIT_SUN)
          color = color + attenuation * (
              hitType == HIT_COUCH?Vec(200, 600, 400):Vec(500, 400, 100)
              ) * incidence;
    }
    if (hitType == HIT_SUN) {
      color = color + attenuation * Vec(50, 80, 100); break;
    }
  }
  return color;
}

void t(Vec* a,Vec b,Vec c){*a=*a+Trace(b,c);}

int main() {
  int w = 960, h = 540, samplesCount = 128;
//  int w = 480, h = 270, samplesCount = 16;
  Vec pos(1, 5, 9);
  Vec goal = !(Vec(8, 4, -8) + pos * -1);
  Vec left = !Vec(goal.z, 0, -goal.x) * (1. / w);

  // Cross-product to get the up vector
  Vec up(goal.y * left.z - goal.z * left.y,
      goal.z * left.x - goal.x * left.z,
      goal.x * left.y - goal.y * left.x);

  // Create array of threads
  T** threads = (T**)malloc(sizeof(T*)*samplesCount);

  printf("P6 %d %d 255 ", w, h);
  for (int y = h; y--;)
    for (int x = w; x--;) {
      Vec col;
      for (int p = samplesCount; p--;)
        threads[p] = new T(t,&col,pos,!(goal+left*(x-w/2+r())+up*(y-h/2+r())));
      for (int p = samplesCount; p--;)
        threads[p]->join();

      // Reinhard tone mapping
      col = col * (1. / samplesCount) + 14. / 241;
      Vec o = col + 1;
      col = Vec(col.x / o.x, col.y / o.y, col.z / o.z) * 255;
      printf("%c%c%c", (int) col.x, (int) col.y, (int) col.z);
    }
}
