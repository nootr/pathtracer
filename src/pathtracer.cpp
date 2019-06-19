// https://mattermost.hostnet.nl/hostnet/pl/pz8uhrwieifpdgtj6enafen4bc
#include <stdlib.h> /*                                                        */
#include <stdio.h>  /*                 P A T H T R A C I N G                  */
#include <math.h>   /*                    My living room                      */
#include <thread>   /*                                           Joris Hartog */
#define R return    /* $ g++ -lm -std=c++11 -o pt pt.cpp                 2019 */
#define O operator  /* $ ./pt > room.ppm                            curlba.sh */
typedef char const* S;typedef std::thread X;typedef float F;F A(F l,F r){R l<r?l
:r;}typedef int I;struct V{F x,y,z;V(F v=0){x=y=z=v;}V(F a,F b,F c=0){x=a;y=b;z=
c;}V O+(V r){R V(x+r.x,y+r.y,z+r.z);}V O*(V r){R V(x*r.x,y*r.y,z*r.z);}F O%(V r)
{R x*r.x+y*r.y+z*r.z;}V O!(){R*this*(1/sqrtf(*this%*this));}};I C(S a,I p,I l){I
r=0;while(l)if(++p%8&&p%8!=2){r=(r<<1)+(128>>((p%8))&a[p/8]?1:0);l--;}R r;}F r()
{R(F)rand()/RAND_MAX;}


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

  float roomDist = -A(// Room
                        BoxTest(dup, V(-1, 0, -9), V(10, 12, 21)),
                        // Window
                        BoxTest(dup, V(9, 3, -6), V(13, 10, 0)));
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
      -A(
        -BoxTest(position, V(4.1, 1.3, -7.2), V(4.3, 1.5, -7.17)),
        -SphereTest(position, V(4.2, 1.4, -7.2), 0.05)
        ),
      -A(
        -BoxTest(position, V(4.7, 1.3, -7.2), V(4.9, 1.5, -7.17)),
        -SphereTest(position, V(4.8, 1.4, -7.2), 0.05)
        )
      );
  if (roomDist < distance) distance = roomDist, hitType = 6;

  roomDist = A(A(
      -A(
      -BoxTest(position, V(7.8, 6.5, -8.5), V(9.8, 6.8, -6.5)),
      -SphereTest(position, V(8.8, 8, -7.5), 1.5)
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

  roomDist = -A(
    -A(
      BoxTest(position, V(2.5, 0.2, 3), V(9.7, 2.7, 7)),
      BoxTest(position, V(3.5, 1, 2.7), V(8.7, 3, 7))
      ),
    BoxTest(position, V(3.4, 2, 2), V(8.8, 4, 6))
    );
  if (roomDist < distance) distance = roomDist, hitType = 3;

  roomDist = -A(
    -CilinderTest(position, V(6.5, 1.5, .7), 0.14, 1.5),
    CilinderTest(position, V(6.5, 1.52, .7), 0.2, 1.49)
  );
  if (roomDist < distance) distance = roomDist, hitType = 2;

  float sun = 11 - position.x;
  if (sun < distance) distance = sun, hitType = 1;

  return distance;
}


I M(V o,V v,V&p,V&n){I t,c=0;F d;for(F a=0;a<99;a+=d)if((d
=Q(p=o+v*a,t))<0.01||++c>99)R n=!V(Q(p+V(.01,0),c)-d,Q(p+V(0,.01),c)-d,Q(p+V(0,0
,.01),c)-d),t;R 0;}

V T(V o,V d){V q,n,e,a=1,w(!V(1,1,3));for(I x=3;x--;){I t=M(o
,d,q,n);if(t==1){e=e+a*V(51,81,99);break;}F i=n%w;if(t>3){d=d+n*(n%d*-2);o=q+d*
0.1;a=a*(t==6?0.6:(t==4?0.01:0.05));if(t!=5)d=!(d+V(r(),r(),r())*0.2);}if(t>1&&t
<4){F p=6.28*r(),c=r(),s=sqrtf(1-c),g=n.z<0?-1:1,u=-1/(g+n.z),v=n.x*n.y*u;d=V(v,
g+n.y*n.y*u,-n.y)*(cosf(p)*s)+V(1+g*n.x*n.x*u,g*v,-g*n.x)*(sinf(p)*s)+n*sqrtf(c)
;o=q+d*.1;a=a*0.2;}if(t!=5&&i>0&&M(q+n*.1,w,q,n)==1)e=e+a*(t==3?V(200,600,400):(
t==4?V(100):V(500,400,100)))*i;}R e;}void t(V* a,V b,V c){*a=*a+T(b,c);}I main()
{I w=320,h=180,s=1;V o(1,5,9),g=!(V(8,4,-8)+o*-1),l=!V(g.z,0,-g.x)*(1./w),u(g.y*
l.z-g.z*l.y,g.z*l.x-g.x*l.z,g.x*l.y-g.y*l.x);printf("P6 %d %d 255 ",w,h);X**e=(X
**)malloc(sizeof(X*)*s);for(I y=h;y--;)for(I x=w;x--;){V c;for(I p=s;p--;)e[p]=
new X(t,&c,o,!(g+l*(x-w/2+r())+u*(y-h/2+r())));for(I p=s;p--;)e[p]->join();c=c*(
1./s)+14./241;V o=c+1;c=V(c.x/o.x,c.y/o.y,c.z/o.z)*255;printf("%c%c%c",(I)c.x,(I
                                )c.y,(I)c.z);}}


  /* Stack based system with the following instructions:
   * - 0bX0X0 0... SphereTest: [4.2,4.8,8.8] a 2 bits (4.2 + 0b10*4 + 0b01*0.6)
   *                      [1.4,8.0] a 1 bit (1.4 + 0b1*6.6)
   *                      [-7.2,-7.5] a 1 bit (-7.5 + 0b1*0.3)
   *                      [0.05,1.50] a 1 bit (0.05 + 0b1*1.45)
   * - 0bX1X. .... BoxTest: [-1>4.45] a 12 bits ((-1 + 0b111111111100)*10^-0b11)
   *                   [0>9.40] a 9 bits (0b111111100*10^-0b11)
   *                   [-9>3.0] a 8 bits ((-9 + 0b11111110)*10^-0b1)
   *                   [2.1>9.80] a 12 bits (0b111111111100*10^-0b11)
   *                   [0.6>6.80] a 12 bits (0b111111111100*10^-0b11)
   *                   [-8.6>21.0] a 10 bits ((-86 + 0b1111111100*)10^-0b11)
   * - 0bX0X0 1... CilinderTest: [6.5,8.8] a 1 bit (6.5 + 0b1*2.3)
   *                        [0, 1.5, 1.52] a 2 bits (0b10*1.5 + 0b01*0.02)
   *                        [-7.5,0.7] a 1 bit (-7.5 + 0b1*8.2)
   *                        [0.1>1.40] a 10 bits (0b1111111100*10^-0b11)
   *                        [0.1>1.49] a 10 bits (0b1111111100*10^-0b11)
   * - 0bX0X1 1... SetObject(0b111)
   * - 0bX0X1 0000 '0' Min(stack.pop(2))
   * - 0bX0X1 0001 '1' if(stack.pop(1) < 1)
   * - 0bX0X1 0010 '2' Invert(stack.pop(1))
   * - 0bX0X1 0100 '4' ToggleDuplicate
   * - 0bX0X1 0110 '7' endif
   *
   * ASCII characters are printable between 0b00100000 and 0b01111111, so the
   * first bit must be zero and the third bit must be 1. This means we only have
   * 6 bits of data per character, with a bitmask of 0b01011111.
   */


/// The program:
//  padding -> "("
//  hitType = 2 -> ":"
//  Boxtest (-1, 0, -9, 10, 12, 21) -> "`     h p2a"
//  Boxtest (9, 3, -6, 13, 10, 0) -> "`4 p& t h%9"
//  Min() -> "0"
//  Invert() -> "2"
//  BoxTest(dup, V(10.7, 3, -6), V(11, 10, 0)) -> "cj`p& l h%9"
//  if (stack.pop(1) < 1) { -> "1"
//    BoxTest(dup, V(10.7, 3, -6), V(10.8, 6.7, -5.4)); -> "cj`p&&q$-4\"" + "0"
//    BoxTest(dup, V(10.9, 6.5, -6), V(11, 10, -5.4)); -> "cnp4& l h4\"" + "0"
//    BoxTest(dup, V(10.7, 3, -0.6), V(10.8, 6.7, 0)); -> "cj`ri&q$-%9" + "0"
//    BoxTest(dup, V(10.9, 6.5, -0.6), V(11, 10, 0)); -> "cnp6i l h%9" + "0"
//    BoxTest(dup, V(10.7, 3, -6), V(10.8, 3.6, 0)); -> "cj`p&&q\"1%9" + "0"
//    BoxTest(dup, V(10.9, 9.4, -6), V(11, 10, 0)); -> "cnwd& l h%9" + "0"
//    BoxTest(dup, V(10.7, 3, -3.2), V(10.8, 6.5, -2.8)); -> "cj`qu&q$%#i" + "0"
//    BoxTest(dup, V(10.9, 6.5, -3.2), V(11, 10, -2.8)); -> "cnp5u l h#i" + "0"
//    BoxTest(dup, V(10.7, 6.3, -6), V(10.8, 6.7, 0)); -> "cjot&&q$-%9" + "0"
//  } -> "7"
//  ToggleDuplicate() -> "4"
//  BoxTest(V(2, 0, -8.8), V(7, 2.5, -7.2)); -> "`&  # <!e(n"
//  if (stack.pop(1) < 1) { -> "1"
//    BoxTest(V(2, 0.5, -8.8), V(2.1, 2.5, -7.2)); -> "`&!4#!5!e(n" + "0"
//    BoxTest(V(4.45, 0.5, -8.8), V(4.55, 2.5, -7.2)); -> "q#!4#<>!e(n" + "0"
//    BoxTest(V(6.9, 0.5, -8.8), V(7, 2.5, -7.2)); -> "b>a4# <!e(n" + "0"
//    BoxTest(V(2, 2.3, -8.8), V(7, 2.5, -7.2)); -> "`&%t# <!e(n" + "0"
//    BoxTest(V(2, 0.5, -8.8), V(7, 0.6, -7.2)); -> "`&!4# < 9(n" + "0"
//    BoxTest(V(2, 0, -7.4), V(2.1, 1, -7.2)); -> "`&  ?!5 $(n" + "0"
//    BoxTest(V(6.9, 0, -7.4), V(7, 1, -7.2)); -> "b>` ?!5 $(n" + "0"
//    BoxTest(V(2, 0, -8.8), V(2.1, 1, -8.6)); -> "`&  #!5 $  " + "0"
//    BoxTest(V(6.9, 0, -8.8), V(7, 1, -8.6)); -> "b>` # < $  " + "0"
//  } -> "7";
//  BoxTest(V(2.14, 0.64, -8.8), V(4.41, 2.26, -7.2)); -> "iu0(#;f.&(n" + "0"
//  BoxTest(V(4.59, 0.64, -8.8), V(6.86, 2.26, -7.2)); -> "q?0(#jz.&(n" + "0"
//  CilinderTest(position, V(6.5, 1.5, .7), 0.14, 1.5); -> "*ch/0"
//  Invert(); -> "2"
//  CilinderTest(position, V(6.5, 1.52, .7), 0.2, 1.49); -> "+`f5`"
//  Min(); -> "0"
//  Invert(); -> "2"
//  Min(); "0"
//  hitType = 3 -> ";"
//  BoxTest(V(2.5, 0.2, 3), V(9.7, 2.7, 7)); -> "a&`d8&%!m)q"
//  BoxTest(V(3.5, 1, 2.7), V(8.7, 3, 7));   -> "a:`3k%= ,)q"
//  Min(); -> "0"
//  Invert(); -> "2"
//  BoxTest(V(3.4, 2, 2), V(8.8, 4, 6)); -> "a8``6%a 0))"
//  Min(); -> "0"
//  Invert(); -> "2"
//  Min(); -> "0"
//  hitType = 4; -> "<"
//  BoxTest(V(7.8, 6.5, -8.5), V(9.8, 6.8, -6.5)); -> "bpp4+&)$1-&"
//  Invert(); -> "2"
//  SphereTest(position, V(8.8, 8, -7.5), 1.5); -> "'!"
//  Invert(); -> "2"
//  Min(); -> "0"
//  Invert(); -> "2"
//  Min(); -> "0"
//  CilinderTest(position, V(8.8, 0, -7.5), 6.5, 0.03); -> ",04#`"
//  Min(); -> "0"
//  CilinderTest(position, V(8.8, 0, -7.5), 0.1, 0.6); -> ", 4&0"
//  Min(); -> "0"
//  hitType = 5; -> "="
//  BoxTest(V(2.2, 2.6, -8.8), V(6.8, 5, -8.6)); -> "a fd#$1 4  " + "0"
//  hitType = 6; -> ">"
//  BoxTest(V(4.1, 1.3, -7.2), V(4.3, 1.5, -7.17)); -> "afc4e\"m }(z"
//  Invert(); -> "2"
//  SphereTest(V(4.2, 1.4, -7.2), 0.05); -> " \""
//  Invert(); -> "2"
//  Min(); -> "0"
//  Invert(); -> "2"
//  BoxTest(V(4.7, 1.3, -7.2), V(4.9, 1.5, -7.17)); -> "arc4e#% }(z"
//  Invert(); -> "2"
//  SphereTest(V(4.8, 1.4, -7.2), 0.05); -> "\"\""
//  Invert(); -> "2"
//  Min(); -> "0"
//  Invert(); -> "2"
//  Min(); -> "0"
//  Min(); -> "0"
