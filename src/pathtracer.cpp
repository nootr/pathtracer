// https://mattermost.hostnet.nl/hostnet/pl/pz8uhrwieifpdgtj6enafen4bc
#include <stdio.h>  /*                 P A T H T R A C I N G                  */
#include <math.h>   /*                    My living room                      */
#include <stdlib.h> /*                                           Joris Hartog */
#define R return    /* $ g++ -lm -std=c++11 -o pt pt.cpp                 2019 */
#define O operator  /* $ ./pt > room.ppm                            curlba.sh */
typedef char const* S;typedef float F;F A(F l,F r){R l<r?l
:r;}typedef int I;struct V{F x,y,z;V(F v=0){x=y=z=v;}V(F a,F b,F c=0){x=a;y=b;z=
c;}V O+(V r){R V(x+r.x,y+r.y,z+r.z);}V O*(V r){R V(x*r.x,y*r.y,z*r.z);}F O%(V r)
{R x*r.x+y*r.y+z*r.z;}V O!(){R*this*(1/sqrtf(*this%*this));}};

S w = "`!  \"0\"`!`!4p)(,!`#0!0  :l:v# 8 l 4  $xul& p-b$-alo&up4& + %$-exul& t-b$-  /&up4&`+ %  !xul& p-b\"1  /&uwd& + %  !xul&$$-b$%`xo&up4``+ %$'%xum~`p-b$-  -; ( !8`' ,l2%. 1\"jq j`98d+v}h56)qx#+$a>(k\"jq . 98d+p\")=6(!p#+$a> 1\"jq . &8d+p\"  2h%4 )$a>(j \"5 . !(d+p\"  6(%4 )%9>(j \"q . !(k+s6l\"6)n8<-$a>y=`2q5=#ahd+*#!z#pz!zhh-,0/ e*s4t8,l$p8,*!m .81l\"3<*z , .-6$3!$$\"p`$ ##4t8g.\"jl,2$1b\"j\"q0r{u34rl0 9=!0',k$ &7 4#+1`ls2l0b(!2*rt84l:i$%: }v;*!5=r)935a>qjd08h#w9lh&%w(ee-3:x";
I p;

I C(I l=1) {
  I r=0;
  while (l)
    if (++p % 8 && p % 8 != 2) {
      r = (r<<1) + ((128>>(p%8))&w[p/8]?1:0);
      l--;
    }
  R r;
}

F D(I b=1,I e=0){I m=C(1);R (m?-1:1)*C(b)/pow(10,C(e));}
F r(){R(F)rand()/RAND_MAX;}

float Q(V position, I &hitType) {
  V dup = position; // Used to duplicate window
  while (dup.z > 1 && dup.z < 16) dup.z -= 8;

  F distance, b, dis_stack[9], type_stack[9];
//  fprintf(stderr, "Q start\n");
  p = -1;
  I currentHitType=2, sp = 0;

  while(w[p/8]){
    if(C()) { // 1 box()
      F a=D(10,2), b=D(7, 2), c=D(7, 1),
        x=D(10,2), y=D(10,2), z=D(10,2);
//      fprintf(stderr, "box(%f, %f, %f, %f, %f, %f);\n",
//          -a, -b, -c, x, y, z);
      V l = dup + V(a, b, c),
      u = V(x, y, z) + dup*-1;
      dis_stack[++sp] = -A(A(A(l.x,u.x),A(l.y,u.y)),A(l.z,u.z));
      type_stack[sp] = currentHitType;
    } else { // 0
      if(C()) { // 01
        if(C()) { // 011 min()
//          fprintf(stderr, "min();\n");
          if(dis_stack[sp] < dis_stack[--sp]) {
            dis_stack[sp] = dis_stack[sp+1];
            type_stack[sp] = type_stack[sp+1];
          }
        } else { // 010
          if(C()) { // 0101
            if(C()) { // 01011
              if(C()) { // 010111 halt()
//                fprintf(stderr, "halt();\n");
//                while(1); // for debugging purposes
                break;
              } else { // 010110 toggle_duplicate()
//                fprintf(stderr, "toggle_duplicate();\n");
                dup = position;
              }
            } else { // 01010 invert()
//              fprintf(stderr, "invert();\n");
              dis_stack[sp] *= -1;
            }
          } else { // 0100
            if(C()) { // 01001 if_less_than_one (skip 675 bits)
//              fprintf(stderr, "if_less_than_one -> skip 675 bits\n");
              if(dis_stack[sp--]>1)C(675);
            } else { // 01000 rotate_type()
              currentHitType++;
//              fprintf(stderr, "set_type(%d);\n", currentHitType);
            }
          }
        }
      } else { // 00
        if (C()) { // 001 cilinder()
          F a=D(7,1), b=D(8,2), c=D(7,1), x=D(7,2), y=D(8,2);
//          fprintf(stderr, "cilinder(%f, %f, %f, %f, %f)\n",
//              a, b, c, x, y);
          V bottom = V(a, b, c),
            delta = dup + bottom*-1,
            down = delta,
            up = bottom + V(0, x) + position * -1;
          delta.y = 0;
          dis_stack[++sp] = -A(y - sqrtf(delta % delta), A(down.y, up.y));
          type_stack[sp] = currentHitType;
        } else { // 000 sphere()
          F a=D(7,1), b=D(4,1), c=D(7,1), x=D(4,2);
//          fprintf(stderr, "sphere(%f, %f, %f, %f)\n",
//              a, b, c, x);
          V delta = dup + V(a, b, c)*-1; // TODO: a, b, c inverteren
          dis_stack[++sp] = sqrtf(delta % delta) - x;
          type_stack[sp] = currentHitType;
        }
      }
    }
  }

  // sp zou nu 1 moeten zijn
  distance = dis_stack[sp]; // TODO: verkleinen door dereferencer te gebruiken
  hitType = type_stack[sp]; // TODO: idem

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
t==4?V(100):V(500,400,100)))*i;}R e;}

I main() {
  I w=160,h=90,s=4;
  V o(1,5,9),
    g=!(V(8,4,-8)+o*-1),
    l=!V(g.z,0,-g.x)*(1./w),
    u(g.y*l.z-g.z*l.y,g.z*l.x-g.x*l.z,g.x*l.y-g.y*l.x);

  printf("P6 %d %d 255 ",w,h);
  for(I y=h;y--;)
    for(I x=w;x--;){
      V c;
      for(I p=s;p--;)
        c = c + T(o, !(g+l*(x-w/2+r())+u*(y-h/2+r())));
      c=c*(1./s)+14./241;
      V o=c+1;c=V(c.x/o.x,c.y/o.y,c.z/o.z)*255;printf("%c%c%c",(I)c.x,(I
                                )c.y,(I)c.z);}}


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
