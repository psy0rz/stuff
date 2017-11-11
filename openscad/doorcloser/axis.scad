use <threads_v2p1.scad>;

$fn=100;
axis_h=7;
wall_h=5;
screw_d=4.5;

difference()
{
    union()
    {
        //axis inside bearing and screw hole
        CountersunkClearanceHole(screw_d-1, sinkdiam=0)
        {
            cylinder(d=8-0.3,h=axis_h);
        }

        //clearance from wall
        translate([0,0,axis_h])
            cylinder(d=12,h=wall_h);
    }
    
    //screw hole diameter
    cylinder(d=screw_d, h=wall_h+axis_h);
    
}

