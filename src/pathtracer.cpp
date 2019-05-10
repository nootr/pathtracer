#include <stdlib.h>
#include <stdio.h>
#include <math.h>

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

float randomVal() { return (float) rand() / RAND_MAX; }

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

#define HIT_NONE 0
#define HIT_SUN 1
#define HIT_WALL 2
#define HIT_TV 3
#define HIT_KNOB 4

float QueryDatabase(Vec position, int &hitType) {
  Vec dup = position; // Used to duplicate window
  while (dup.z > 1 && dup.z < 16.9) dup.z -= 8;

  float distance = BoxTest(position, Vec(2.2, 2.6, -8.8), Vec(6.8, 5, -8.6));
  hitType = HIT_TV;

  float roomDist = -min(// Room
                        BoxTest(dup, Vec(-1, 0, -9), Vec(10, 12, 17)),
                        // Window
                        BoxTest(dup, Vec(9, 3, -6), Vec(13, 10, 0)));
  if (roomDist < distance) distance = roomDist, hitType = HIT_WALL;

  /* COMPRESSION START */

  // Window
  if (BoxTest(dup, Vec(10.5, 3, -6), Vec(11, 10, 0)) < 1) {
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
    if (roomDist < distance) distance = roomDist, hitType = HIT_WALL;
  }

  // Locker
  if (BoxTest(position, Vec(2, 0, -8.8), Vec(7, 2.5, -6.7)) < 1) {
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
    if (roomDist < distance) distance = roomDist, hitType = HIT_WALL;
  }

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

  /* COMPRESSION END */

  roomDist = min(
      BoxTest(position, Vec(2.14, 0.64, -8.8), Vec(4.41, 2.26, -7.2)),
      BoxTest(position, Vec(4.59, 0.64, -8.8), Vec(6.86, 2.26, -7.2))
      );
  if (roomDist < distance) distance = roomDist, hitType = HIT_WALL;

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
        attenuation = attenuation * 0.2; // Attenuation via distance traveled.
      } else {
        attenuation = attenuation * 0.6; // Attenuation via distance traveled.
        direction = !(direction + Vec(randomVal(),randomVal(),randomVal())*0.2);
      }
    }
    if (hitType == HIT_WALL) {
      float p = 6.283185 * randomVal();
      float c = randomVal();
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
      attenuation = attenuation * 0.2;
      if (incidence > 0 &&
          RayMarching(sampledPosition + normal * .1,
                      lightDirection,
                      sampledPosition,
                      normal) == HIT_SUN) {
          color = color + attenuation * Vec(500, 400, 100) * incidence;
      }
    }
    if (hitType == HIT_SUN) {
      color = color + attenuation * Vec(50, 80, 100); break; // Sun Color
    }
  }
  return color;
}

int main() {
  int w = 960, h = 540, samplesCount = 128;
//  int w = 480, h = 270, samplesCount = 8;
  Vec position(1, 5, 9);
  Vec goal = !(Vec(8, 4, -8) + position * -1);
  Vec left = !Vec(goal.z, 0, -goal.x) * (1. / w);

  // Cross-product to get the up vector
  Vec up(goal.y * left.z - goal.z * left.y,
      goal.z * left.x - goal.x * left.z,
      goal.x * left.y - goal.y * left.x);

  printf("P6 %d %d 255 ", w, h);
  for (int y = h; y--;)
    for (int x = w; x--;) {
      Vec color;
      for (int p = samplesCount; p--;)
        color = color + Trace(position, !(goal + left * (x - w / 2 + randomVal()) + up * (y - h / 2 + randomVal())));

      // Reinhard tone mapping
      color = color * (1. / samplesCount) + 14. / 241;
      Vec o = color + 1;
      color = Vec(color.x / o.x, color.y / o.y, color.z / o.z) * 255;
      printf("%c%c%c", (int) color.x, (int) color.y, (int) color.z);
    }
}
