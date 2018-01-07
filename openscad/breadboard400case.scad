
/*5
3.8
1.8
3.4 aanvoer*/


include <parts.scad>

wall=1.6;

translate([-10,0,0])
{
  difference()
  {
    //main crate
    crate(breadboard400_l+20, breadboard400_w+20, 30, wall);

    //open wall for cables
    translate([wall+5, wall+breadboard400_w+20,10]) cube([breadboard400_l+20-10, wall, 100]);
  }
}


//cable guide block
translate([wall-10,breadboard400_w+wall,wall])
{
  difference()
  {
    //main block
    cube([breadboard400_l+20, 20, 10+5]);

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


    //powerblock
    translate([+10, wall-breadboard400_w, 0])
    {
        translate([2, 40, hlkpm01_h+4])
        {
          rotate([180,0,90])
          {
            hlkpm01();
          }
        }
    }

  }
}

difference()
{
  //breadboard crate
  open_crate(breadboard400_l,breadboard400_w,breadboard400_h, wall,2,3);

  //breadboard substract
  translate([wall, wall, wall])
  {
    breadboard400();
  }

}

translate([wall, wall, wall])
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
}
