// https://mattermost.hostnet.nl/hostnet/pl/pz8uhrwieifpdgtj6enafen4bc
#include <stdio.h>  /*                 P A T H T R A C I N G                  */
#include <math.h>   /*                    My living room                      */
#include <stdlib.h> /*                                           Joris Hartog */
#define R return    /* $ g++ -lm -std=c++11 -o pt pt.cpp                 2019 */
#define O operator  /* $ ./pt > room.ppm                            curlba.sh */
typedef char const* S;typedef float F;typedef int I;struct V{F x,y,z;V(F v=0){x=
y=z=v;}V(F a,F b,F c=0){x=a;y=b;z=c;}V O+(V r){R V(x+r.x,y+r.y,z+r.z);}V O*(V r)
{R V(x*r.x,y*r.y,z*r.z);}F O%(V r){R x*r.x+y*r.y+z*r.z;}V O!(){R*this*(1/sqrtf(*
this%*this));}};F A(F l,F r){R l<r?l:r;}I p;

S w = "`!  \"0\"`!`!4p)(,!`#0!0  :l:v# 8 l 4  $xul& p-b$-alo&up4& + %$-exul& t-b$-  /&up4&`+ %  !xul& p-b\"1  /&uwd& + %  !xul&$$-b$%`xo&up4``+ %$'%xum~`p-b$-  -; ( !8`' ,l2%. 1\"jq j`98d+v}h56)qx#+$a>(k\"jq . 98d+p\")=6(!p#+$a> 1\"jq . &8d+p\"  2h%4 )$a>(j \"5 . !(d+p\"  6(%4 )%9>(j \"q . !(k+s6l\"6)n8<-$a>y=`2q5=#ahd+.#a{#pz!zhx/,4/ e*s4t8,l$p8,*!m .81l\"3<*z , .-6$3!$$\"p`$ ##4t8g.\"jl,2$1b\"j\"q0r{u34sl0 )=!0',{$ \"7 4#+1`ls2l0b(!2*rt84l:i$%: }v;*!5=r)935a>qjd08h#w9lh&%w(ee-3:x";

I C(I l=1){I r=0;
while(l)if(++p%8&&p%8!=2){r=(r<<1)+((128>>(p%8))&w[p/8]?1:0);l--;}R r;}F D(I b=1
,I e=0){I m=C(1);R(m?-1:1)*C(b)/pow(10,C(e));}F r(){R(F)rand()/RAND_MAX;}

/*
 * p position
 * q hitType
 * s dup
 * t currentHitType
 * u dis_stack
 * v type_stack
 * w distance
 * i sp
 */

F Q(V position, I &hitType) {
  V dup = position; // Used to duplicate window
  while (dup.z > 1 && dup.z < 16) dup.z -= 8;

  F distance, b, dis_stack[9], type_stack[9];
  p = -1;
  I currentHitType=2, sp = 0;

  while(w[p/8]){
    if(C()) { // 1 box()
      F a=D(10,2), b=D(7, 2), c=D(7, 1),
        x=D(10,2), y=D(10,2), z=D(10,2);
      V l = dup + V(a, b, c),
      u = V(x, y, z) + dup*-1;
      dis_stack[++sp] = -A(A(A(l.x,u.x),A(l.y,u.y)),A(l.z,u.z));
      type_stack[sp] = currentHitType;
    } else { // 0
      if(C()) { // 01
        if(C()) { // 011 min()
          if(dis_stack[sp] < dis_stack[--sp]) {
            dis_stack[sp] = dis_stack[sp+1];
            type_stack[sp] = type_stack[sp+1];
          }
        } else { // 010
          if(C()) { // 0101
            if(C()) { // 01011
              if(C()) { // 010111 halt()
                break;
              } else { // 010110 toggle_duplicate()
                dup = position;
              }
            } else { // 01010 invert()
              dis_stack[sp] *= -1;
            }
          } else { // 0100
            if(C()) { // 01001 if_less_than_one (skip 675 bits)
              if(dis_stack[sp--]>1)C(675);
            } else { // 01000 rotate_type()
              currentHitType++;
            }
          }
        }
      } else { // 00
        if (C()) { // 001 cilinder()
          F a=D(7,1), b=D(8,2), c=D(7,1), x=D(7,2), y=D(8,2);
          V bottom = V(a, b, c),
            delta = dup + bottom*-1,
            down = delta,
            up = bottom + V(0, x) + position * -1;
          delta.y = 0;
          dis_stack[++sp] = -A(y - sqrtf(delta % delta), A(down.y, up.y));
          type_stack[sp] = currentHitType;
        } else { // 000 sphere()
          F a=D(7,1), b=D(4,1), c=D(7,1), x=D(4,2);
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
  I w=160,h=90,s=1;
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
