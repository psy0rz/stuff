include <parts.scad>

commonLegLength=0;
commonLegDiameter=0;
include <standard5mm.scad>

wall=2;
window=19;
boxh=40;

//diff for all the holes
difference()
{
    union()
    {

        //diff for breadboard points
        difference()
        {
            union()
            {
                //boxpart on window
                crate(breadboard170_l, pn532_w, boxh, wall);


                //rfid holder
                crate(pn532_l, pn532_w, pn532_h, wall);
                //translate([wall,wall,wall])   pn532();
            }

            //breadboard points
            translate([wall,wall,20]) breadboard170();
        }

        //start breadboard rim
        translate([wall,wall,20-wall])
        {
            difference()
            {
                cube([breadboard170_l, breadboard170_w, wall]);
                translate([wall, wall,0]) cube([breadboard170_l-wall*2, breadboard170_w-wall, wall]);
            }
        }

        //breadboard model
        //translate([wall,wall,20]) breadboard170();


        //boxpart on wood
        translate([0,wall+pn532_w,window])
        {
                crate(breadboard170_l, 30,boxh-window, wall);

                translate([0,10,0]) crate(hlkpm01_l, hlkpm01_w, 10, wall);
        }
    }

    //5v hole
    color("red") 
    translate([10,wall+pn532_w, boxh-5]) 
    rotate([-90,0,0]) 
    cylinder(h=wall+0.1,d=5);

   //220v hole
   color("red") 
   translate([breadboard170_l+wall, pn532_w+breadboard170_w-10, window+wall+6]) 
   rotate([0,90,0]) 
   cylinder(h=wall+0.1,d=6);

   //lock control hole
   color("red") 
   translate([0, breadboard170_w+14, window+wall+5]) 
   rotate([0,90,0]) 
   cylinder(h=wall+0.1,d=5);

}

//wemosd1mini();

//led5mm_hq();
