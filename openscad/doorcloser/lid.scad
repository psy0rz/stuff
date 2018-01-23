include <pulley608lib.scad>

$fn=50;

thickness=1;
translate([0,0,thickness]) difference()
{
    cylinder(r= brg_608_outer_d/2 - pulley_shoulder, h= thickness);
    cylinder(r= brg_608_outer_d/2 - pulley_shoulder-1, h= thickness);

}

cylinder(r= brg_608_outer_d/2 - pulley_shoulder+1, h= thickness);
