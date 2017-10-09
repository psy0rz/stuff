include <parts.scad>

commonLegLength=0;
commonLegDiameter=0;
include <standard5mm.scad>



wall=1;
window=19;
boxh=40;
woodboxw=30;




//diff for all
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
                translate([breadboard170_l-pn532_l,0,0]) crate(pn532_l, pn532_w, pn532_h, wall);
                
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
                crate(breadboard170_l, woodboxw,boxh-window, wall);

                //powerblok holder
                translate([0,woodboxw-hlkpm01_w,0]) crate(hlkpm01_l, hlkpm01_w, 10, wall);
        }
    }

    //5v hole
    color("red") 
    translate([10,wall+pn532_w, boxh-5]) 
    rotate([-90,0,0]) 
    cylinder(h=wall+0.1,d=5);

   //220v hole
   color("red") 
   translate([breadboard170_l+wall, pn532_w+woodboxw-10, window+wall+6]) 
   rotate([0,90,0]) 
   cylinder(h=wall+0.1,d=6);

   //lock control hole
   color("red") 
   translate([0, breadboard170_w+14, window+wall+5]) 
   rotate([0,90,0]) 
   cylinder(h=wall+0.1,d=5);


   //sloped window edge
   color("red") 
   translate([0,pn532_w+wall,0]) 
    rotate([-45,0,0]) 
    cube([breadboard170_l+wall*2,wall,wall*2]);

   //led
   translate([2, pn532_w-3.5,10])
   color("blue") 
   rotate([180,0,0]) 
   cylinder(d=5,h=boxh);



   //start of box woodpart corner
   color("red") 
   translate([wall, wall*2+pn532_w, window])
   {
      //screw holes
      translate([5,5,0]) cylinder(h=wall+0.1,d=3);
      translate([5,woodboxw-5,0]) cylinder(h=wall+0.1,d=3);
      translate([breadboard170_l-5,5,0]) cylinder(h=wall+0.1,d=3);
      translate([breadboard170_l-5,woodboxw-5 ,0]) cylinder(h=wall+0.1,d=3);
   }

}




