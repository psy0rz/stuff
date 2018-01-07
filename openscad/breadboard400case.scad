
/*5
3.8
1.8
3.4 aanvoer*/


include <parts.scad>

$fn=100;
wall=1.4;

//margins for pressfitting
margin=0.1;
module guide(offset, angle, diameter)
{
  translate([offset,0,10]) rotate([angle,0,0]) cube([diameter, 100,100]);
  translate([offset,0,10]) cube([diameter, 100,100]);

}

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
        translate([wall+5, wall+(breadboard400_w+margin)+20,wall]) cube([(breadboard400_l+margin)+20-10, wall, 100]);
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
        translate([5,5,5]) cube([10, 3.3,100]);
        translate([0,5,0]) guide(5, -30, 3.3);

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
        guide(35, -30, 3.7);
        guide(45, -30, 3.7);
        guide(55, -30, 3.7);


        //1.8 mm
        guide(65, -30, 1.7);
        guide(70, -30, 1.7);
        guide(75, -30, 1.7);

        //5 mm
        guide(80, -30, 4.8);
        guide(90, -30, 4.8);



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

  //screw holes
  translate([-4,7,0]) cylinder(d=3.5,h=wall);
  translate([breadboard400_l+7, 7, 0]) cylinder(d=3.5,h=wall);

  translate([-4, breadboard400_w-7, 0]) cylinder(d=3.5,h=wall);
  translate([breadboard400_l+7, breadboard400_w-7, 0]) cylinder(d=3.5,h=wall);


  //vent holes
  for(offset = [0 : 5 : breadboard400_l])
    translate([offset,0,15]) rotate([0,30,0]) cube([1,wall,15]);

}
