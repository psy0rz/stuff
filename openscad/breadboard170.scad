$fn=100;

color ("white")
{
    l=46.5;
    w=34.5;
    h=9.5;
    difference(){
        union()
        {
            //main
            cube([l,w,h]);
            //notch
            bl=4.3;
            bh=5;
            bw=1.5;
            translate([l/2-bl/2, -bw, 0]) cube([bl,bw,bh]);
            translate([-bw, w/2-bl/2, 0]) cube([bw,bl,bh]);
        }
        translate([4.8,w/2,0]) cylinder(d=3.5,h=h);
        translate([l-4.8,w/2,0]) cylinder(d=3.5,h=h);
    }
}

