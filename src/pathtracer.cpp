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
#define HIT_TV 1
#define HIT_WALL 2
#define HIT_SUN 3

float QueryDatabase(Vec position, int &hitType) {
  float distance = 1e9;
  Vec dup = position; // Used to duplicate window
  while (dup.z > 0 && dup.z < 24) dup.z -= 8; // TODO: Adjust 20 to fit 3 windows

  distance = BoxTest(position, Vec(3, 2.7, -9.8), Vec(7, 5, -9.6));
  hitType = HIT_TV;

  float roomDist ;
  roomDist = -min(// Room
                  BoxTest(dup, Vec(-5, 0, -10), Vec(10, 12, 24)),
                  // Window
                  BoxTest(dup, Vec(9, 3, -8), Vec(13, 10, -2)));
  if (roomDist < distance) distance = roomDist, hitType = HIT_WALL;

  /* COMPRESSION START */

  roomDist = BoxTest(dup, Vec(10.3, 3, -8), Vec(10.4, 10, -7.4));
  if (roomDist < distance) distance = roomDist, hitType = HIT_WALL;

  roomDist = BoxTest(dup, Vec(10.3, 3, -2.6), Vec(10.4, 10, -2));
  if (roomDist < distance) distance = roomDist, hitType = HIT_WALL;

  roomDist = BoxTest(dup, Vec(10.3, 3, -8), Vec(10.4, 3.6, -2));
  if (roomDist < distance) distance = roomDist, hitType = HIT_WALL;

  roomDist = BoxTest(dup, Vec(10.3, 9.4, -8), Vec(10.4, 10, -2));
  if (roomDist < distance) distance = roomDist, hitType = HIT_WALL;

  roomDist = BoxTest(dup, Vec(10.3, 3, -5.1), Vec(10.4, 10, -4.9));
  if (roomDist < distance) distance = roomDist, hitType = HIT_WALL;

  roomDist = BoxTest(dup, Vec(10.3, 6.3, -8), Vec(10.4, 6.7, -2));
  if (roomDist < distance) distance = roomDist, hitType = HIT_WALL;

  /* COMPRESSION END */

  float sun = 10.5 - position.x;
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
  Vec lightDirection(!Vec(2,3,8)); // Directional light

  for (int bounceCount = 3; bounceCount--;) {
  //for (int bounceCount = 2; bounceCount--;) {
    int hitType = RayMarching(origin, direction, sampledPosition, normal);
    float incidence = normal % lightDirection;
    //if (hitType == HIT_NONE) break; // No hit. Should not be possible.
    if (hitType == HIT_TV) {
      direction = direction + normal * ( normal % direction * -2);
      origin = sampledPosition + direction * 0.1;
      attenuation = attenuation * 0.2; // Attenuation via distance traveled.
      if (incidence > 0 &&
          RayMarching(sampledPosition + normal * .1,
                      lightDirection,
                      sampledPosition,
                      normal) == HIT_SUN)
        color = color + attenuation * Vec(500,400,100) * incidence;
    }
    if (hitType == HIT_WALL) { // Wall hit uses color yellow?
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
                      normal) == HIT_SUN)
        color = color + attenuation * Vec(500, 400, 100) * incidence;
    }
    if (hitType == HIT_SUN) {
      color = color + attenuation * Vec(50, 80, 100); break; // Sun Color
    }
  }
  return color;
}

int main() {
//  int w = 960, h = 540, samplesCount = 64;
  int w = 480, h = 270, samplesCount = 2;
  Vec position(1, 5, 6);
  Vec goal = !(Vec(8, 4, -10) + position * -1);
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
