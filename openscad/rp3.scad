
$fn=100;
rotate([0,180,0]) import("Raspberry_Pi_3_Cover.stl");

//cr-10 mount
difference()
{
    translate([-25,1,-1])
    { 
        cube([50,3,10]);
    }


    translate([-25/2,5,4])
    { 
        rotate([90,0,0])
        {
            cylinder(d=4,h=10);
            translate([25,0,0]) cylinder(d=4,h=10);
        }    
    }
}


//solve petg printing problems?
translate([-25,-12.1,-57])
{ 
    cube([50,1.6,5]);
}
