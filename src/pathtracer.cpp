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

/* Vector class */
struct Vec {
    float x, y, z;

    Vec(float v = 0) { x = y = z = v; }
    Vec(float a, float b, float c = 0) { x = a; y = b; z = c;}

    Vec operator+(Vec r) { return Vec(x + r.x, y + r.y, z + r.z); }
    Vec operator*(Vec r) { return Vec(x * r.x, y * r.y, z * r.z); }
    // dot product
    float operator%(Vec r) { return x * r.x + y * r.y + z * r.z; }
    // inverse square root
    Vec operator!() {return *this * (1 / sqrtf(*this % *this) );}
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
  return r - distance;
}

#define HIT_NONE 0
#define HIT_TABLE 1
#define HIT_WALL 2
#define HIT_SUN 3

// Sample the world using Signed Distance Fields.
float QueryDatabase(Vec position, int &hitType) {
  float distance = 1e9;

  float roomDist ;
  roomDist = min(// min(A,B) = Union with Constructive solid geometry
               //-min carves an empty space
                -min(// Lower room
                     BoxTest(position, Vec(-30, -.5, -30), Vec(30, 18, 30)),
                     // Upper room
                     BoxTest(position, Vec(-25, 17, -25), Vec(25, 20, 25))
                ),
                BoxTest( // Ceiling "planks" spaced 8 units apart.
                  Vec(fmodf(fabsf(position.x), 8),
                      position.y,
                      position.z),
                  Vec(1.5, 18.5, -25),
                  Vec(6.5, 20, 25)
                )
  );
  if (roomDist < distance) distance = roomDist, hitType = HIT_WALL;

  float tableDist ;
  tableDist = BoxTest(position, Vec(-5, 5, -5), Vec(5, 6, 5));
  if (tableDist < distance) distance = tableDist, hitType = HIT_TABLE;

  float sun = 19.9 - position.y; // Everything behind 29.9 is light source.
  if (sun < distance) distance = sun, hitType = HIT_SUN;

  return distance;
}

// Perform signed sphere marching
// Returns hitType 0, 1, 2, or 3 and update hit position/normal
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
  Vec lightDirection(!Vec(.6, .6, 1)); // Directional light

  for (int bounceCount = 2; bounceCount--;) {
    int hitType = RayMarching(origin, direction, sampledPosition, normal);
    if (hitType == HIT_NONE) break; // No hit. This is over, return color.
    if (hitType == HIT_TABLE) { // Specular bounce on a letter. No color acc.
      direction = direction + normal * ( normal % direction * -2);
      origin = sampledPosition + direction * 0.1;
      attenuation = attenuation * 0.2; // Attenuation via distance traveled.
    }
    if (hitType == HIT_WALL) { // Wall hit uses color yellow?
      float incidence = normal % lightDirection;
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
    if (hitType == HIT_SUN) { //
      color = color + attenuation * Vec(50, 80, 100); break; // Sun Color
    }
  }
  return color;
}

int main() {
  int w = 960, h = 540, samplesCount = 2;
  Vec position(20, 5, 25);
  Vec goal = !(Vec(1, 4, 0) + position * -1);
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
        color = color + Trace(position, !(goal + left * (x - w / 2 + randomVal())+
        up * (y - h / 2 + randomVal())));

      // Reinhard tone mapping
      color = color * (1. / samplesCount) + 14. / 241;
      Vec o = color + 1;
      color = Vec(color.x / o.x, color.y / o.y, color.z / o.z) * 255;
      printf("%c%c%c", (int) color.x, (int) color.y, (int) color.z);
    }
}

