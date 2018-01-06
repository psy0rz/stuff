$fn=100;

//x,y,z -> l,w,h

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

//note: bottom is 20.2 and 34.2
hlkpm01_w=20;
hlkpm01_l=34;
hlkpm01_h=15.2;

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



breadboard170_l=46.4;
breadboard170_w=34.5;
breadboard170_h=9.7;
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
                margin=0.2;
                bl=4.45+margin;
                bh=5.3+margin;
                bw=1.7+margin;
                translate([l/2-bl/2, -bw, 1]) cube([bl,bw,bh]);
                translate([-bw, w/2-bl/2, 1]) cube([bw,bl,bh]);
            }
            translate([4.8,w/2,0]) cylinder(d=3.5,h=h);
            translate([l-4.8,w/2,0]) cylinder(d=3.5,h=h);
        }
    }
}


breadboard400_l=82.4;
breadboard400_w=54.7;
breadboard400_h=9.7;
module breadboard400()
{
    color ("white")
    {
        l=breadboard400_l;
        w=breadboard400_w;
        h=breadboard400_h;
        difference(){
            union()
            {
                //main
                cube([l,w,h]);
                //notch
                margin=0.2;
                bl=2.1+margin;
                bh=breadboard400_h;
                bw=1.7+margin;
                translate([13.2, -bw, 0]) cube([bl,bw,bh]);
                translate([breadboard400_l-13.2-bw, -bw, 0]) cube([bl,bw,bh]);

                translate([breadboard400_l, 3.8, 0]) cube([bw,bl,bh]);
                translate([breadboard400_l, breadboard400_w-3.8-bl, 0]) cube([bw,bl,bh]);
                translate([breadboard400_l, (breadboard400_w-bl)/2, 0]) cube([bw,bl,bh]);
                /*translate([], -bw, 0]) cube([bl,bw,bh]);*/

                /*translate([-bw, w/2-bl/2, 0]) cube([bw,bl,bh]);*/
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



//crate , specifiy inner dimensions
module crate(w,l,h,wall)
{
    difference()
    {
        cube([w+wall*2,  l+wall*2,  h+wall]);
        translate([wall,wall,wall]) cube([w, l, h+wall]);
    }
}

//a crate with only the corners
// specifiy inner dimensions, and how much to close it from the bottom and corners
module open_crate(w,l,h,wall,corner, bottom)
{
    difference()
    {
        crate(w,l,h,wall);

        translate([0, wall+corner, wall+bottom]) cube([w+wall*2,  l-corner*2,  h]);
        //same as above, but for x
        translate([wall+corner, 0, wall+bottom]) cube([w-corner*2,l+wall*2,  h]);
    }
}

//a cover that fits inside a crate (a crate that has w,l,h as inner dimensions)
//basically a open_crate with a rimm that sticks out by crate_wall mm
module cover(w,l,h,wall,corner, bottom,crate_wall)
{

        open_crate(w-wall*2,l-wall*2,h, wall, corner, bottom);

        //the rimm
        translate([-crate_wall, -crate_wall,0]) cube([w+crate_wall*2, l+crate_wall*2, wall]);
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


module test(s,h)
{
    for (a =[0,90,180,270])
    {
        rotate([0,0,a+45])
        {
            translate([-s/2,-s/2,0])
            {
                cube([s*3,s,h]);
                translate([s*2,-s*2,0])cube([s,s*2,h]);
            }
        }
    }
}


module test2(h,wall)
{
    r=1.3 * h;
    b=0.4 * h;

    center= sqrt(pow(r,2)-pow((h/2),2))+b/2;

    difference()
    {
        translate([-h/2,-b/2,0])cube([h,b,wall]);
        translate([0,center,0]) color("red") cylinder(d=r*2, wall);
        translate([0,-center,0]) color("red") cylinder(d=r*2, wall);
    }


    difference()
    {
        translate([-b/2,-h/2,0])cube([b,h,wall]);
        translate([center,0,0]) color("red") cylinder(d=r*2, wall);
        translate([-center,0,0]) color("red") cylinder(d=r*2, wall);
    }
}
