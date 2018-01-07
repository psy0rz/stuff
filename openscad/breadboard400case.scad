
/*5
3.8
1.8
3.4 aanvoer*/


include <parts.scad>

wall=1.4;

//margins for pressfitting
margin=0.1;

difference()
{

  union()
  {

    translate([-10,0,0])
    {
      difference()
      {
        //main crate
        crate((breadboard400_l+margin)+20, (breadboard400_w+margin)+20, 30, wall);

        //open wall for cables
        translate([wall+5, wall+(breadboard400_w+margin)+20,10]) cube([(breadboard400_l+margin)+20-10, wall, 100]);
      }
    }


    //cable guide block
    translate([wall-10,(breadboard400_w+margin)+wall,wall])
    {
      difference()
      {
        //main filling material
        cube([(breadboard400_l+margin)+20, 20, 10+5]);

        //powerblock cable guide (flat 3.4mm)
        translate([5,5,10]) cube([3.4, 100,100]);
        translate([5,5,10]) cube([10, 3.4,100]);

        //powerblock
        translate([+10, wall-(breadboard400_w+margin), 0])
        {
          translate([2, 40-wall, hlkpm01_h+4])
          {
            rotate([180,0,90])
            {
              hlkpm01();
            }
          }
        }

        //3.8 mm
        translate([35,0,10]) cube([3.8, 100,100]);
        translate([45,0,10]) cube([3.8, 100,100]);
        translate([55,0,10]) cube([3.8, 100,100]);

        //1.8 mm
        translate([65,0,10]) cube([1.8, 100,100]);
        translate([70,0,10]) cube([1.8, 100,100]);
        translate([75,0,10]) cube([1.8, 100,100]);

        //5 mm
        translate([80,0,10]) cube([5, 100,100]);
        translate([90,0,10]) cube([5, 100,100]);



      }
    }

    //breadboard crate
    open_crate((breadboard400_l+margin),(breadboard400_w+margin),breadboard400_h, wall,2,3);

    /*translate([wall, wall, wall])
    {
      //  difference()
      {
        translate([2, 39, hlkpm01_h+10])
        {
          rotate([180,0,90])
          {
            //        hlkpm01();
          }
        }
      }
    }*/
  }


  //breadboard substract
  translate([wall, wall, wall])
  {
    breadboard400();
  }
}
