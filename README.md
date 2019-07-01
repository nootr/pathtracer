# Pathtracer

```c++
#include <stdio.h>  /*                                                        */
#include <math.h>   /*                 P A T H T R A C I N G                  */
#include <stdlib.h> /*                    My living room                      */
#define R return    /*                                                        */
#define O operator  /* $ g++ -lm -std=c++11 -o pt pt.cpp       For more info: */
typedef char const* /* $ ./pt > room.ppm                            curlba.sh */
S;typedef float F;typedef int I;struct V{F x,y,z;V(F v=0){x=y=z=v;}V(F a,F b,F c
=0){x=a;y=b;z=c;}V O+(V r){R V(x+r.x,y+r.y,z+r.z);}V O*(V r){R V(x*r.x,y*r.y,z*r
.z);}F O%(V r){R x*r.x+y*r.y+z*r.z;}V O!(){R*this*(1/sqrtf(*this%*this));}};F A(
F l,F r){R l<r?l:r;}I p;S w="`!  \"0\"`!`!4p)(,!`#0!0  :l:v# 8 l 4  $xul& p-b$-"
"alo&up4& + %$-exul& t-b$-  /&up4&`+ %  !xul& p-b\"1  /&uwd& + %  !xul&$$-b$%`x"
"o&up4``+ %$'%xum~`p-b$-  -; ( !8`' ,l2%. 1\"jq j`98d+v}h56)qx#+$a>(k\"jq . 98d"
"+p\")=6(!p#+$a> 1\"jq . &8d+p\"  2h%4 )$a>(j \"5 . !(d+p\"  6(%4 )%9>(j \"q . "
"!(k+s6l\"6)n8<-$a>y=`2q5=#ahd+*#!z#pz!zhh-,0/ e*s4t8,l$p8,*!m .81l\"3<*z , .-6"
"$3!$$\"p`$ ##4t8g.\"jl,2$1b\"j&qp2{u34rl0 9=!0',k$ &7 4#+1`ls2l0b(!2*rt84l:i$%"
": }v;*%5}2)935a>qjd08h#w9lh6'u(ee-3:x";I C(I l=1){I r=0;while(l)if(++p%8&&p%8!=
2){r=(r<<1)+((128>>(p%8))&w[p/8]?1:0);l--;}R r;}F D(I b=1,I e=0){I m=C(1);R(m?-1
:1)*C(b)/pow(10,C(e));}F r(){R(F)rand()/RAND_MAX;}F Q(V o,I&q){V s=o;while(s.z>1
&&s.z<16)s.z-=8;F h,b,u[9],v[9];p=-1;I t=2,i=0;while(w[p/8])if(C()){F a=D(10,2),
b=D(7, 2),c=D(7, 1),x=D(10,2),y=D(10,2),z=D(10,2);V l=s+V(a,b,c),j=V(x,y,z)+s*-1
;u[++i]=-A(A(A(l.x,j.x),A(l.y,j.y)),A(l.z,j.z));v[i]=t;}else if(C())if(C()){if(u
[i]<u[--i]){u[i]=u[i+1];v[i]=v[i+1];}}else if(C()){if(C()){if(C())break;else{s=o
;}}else{u[i]*=-1;}}else{if(C()){if(u[i--]>1)C(675);}else t++;}else if(C()){F a=D
(7,1),b=D(8,2),c=D(7,1),x=D(7,2),y=D(8,2);V e=V(a,b,c),d=s+e*-1,f=d,g=e+V(0,x)+o
*-1;d.y=0;u[++i]=-A(y-sqrtf(d%d),A(f.y,g.y));v[i]=t;}else{F a=D(7,1),b=D(4,1),c=
D(7,1),x=D(4,2);V d=s+V(a,b,c);u[++i]=sqrtf(d%d)-x;v[i]=t;}h=u[i];q=v[i];F g=11-
o.x;if(g<h)h=g,q=1;R h;}I M(V o,V v,V&p,V&n){I t,c=0;F d;for(F a=0;a<99;a+=d)if(
(d=Q(p=o+v*a,t))<0.01||++c>99)R n=!V(Q(p+V(.01,0),c)-d,Q(p+V(0,.01),c)-d,Q(p+V(0
,0,.01),c)-d),t;R 0;}V T(V o,V d){V q,n,e,a=1,w(!V(1,1,3));for(I x=3;x--;){I t=M
(o,d,q,n);if(t==1){e=e+a*V(51,81,99);break;}F i=n%w;if(t>3){d=d+n*(n%d*-2);o=q+d
*0.1;a=a*(t==6?0.6:(t==4?0.01:0.05));if(t!=5)d=!(d+V(r(),r(),r())*0.2);}if(t>1&&
t<4){F p=6.28*r(),c=r(),s=sqrtf(1-c),g=n.z<0?-1:1,u=-1/(g+n.z),v=n.x*n.y*u;d=V(v
,g+n.y*n.y*u,-n.y)*(cosf(p)*s)+n*sqrtf(c)+V(1+g*n.x*n.x*u,g*v,-g*n.x)*(sinf(p)*s
);o=q+d*.1;a=a*0.2;}if(t!=5&&i>0&&M(q+n*.1,w,q,n)==1)e=e+a*(t==3?V(2,6,4)*100:(t
==4?V(1):V(5,4,1))*100)*i;}R e;}I main(){I w=160,h=90,s=8;V o(1,5,9),g=!(V(8,4,-
8)+o*-1),l=!V(g.z,0,-g.x)*(1./w),u(g.y*l.z-g.z*l.y,g.z*l.x-g.x*l.z,g.x*l.y-g.y*l
.x);printf("P6 %d %d 255 ",w,h);for(I y=h;y--;)for(I x=w;x--;){V c;for(I p=s;p--
;)c=c+T(o,!(g+l*(x-w/2+r())+u*(y-h/2+r())));c=c*(1./s)+14./241;V o=c+1;c=V(c.x/o
.x,c.y/o.y,c.z/o.z)*255;printf("%c%c%c",(I)c.x,(I)c.y,(I)c.z);}} // Joris Hartog
```

## What?
This program is a codegolf renderer; it's a program that contains a model of my
living room and generates an image of it when executed. To make things
interesting for me, I decided to try my best to keep the code as short as
possible (although I'm certain the size could be reduced a lot more).

For whoever is interested: this renderer does anti-aliasing and simulates a
camera with focus point, although you can't really see that from the image.

## Why?
I was inspired by Andrew Kensler's business card raytracer[1] to learn about
renderers and create one myself. `Raytracing in one weekend`[2] was an excellent
source.

## How?
The renderer consists of two parts: a simple *Virtual Machine* which calculates
the distance to the nearest object and a function which calculates the direction
that a lightray will go after being reflected of a certain surface.

I chose to write a Virtual Machine as it could shorten the code necessary to
describe the objects in the model of my living room. The opcodes and their
arguments are smaller than a byte and the VM will read out the program bit by
bit.

I also wrote an extremely basic programming language and a *compiler* for the VM
which makes sure not a bit of data is wasted as it is optimized for this
specific 3D-model.

Note that you can adjust the resolution of the resulting image and the
sample rate (higher means less noise) by adjusting the values for the `w`, `h`
and `s` variables on line 35.

## Useful links
[1]: http://fabiensanglard.net/rayTracing_back_of_business_card/
[2]: https://github.com/petershirley/raytracinginoneweekend
