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
#define HIT_WHITE 1
#define HIT_OVEN 2
#define HIT_WALL 3
#define HIT_SUN 4
#define HIT_BLACK 5
#define HIT_LIGHT 6

float QueryDatabase(Vec position, int &hitType) {
  float distance = 1e9;

  float closest ;
  closest = -min(// Room
                  BoxTest(position, Vec(0), Vec(10, 3, 3)),
                      // Window
                  min(BoxTest(position, Vec(5, 1, 2), Vec(8, 2, 5)),
                      // Door + window
                      BoxTest(position, Vec(1, 0.2, 2), Vec(4.9, 2, 5))));
  if (closest < distance) distance = closest, hitType = HIT_WALL;

  for (int x=2;x<9;x++)
    closest = min(closest, BoxTest(position, Vec(x+0.05, 0), Vec(x+1, 1, 1)));

  closest = min(
      -min(-closest,
        BoxTest(position, Vec(4.1, 0, 0.1), Vec(4.9, 2, 0.9))),
      min(BoxTest(position, Vec(9, 0, 1), Vec(10, 2.5, 1.7)),
        BoxTest(position, Vec(9, 0, 1.75), Vec(10, 2.5, 2.8))));

  if (closest < distance) distance = closest, hitType = HIT_BLACK;

  closest = BoxTest(position, Vec(7.1, 0.1), Vec(7.9, 0.8, 1.03));
  if (closest < distance) distance = closest, hitType = HIT_OVEN;

  closest = -min(
      -BoxTest(position, Vec(2, 1), Vec(10, 1.04, 1)),
      BoxTest(position, Vec(4.1, 0, 0.1), Vec(4.9, 2, 0.9)));
  if (closest < distance) distance = closest, hitType = HIT_WHITE;

  closest = BoxTest(position, Vec(1.99, 0), Vec(2));
  if (closest < distance) distance = closest, hitType = HIT_WALL;

  closest = 4 - position.z;
  if (closest < distance) distance = closest, hitType = HIT_SUN;

  closest = position.x - 1;
  if (closest < distance) distance = closest, hitType = HIT_LIGHT;

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

  for (int bounceCount = 3; bounceCount--;) {
  //for (int bounceCount = 2; bounceCount--;) {
    int hitType = RayMarching(origin, direction, sampledPosition, normal);
    if (hitType == HIT_NONE) break; // No hit. This is over, return color.
    if (hitType == HIT_OVEN) {
      direction = direction + normal * ( normal % direction * -2);
      origin = sampledPosition + direction * 0.1;
      attenuation = attenuation * 0.2; // Attenuation via distance traveled.
    }
    if (hitType % 2){ // Wall(3), White(1), Black(5)
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
      attenuation = attenuation * (hitType<5?0.2:0.02);
    }
    if (hitType == HIT_SUN) {
      color = color + attenuation * Vec(50, 80, 100); break;
    }
    if (hitType == HIT_LIGHT) {
      color = color + attenuation * Vec(50, 45, 10); break;
    }
  }
  return color;
}

int main() {
  int w = 200, h = 100, samplesCount = 128;
  Vec position(1.5, 1.5, 2.5);
  Vec goal = !(Vec(7,1,0) + position * -1);
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
