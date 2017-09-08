$fn=100;
color("red") {
    l=42.4;
    w=40.3;
    h=7;
    difference()
    {
        cube([l,w,h]);    
//        cyclinder(h=h, d=3);
    }
}
