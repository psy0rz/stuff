$fn=100;
h=200;
d=14;
wall=0.8;
outer_d=d+wall*2;


use <threads_v2p1.scad>;


difference()
{
    cylinder(d=outer_d, h=h);
    translate([0,0,3]) cylinder(d=d, h=h-3);
}



hook_d=3;
hook_h=5;
difference()
{
    translate([-d/2,-hook_d/2,h-5-hook_h])
        cube([d,hook_d,hook_h]);

    translate([-0.4,-hook_d/2,h-5-hook_h])
        cube([1,0.4,hook_h]);
        
    translate([-0.4,+hook_d/2-0.4,h-5-hook_h])
        cube([1,0.4,hook_h]);
    
}

