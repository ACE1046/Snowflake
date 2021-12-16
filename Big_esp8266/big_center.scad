w=20.2; // Al stripe width
t=2.0; // Al thickness
bt=2.0; // Bottom thickness
ww=2.3; // wall width
tt=1.0; // top overhang
ml=35; // section length

ofs=50;
dc=130;
dci=80;
// ofs=-w*sin(60);

module outer(l)
{
translate([0, -w/2-ww, -bt]) cube([l, w+ww*2, bt+t+tt]);
}

module inner(l, ofs)
{
translate([ofs,0,0]) hull()
{
  translate([-0.1, -w/2, 0]) cube([l+0.2, w, t]);
  translate([-0.1, -w/2+tt+1, 0]) cube([l+0.2, w-tt*2-2, t+tt+1]);
}
}

rotate([0,0,30])difference()
{
union()
{
translate([0,0,-bt])rotate([0,0,15])cylinder(h=5, d=dc, $fn=12);
for (i = [0 : 5]) rotate([0,0,60*i]) translate([ofs,0,0]) outer(ml);
}
for (i = [0 : 5]) rotate([0,0,60*i]) translate([0,0,0]) inner(ml, ofs);
translate([0,0,-bt-1])rotate([0,0,15])cylinder(h=5+2, d=dci, $fn=12);
for (i = [0 : 5]) rotate([0,0,60*i+30]) translate([-7,0,t]) cube([15, 100, tt+1]);
}




