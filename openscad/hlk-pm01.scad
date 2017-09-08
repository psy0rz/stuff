$fn=100;
color("black"){
    l=34;
    w=20.2;
    h=15;
    cube([l,w,h]);
    
    translate([l/2-29.4/2, w/2-5/2, 0])  cylinder(d=0.8,h=h+5);
    translate([l/2-29.4/2, w/2+5/2, 0])  cylinder(d=0.8,h=h+5);
    translate([l/2+29.4/2, w/2-15.4/2, 0])  cylinder(d=0.8,h=h+5);
    translate([l/2+29.4/2, w/2+15.4/2, 0])  cylinder(d=0.8,h=h+5);
    
}