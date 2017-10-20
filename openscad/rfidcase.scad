/*
d5 3M ohm
d6 100K ohm
d0 rgb led pin 1 (from flat side)
d1 rxd rfid
d2 txd rfid
d4 rstpdn rfid
d3 led pin 3
d7 transister base
vcc led pin 2


D5 -[ 3M   ] ---- sense wire
                |
D6 -[ 100K ] ---|
                |
gnd ---|>-------|
     5v zener           
              

D7 - [ 1k ] -- transistor base

(open collector to Cisa lock coil, with flyback diode)
         
  
TODO: 

*/


include <parts.scad>

commonLegLength=0;
commonLegDiameter=0;
include <standard5mm.scad>


wall=1.21;

boxh=40; //height of the case 
boxw=breadboard170_l+0.4; //width of the case (fits breadboard length)
window=19; //depth of the window (so on the wood it will be boxh-window heigh)

//box length (window part + wood part)
boxlwindow=pn532_w+0.1;  //rfid fits in width direction
boxlwood=30;


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
                crate(boxw, boxlwindow, boxh, wall);


                //rfid holder
                translate([boxw-pn532_l,0,0]) crate(pn532_l, boxlwindow, pn532_h, wall);
                
            }

            //breadboard points
            translate([wall,wall,20]) breadboard170();
        }

        //start breadboard rim
        rimthick=wall*2;
        translate([wall,wall,20-rimthick])
        {
            difference()
            {
                cube([boxw, breadboard170_w, rimthick]);
                translate([wall, wall,0]) cube([boxw-wall*2, breadboard170_w-wall, rimthick]);
            }
        }

        //breadboard model
        //translate([wall,wall,20]) breadboard170();


        //boxpart on wood
        translate([0,wall+boxlwindow,window])
        {
                crate(boxw, boxlwood,boxh-window, wall);

                //powerblok holder
                translate([0,boxlwood-hlkpm01_w,0]) crate(hlkpm01_l, hlkpm01_w, 10, wall);
        }
    }

    //5v hole
    color("red") 
    translate([10,wall+boxlwindow, boxh-5]) 
    rotate([-90,0,0]) 
    cylinder(h=wall+0.1,d=5);

   //220v hole lower
   color("red") 
   translate([boxw+wall, boxlwindow+boxlwood-18, window+wall+4]) 
   rotate([0,90,0]) 
   cylinder(h=wall+0.1,d=6);

   //220v hole upper
   color("red") 
   translate([0, boxlwindow+wall*2+5, window+wall+4]) 
   rotate([0,90,0]) 
   cylinder(h=wall+0.1,d=6);

   //lock control hole
   color("red") 
   translate([boxw+wall, boxlwindow+boxlwood-8, window+wall+3.5]) 
   rotate([0,90,0]) 
   cylinder(h=wall+0.1,d=5);


   //sloped window edge
   color("red") 
   translate([0,boxlwindow+wall,0]) 
    rotate([-45,0,0]) 
    cube([boxw+wall*2,wall,wall*2]);

   //led
   translate([2, boxlwindow-3.5,10])
   color("blue") 
   rotate([180,0,0]) 
   cylinder(d=5,h=boxh);

   //rfid board removal help 
   translate([boxw/2,boxlwindow/2,0])
   cylinder(d=3,h=wall, center=true);


   //start of box woodpart corner
   color("red") 
   translate([wall, wall*2+boxlwindow, window])
   {
      //screw holes
      translate([5,5,0]) cylinder(h=wall+0.1,d=3);
      translate([5,boxlwood-5,0]) cylinder(h=wall+0.1,d=3);
      translate([boxw-5,5,0]) cylinder(h=wall+0.1,d=3);
      translate([boxw-5,boxlwood-5 ,0]) cylinder(h=wall+0.1,d=3);
   }

}




