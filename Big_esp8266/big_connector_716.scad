w=16.2; // Al stripe width
t=7.0; // Al thickness
bt=2.5; // Bottom thickness
ww=2.7; // wall width
tt=1.0; // top overhang
ml=55; // main (central) section length
rl=40; // ray section length

ofs=0;
// ofs=-w*sin(60);

module outer(l)
{
translate([0, -w/2-ww, -bt]) cube([l, w+ww*2, bt+t+tt]);
}

module inner(l, ofs)
{
translate([-ofs,0,0]) hull()
{
  translate([-0.1, -w/2, 0]) cube([l+0.2, w, t]);
  translate([-0.1, -w/2+tt+1, 0]) cube([l+0.2, w-tt*2-2, t+tt+1]);
}
}

difference()
{
union()
{
translate([-ml/3,0,0]) outer(ml);
rotate([0,0,60])outer(rl);
rotate([0,0,-60])outer(rl);
}
translate([0,0,0]) inner(ml, ml/3);
rotate([0,0,60]) inner(rl, ofs);
rotate([0,0,-60]) inner(rl, ofs);
}

