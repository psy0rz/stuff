
/*5
3.8
1.8
3.4 aanvoer*/


include <parts.scad>

$fn=100;
wall=1.4;

//margins for pressfitting
margin=0.3;
module guide(offset, angle, diameter)
{
  translate([offset,0,10]) rotate([angle,0,0]) cube([diameter, 100,100]);
  translate([offset,0,10]) cube([diameter, 100,100]);

}

case_l=82;
case_w=55;
case_h=30;

difference()
{

  union()
  {

    difference()
    {
      //main crate
      crate(case_l, case_w, 30, wall);

      //open wall for cables
      translate([wall+5, 0,wall]) cube([case_l-10, wall, case_h]);
    }


    //cable guide block
    translate([wall, wall, wall])
    {
      difference()
      {
        //main filling material
        cube([case_l, 20, 15]);



        for(offset = [6 : 22 : 60])
        {

          translate([offset+0,0,0]) rotate([30,0,0]) cube([4.8, 100,100]); //5mm
          translate([offset+9,0,0]) rotate([30,0,0]) cube([3.7, 100,100]); //3.8mm
          translate([offset+16,0,0]) rotate([30,0,0]) cube([1.7, 100,100]); //1.8mm
        }

        //powerblock cable guide (flat 3.4mm)
        translate([case_l-9,0,0]) rotate([30,0,0]) cube([3.4, 100,100]);


        //powerblock
        /*translate([+10, wall-(breadboard400_w+margin), 0])
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
  guide(90, -30, 4.8);*/



}
}

//breadboard crate
//    open_crate((breadboard400_l+margin),(breadboard400_w+margin),breadboard400_h, wall,2,3);

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

/*
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
translate([offset,0,15]) rotate([0,30,0]) cube([1,wall,15]);*/

}
