$fn=100;


pn532_l=42.4;
pn532_w=40.3;
pn532_h=7;
module pn532()
{
  color("red") {
      difference()
      {
          cube([pn532_l,pn532_w,pn532_h]);    
  //        cyclinder(h=h, d=3);
      }
  }
}

hlkpm01_w=20.2;
hlkpm01_l=34;
hlkpm01_h=15;

module hlkpm01()
{
  color("black")
  {
    l=hlkpm01_l;
    w=hlkpm01_w;
    h=hlkpm01_h;
    cube([l,w,h]);

    translate([l/2-29.4/2, w/2-5/2, 0])  cylinder(d=0.8,h=h+5);
    translate([l/2-29.4/2, w/2+5/2, 0])  cylinder(d=0.8,h=h+5);
    translate([l/2+29.4/2, w/2-15.4/2, 0])  cylinder(d=0.8,h=h+5);
    translate([l/2+29.4/2, w/2+15.4/2, 0])  cylinder(d=0.8,h=h+5);

  }
}



breadboard170_l=46.5;
breadboard170_w=34.5;
breadboard170_h=9.5;
module breadboard170()
{
    color ("white")
    {
        l=breadboard170_l;
        w=breadboard170_w;
        h=breadboard170_h;
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
}


wemosd1mini_w=25.5;
wemosd1mini_l=34.6;
wemosd1mini_h=6.5;

module wemosd1mini()
{
  color("blue")
  {
    wemosd1mini_w=25.5;
    wemosd1mini_l=34.6;
    wemosd1mini_h=6.5;
    cube([wemosd1mini_l,wemosd1mini_w,wemosd1mini_h]);
  }
}




module crate(w,l,h,wall)
{
    difference()
    {
        cube([w+wall*2,  l+wall*2,  h+wall]);
        translate([wall,wall,wall]) cube([w, l, h+wall]);
    }
}


module trapezoid(base,top,height, depth)
{

    polyhedron(
        points=[
            //front
            [0,0,0],
            [base,0,0],
            [(base/2)+(top/2),height,0],
            [(base/2)-(top/2),height,0],

            //rear
            [0,0,depth],
            [base,0,depth],
            [(base/2)+(top/2),height,depth],
            [(base/2)-(top/2),height,depth],
        ],

        faces=[
            [0,1,2,3], //front
            [7,6,5,4], //rear
            [0,4,5,1], //bottom
            [2,6,7,3], //top
            [0,3,7,4], //left
            [5,6,2,1], //right

        ]

    );
}
