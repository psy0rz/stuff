use <trapezoid.scad>;
$fn=100;

//translate([0,0,10])rotate([-90,0,0])  difference()
//{

//translate([-110.3,-20,-5]) import("/home/psy/Downloads/CR-10_Cable_strain_relief.stl");
    
//translate([-200,-200,-10]) cube([1000,1000,10]);

//}

//pin
color("red")  translate([0,6,10]) rotate([90,0,0]) cylinder(d=3.8,h=6);

color("red") translate([-5.1,6,2.1]) rotate([90,0,0]) trap(10.2, 6, 3.8,15);

color("red") translate([+5.1,15,17.9]) rotate([90,180,0]) trap(10.2,6.5,3.8,16);

//pinnetjes vasthouder
translate([-10,-10,0])cube([20,10,20]);

translate([0,-10,10])cube([30,10,10]);


difference()
{
    translate([-10,0,-10]) rotate([0,60,0]) cylinder(d=20,h=150);
    translate([-10,0,0])cube([20,10,20]);
    
    //-top
    translate([-20,-20,-20]) cube([50,40,20]);
    
    //-floor
    translate([-100,-100,37]) cube([2000,2000,2000]);

    //-inside
    translate([-30,-10,0]) cube([20,20,20]);
}