#include <stdlib.h> /*                 P A T H T R A C I N G                  */
#include <stdio.h>  /*                    My living room                      */
#include <math.h>   /*                                                        */
#include <thread>   /* $ gcc -o pt ./pt.cpp                                   */
#define R return    /* $ chmod +x pt                       Joris Hartog, 2019 */
#define O operator  /* $ ./pt > room.ppm                            curlba.sh */
typedef float F;typedef int I;typedef std::thread T;struct V{F x,y,z;V(F v=0){x=
y=z=v;}V(F a,F b,F c=0){x=a;y=b;z=c;}V O+(V r){R V(x+r.x,y+r.y,z+r.z);}V O*(V r)
{R V(x*r.x,y*r.y,z*r.z);}F O%(V r){R x*r.x+y*r.y+z*r.z;}V O!(){R*this*(1/sqrtf(*
this%*this));}};F r(){R(F)rand()/RAND_MAX;}F A(F l,F r){R l<r?l:r;}F Z(F l,F r){
R l>r?l:r;}

float SphereTest(V position, V center, float radius) {
  V delta = position + center * -1;
  return sqrtf(delta % delta) - radius;
}

float BoxTest(V position, V lowerLeft, V upperRight) {
  lowerLeft = position + lowerLeft * -1;
  upperRight = upperRight + position * -1;
  return -A(
          A(
                  A(lowerLeft.x, upperRight.x),
                  A(lowerLeft.y, upperRight.y)),
          A(lowerLeft.z, upperRight.z));
}

float CilinderTest(V position, V bottom, float height, float width) {
  V delta = position + bottom * -1;
  delta.y = 0;

  V down = position + bottom * -1;
  V up = bottom + V(0, height, 0) + position * -1;

  return -A(
      width - sqrtf(delta % delta),
      A(down.y, up.y)
      );
}

#define HIT_NONE 0
#define HIT_SUN 1
#define HIT_WALL 2
#define HIT_COUCH 3
#define HIT_LAMP 4
#define HIT_TV 5
#define HIT_KNOB 6

float QueryDatabase(V position, int &hitType) {
  V dup = position; // Used to duplicate window
  while (dup.z > 1 && dup.z < 16) dup.z -= 8;

  float distance = BoxTest(position, V(2.2, 2.6, -8.8), V(6.8, 5, -8.6));
  hitType = HIT_TV;

  float roomDist = Z(// Room
                        -BoxTest(dup, V(-1, 0, -9), V(10, 12, 21)),
                        // Window
                        -BoxTest(dup, V(9, 3, -6), V(13, 10, 0)));
  if (roomDist < distance) distance = roomDist, hitType = HIT_WALL;

  // Window
  roomDist = BoxTest(dup, V(10.7, 3, -6), V(11, 10, 0));
  if (roomDist < 1) {
    roomDist = BoxTest(dup, V(10.7, 3, -6), V(10.8, 6.7, -5.4));
    if (roomDist < distance) distance = roomDist, hitType = HIT_WALL;

    roomDist = BoxTest(dup, V(10.9, 6.5, -6), V(11, 10, -5.4));
    if (roomDist < distance) distance = roomDist, hitType = HIT_WALL;

    roomDist = BoxTest(dup, V(10.7, 3, -0.6), V(10.8, 6.7, 0));
    if (roomDist < distance) distance = roomDist, hitType = HIT_WALL;

    roomDist = BoxTest(dup, V(10.9, 6.5, -0.6), V(11, 10, 0));
    if (roomDist < distance) distance = roomDist, hitType = HIT_WALL;

    roomDist = BoxTest(dup, V(10.7, 3, -6), V(10.8, 3.6, 0));
    if (roomDist < distance) distance = roomDist, hitType = HIT_WALL;

    roomDist = BoxTest(dup, V(10.9, 9.4, -6), V(11, 10, 0));
    if (roomDist < distance) distance = roomDist, hitType = HIT_WALL;

    roomDist = BoxTest(dup, V(10.7, 3, -3.2), V(10.8, 6.5, -2.8));
    if (roomDist < distance) distance = roomDist, hitType = HIT_WALL;

    roomDist = BoxTest(dup, V(10.9, 6.5, -3.2), V(11, 10, -2.8));
    if (roomDist < distance) distance = roomDist, hitType = HIT_WALL;

    roomDist = BoxTest(dup, V(10.7, 6.3, -6), V(10.8, 6.7, 0));
  }
  if (roomDist < distance) distance = roomDist, hitType = HIT_WALL;

  // Locker
  roomDist = BoxTest(position, V(2, 0, -8.8), V(7, 2.5, -7.2));
  if (roomDist < 1) {
    roomDist = BoxTest(position, V(2, 0.5, -8.8), V(2.1, 2.5, -7.2));
    if (roomDist < distance) distance = roomDist, hitType = HIT_WALL;

    roomDist = BoxTest(position, V(4.45, 0.5, -8.8), V(4.55, 2.5, -7.2));
    if (roomDist < distance) distance = roomDist, hitType = HIT_WALL;

    roomDist = BoxTest(position, V(6.9, 0.5, -8.8), V(7, 2.5, -7.2));
    if (roomDist < distance) distance = roomDist, hitType = HIT_WALL;

    roomDist = BoxTest(position, V(2, 2.3, -8.8), V(7, 2.5, -7.2));
    if (roomDist < distance) distance = roomDist, hitType = HIT_WALL;

    roomDist = BoxTest(position, V(2, 0.5, -8.8), V(7, 0.6, -7.2));
    if (roomDist < distance) distance = roomDist, hitType = HIT_WALL;

    roomDist = BoxTest(position, V(2, 0, -7.4), V(2.1, 1, -7.2));
    if (roomDist < distance) distance = roomDist, hitType = HIT_WALL;

    roomDist = BoxTest(position, V(6.9, 0, -7.4), V(7, 1, -7.2));
    if (roomDist < distance) distance = roomDist, hitType = HIT_WALL;

    roomDist = BoxTest(position, V(2, 0, -8.8), V(2.1, 1, -8.6));
    if (roomDist < distance) distance = roomDist, hitType = HIT_WALL;

    roomDist = BoxTest(position, V(6.9, 0, -8.8), V(7, 1, -8.6));
  }
  if (roomDist < distance) distance = roomDist, hitType = HIT_WALL;

  roomDist = A(
      Z(
        BoxTest(position, V(4.1, 1.3, -7.2), V(4.3, 1.5, -7.17)),
        SphereTest(position, V(4.2, 1.4, -7.2), 0.05)
        ),
      Z(
        BoxTest(position, V(4.7, 1.3, -7.2), V(4.9, 1.5, -7.17)),
        SphereTest(position, V(4.8, 1.4, -7.2), 0.05)
        )
      );
  if (roomDist < distance) distance = roomDist, hitType = HIT_KNOB;

  roomDist = A(A(
      Z(
      BoxTest(position, V(7.8, 6.5, -8.5), V(9.8, 6.8, -6.5)),
      SphereTest(position, V(8.8, 8, -7.5), 1.5)
      ),
      CilinderTest(position, V(8.8, 0, -7.5), 6.5, 0.03)
      ),
      CilinderTest(position, V(8.8, 0, -7.5), 0.1, 0.6)
      );
  if (roomDist < distance) distance = roomDist, hitType = HIT_LAMP;

  roomDist = A(
      BoxTest(position, V(2.14, 0.64, -8.8), V(4.41, 2.26, -7.2)),
      BoxTest(position, V(4.59, 0.64, -8.8), V(6.86, 2.26, -7.2))
      );
  if (roomDist < distance) distance = roomDist, hitType = HIT_WALL;

  roomDist = Z(
    A(
      BoxTest(position, V(2.5, 0.2, 3), V(9.7, 2.7, 7)),
      BoxTest(position, V(3.5, 1, 2.7), V(8.7, 3, 7))
      ),
    -BoxTest(position, V(3.4, 2, 2), V(8.8, 4, 6))
    );
  if (roomDist < distance) distance = roomDist, hitType = HIT_COUCH;

  roomDist = Z(
    CilinderTest(position, V(6.5, 1.5, 1.5), 0.14, 1.5),
    -CilinderTest(position, V(6.5, 1.52, 1.5), 0.2, 1.49)
  );
  if (roomDist < distance) distance = roomDist, hitType = HIT_WALL;

  float sun = 11 - position.x;
  if (sun < distance) distance = sun, hitType = HIT_SUN;

  return distance;
}

int RayMarching(V origin, V direction, V &hitPos, V &hitNorm) {
  int hitType = HIT_NONE;
  int noHitCount = 0;
  float d; // distance from closest object in world.

  // Signed distance marching
  for (float total_d=0; total_d < 99; total_d += d)
    if ((d = QueryDatabase(hitPos = origin + direction * total_d, hitType)) < 0.01
            || ++noHitCount > 99)
      return hitNorm =
         !V(QueryDatabase(hitPos + V(.01, 0), noHitCount) - d,
              QueryDatabase(hitPos + V(0, .01), noHitCount) - d,
              QueryDatabase(hitPos + V(0, 0, .01), noHitCount) - d)
         , hitType; // Weird return statement where a variable is also updated.
  return 0;
}

V Trace(V origin, V direction) {
  V sampledPosition, normal, color, attenuation = 1;
  V lightDirection(!V(1,1,3)); // Directional light

  for (int bounceCount = 3; bounceCount--;) {
    int hitType = RayMarching(origin, direction, sampledPosition, normal);
    if (hitType == HIT_SUN) {
      color = color + attenuation * V(50, 80, 100); break;
    }
    float incidence = normal % lightDirection;
    if (hitType >= HIT_LAMP) { // Lamp, TV or knob
      direction = direction + normal * ( normal % direction * -2);
      origin = sampledPosition + direction * 0.1;
      attenuation = attenuation * (
          hitType == HIT_KNOB ? 0.6 : (
            hitType == HIT_LAMP?0.01 : 0.05));
      if (hitType != HIT_TV)
        direction = !(direction + V(r(),r(),r())*0.2);
    }
    if (hitType >= HIT_WALL && hitType <= HIT_COUCH) {
      float p = 6.283185 * r();
      float c = r();
      float s = sqrtf(1 - c);
      float g = normal.z < 0 ? -1 : 1;
      float u = -1 / (g + normal.z);
      float v = normal.x * normal.y * u;
      direction = V(v,
                      g + normal.y * normal.y * u,
                      -normal.y) * (cosf(p) * s)
                  +
                  V(1 + g * normal.x * normal.x * u,
                      g * v,
                      -g * normal.x) * (sinf(p) * s) + normal * sqrtf(c);
      origin = sampledPosition + direction * .1;
      attenuation = attenuation * 0.2;
    }
    if (hitType != HIT_TV && incidence > 0 &&
          RayMarching(sampledPosition + normal * .1,
                      lightDirection,
                      sampledPosition,
                      normal) == HIT_SUN)
      color = color + attenuation * (
          hitType == HIT_COUCH ? V(200, 600, 400) : (
            hitType == HIT_LAMP ? V(100) : V(500, 400, 100)
          )) * incidence;
  }
  return color;
}

void t(V* a,V b,V c){*a=*a+Trace(b,c);}

int main() {
//  int w = 960, h = 540, samplesCount = 128;
  int w = 480, h = 270, samplesCount = 8;
  V pos(1, 5, 9);
  V goal = !(V(8, 4, -8) + pos * -1);
  V left = !V(goal.z, 0, -goal.x) * (1. / w);

  // Cross-product to get the up vector
  V up(goal.y * left.z - goal.z * left.y,
      goal.z * left.x - goal.x * left.z,
      goal.x * left.y - goal.y * left.x);

  // Create array of threads
  T** threads = (T**)malloc(sizeof(T*)*samplesCount);

  printf("P6 %d %d 255 ", w, h);
  for (int y = h; y--;)
    for (int x = w; x--;) {
      V col;
      for (int p = samplesCount; p--;)
        threads[p] = new T(t,&col,pos,!(goal+left*(x-w/2+r())+up*(y-h/2+r())));
      for (int p = samplesCount; p--;)
        threads[p]->join();

      // Reinhard tone mapping
      col = col * (1. / samplesCount) + 14. / 241;
      V o = col + 1;
      col = V(col.x / o.x, col.y / o.y, col.z / o.z) * 255;
      printf("%c%c%c", (int) col.x, (int) col.y, (int) col.z);
    }
}
