#include <stdlib.h> /*                 P A T H T R A C I N G                  */
#include <stdio.h>  /*                    My living room                      */
#include <math.h>   /*                                                        */
#include <thread>   /*                                                        */
#define R return    /* $ gcc -lm -o pt pt.cpp              Joris Hartog, 2019 */
#define O operator  /* $ ./pt > room.ppm                            curlba.sh */
typedef std::thread X;
typedef float F;typedef int I;struct V{F x,y,z;V(F v=0){x=y=z=v;}V(F a,F b,F c=0
){x=a;y=b;z=c;}V O+(V r){R V(x+r.x,y+r.y,z+r.z);}V O*(V r){R V(x*r.x,y*r.y,z*r.z
);}F O%(V r){R x*r.x+y*r.y+z*r.z;}V O!(){R*this*(1/sqrtf(*this%*this));}};F r(){
R(F)rand()/RAND_MAX;}F A(F l,F r){R l<r?l:r;}F Z(F l,F r){R l>r?l:r;}


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

float Q(V position, I &hitType) {
  V dup = position; // Used to duplicate window
  while (dup.z > 1 && dup.z < 16) dup.z -= 8;

  float distance = BoxTest(position, V(2.2, 2.6, -8.8), V(6.8, 5, -8.6));
  hitType = 5;

  float roomDist = Z(// Room
                        -BoxTest(dup, V(-1, 0, -9), V(10, 12, 21)),
                        // Window
                        -BoxTest(dup, V(9, 3, -6), V(13, 10, 0)));
  if (roomDist < distance) distance = roomDist, hitType = 2;

  // Window
  roomDist = BoxTest(dup, V(10.7, 3, -6), V(11, 10, 0));
  if (roomDist < 1) {
    roomDist = BoxTest(dup, V(10.7, 3, -6), V(10.8, 6.7, -5.4));
    if (roomDist < distance) distance = roomDist, hitType = 2;

    roomDist = BoxTest(dup, V(10.9, 6.5, -6), V(11, 10, -5.4));
    if (roomDist < distance) distance = roomDist, hitType = 2;

    roomDist = BoxTest(dup, V(10.7, 3, -0.6), V(10.8, 6.7, 0));
    if (roomDist < distance) distance = roomDist, hitType = 2;

    roomDist = BoxTest(dup, V(10.9, 6.5, -0.6), V(11, 10, 0));
    if (roomDist < distance) distance = roomDist, hitType = 2;

    roomDist = BoxTest(dup, V(10.7, 3, -6), V(10.8, 3.6, 0));
    if (roomDist < distance) distance = roomDist, hitType = 2;

    roomDist = BoxTest(dup, V(10.9, 9.4, -6), V(11, 10, 0));
    if (roomDist < distance) distance = roomDist, hitType = 2;

    roomDist = BoxTest(dup, V(10.7, 3, -3.2), V(10.8, 6.5, -2.8));
    if (roomDist < distance) distance = roomDist, hitType = 2;

    roomDist = BoxTest(dup, V(10.9, 6.5, -3.2), V(11, 10, -2.8));
    if (roomDist < distance) distance = roomDist, hitType = 2;

    roomDist = BoxTest(dup, V(10.7, 6.3, -6), V(10.8, 6.7, 0));
  }
  if (roomDist < distance) distance = roomDist, hitType = 2;

  // Locker
  roomDist = BoxTest(position, V(2, 0, -8.8), V(7, 2.5, -7.2));
  if (roomDist < 1) {
    roomDist = BoxTest(position, V(2, 0.5, -8.8), V(2.1, 2.5, -7.2));
    if (roomDist < distance) distance = roomDist, hitType = 2;

    roomDist = BoxTest(position, V(4.45, 0.5, -8.8), V(4.55, 2.5, -7.2));
    if (roomDist < distance) distance = roomDist, hitType = 2;

    roomDist = BoxTest(position, V(6.9, 0.5, -8.8), V(7, 2.5, -7.2));
    if (roomDist < distance) distance = roomDist, hitType = 2;

    roomDist = BoxTest(position, V(2, 2.3, -8.8), V(7, 2.5, -7.2));
    if (roomDist < distance) distance = roomDist, hitType = 2;

    roomDist = BoxTest(position, V(2, 0.5, -8.8), V(7, 0.6, -7.2));
    if (roomDist < distance) distance = roomDist, hitType = 2;

    roomDist = BoxTest(position, V(2, 0, -7.4), V(2.1, 1, -7.2));
    if (roomDist < distance) distance = roomDist, hitType = 2;

    roomDist = BoxTest(position, V(6.9, 0, -7.4), V(7, 1, -7.2));
    if (roomDist < distance) distance = roomDist, hitType = 2;

    roomDist = BoxTest(position, V(2, 0, -8.8), V(2.1, 1, -8.6));
    if (roomDist < distance) distance = roomDist, hitType = 2;

    roomDist = BoxTest(position, V(6.9, 0, -8.8), V(7, 1, -8.6));
  }
  if (roomDist < distance) distance = roomDist, hitType = 2;

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
  if (roomDist < distance) distance = roomDist, hitType = 6;

  roomDist = A(A(
      Z(
      BoxTest(position, V(7.8, 6.5, -8.5), V(9.8, 6.8, -6.5)),
      SphereTest(position, V(8.8, 8, -7.5), 1.5)
      ),
      CilinderTest(position, V(8.8, 0, -7.5), 6.5, 0.03)
      ),
      CilinderTest(position, V(8.8, 0, -7.5), 0.1, 0.6)
      );
  if (roomDist < distance) distance = roomDist, hitType = 4;

  roomDist = A(
      BoxTest(position, V(2.14, 0.64, -8.8), V(4.41, 2.26, -7.2)),
      BoxTest(position, V(4.59, 0.64, -8.8), V(6.86, 2.26, -7.2))
      );
  if (roomDist < distance) distance = roomDist, hitType = 2;

  roomDist = Z(
    A(
      BoxTest(position, V(2.5, 0.2, 3), V(9.7, 2.7, 7)),
      BoxTest(position, V(3.5, 1, 2.7), V(8.7, 3, 7))
      ),
    -BoxTest(position, V(3.4, 2, 2), V(8.8, 4, 6))
    );
  if (roomDist < distance) distance = roomDist, hitType = 3;

  roomDist = Z(
    CilinderTest(position, V(6.5, 1.5, .7), 0.14, 1.5),
    -CilinderTest(position, V(6.5, 1.52, .7), 0.2, 1.49)
  );
  if (roomDist < distance) distance = roomDist, hitType = 2;

  float sun = 11 - position.x;
  if (sun < distance) distance = sun, hitType = 1;

  return distance;
}

I M(V o,V v,V&p,V&n){I t,c=0;F d;for(F a=0;a<99;a+=d)if((d=Q(p=o+v*a,t))<0.01||
++c>99)R n=!V(Q(p+V(.01,0),c)-d,Q(p+V(0,.01),c)-d,Q(p+V(0,0,.01),c)-d),t;R 0;}

V T(V o,V d) {V q,n,e,a=1,w(!V(1,1,3));for(I x=3;x--;){I t=M(o,d,q,n);if(t==1){e
=e+a*V(51,81,99);break;}F i=n%w;if(t>=4){d=d+n*(n%d*-2);o=q+d*0.1;a=a*(t==6?0.6:
(t==4?0.01:0.05));if(t!=5)d=!(d+V(r(),r(),r())*0.2);}
    if (t >= 2 && t <= 3) {
      F p = 6.283185 * r();
      F c = r();
      F s = sqrtf(1 - c);
      F g = n.z < 0 ? -1 : 1;
      F u = -1 / (g + n.z);
      F v = n.x * n.y * u;
      d = V(v,
                      g + n.y * n.y * u,
                      -n.y) * (cosf(p) * s)
                  +
                  V(1 + g * n.x * n.x * u,
                      g * v,
                      -g * n.x) * (sinf(p) * s) + n * sqrtf(c);
      o = q + d * .1;
      a = a * 0.2;
    }
    if (t != 5 && i > 0 &&
          M(q + n * .1,
                      w,
                      q,
                      n) == 1)
      e = e + a * (
          t == 3 ? V(200, 600, 400) : (
            t == 4 ? V(100) : V(500, 400, 100)
          )) * i;
  }
  R e;
}

void t(V* a,V b,V c){*a=*a+T(b,c);}

I main() {
//  I w = 1280, h = 720, samplesCount = 512;
  I w = 480, h = 270, samplesCount = 1;
  V pos(1, 5, 9);
  V goal = !(V(8, 4, -8) + pos * -1);
  V left = !V(goal.z, 0, -goal.x) * (1. / w);

  // Cross-product to get the up vector
  V up(goal.y * left.z - goal.z * left.y,
      goal.z * left.x - goal.x * left.z,
      goal.x * left.y - goal.y * left.x);

  X** threads = (X**)malloc(sizeof(X*)*samplesCount);

  printf("P6 %d %d 255 ", w, h);
  for (I y = h; y--;)
    for (I x = w; x--;) {
      V col;
      for (I p = samplesCount; p--;)
        threads[p] = new X(t,&col,pos,!(goal+left*(x-w/2+r())+up*(y-h/2+r())));
      for (I p = samplesCount; p--;)
        threads[p]->join();

      // Reinhard tone mapping
      col = col * (1. / samplesCount) + 14. / 241;
      V o = col + 1;
      col = V(col.x / o.x, col.y / o.y, col.z / o.z) * 255;
      printf("%c%c%c", (I) col.x, (I) col.y, (I) col.z);
    }
}
