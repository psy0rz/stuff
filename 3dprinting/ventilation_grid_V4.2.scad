/*----------------------------------------------------------------------------*/
/*-------                         INFORMATION                         --------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

// ventilation grill
// written by Rolf Jethon 72181 Starzach,Germany
// Remix of https://www.thingiverse.com/thing:3320712
// The code has been nearly completely re-written, so we now have several new 
// features like slanted ribs, centered ribs, better control over the edge
// radius and some more.


// HOW TO USE THIS FILE:

// -- Solution 1:
// Open this file with OpenScad software (free and open-source) 
// http://www.openscad.org/downloads.html

// A: Change the parameters according to your needs in the customizer section of Open Scad.
// Or
// B: Choose your parameters in the SETTINGS chapter (see below) 

// Press F5 to compile and see the resulting hook
// Check for messages in the console window
// If OK, press F6 to render
// Go to : File => Export => Export as STL : choose filename and location
// Save the STL file of your customized grill
// Send this STL file to your 3D printer

// -- Solution 2 :
// This file had been optimized for Thingiverse Customizer
// You will need to have a Thingiverse Account, then :
// Go to object's page on Thingiverse
// Click on the button "Open in Customizer"
// Choose your settings in parameters tabs
// Save the STL file of your customized grill
// Send this STL file to your 3D printer

/*----------------------------------------------------------------------------*/
/*-------                           SETTINGS                          --------*/
/*----------------------------------------------------------------------------*/

/* [Frame] */
// All Sizes in mm
// Length of the cut out - add some play
cutout_length=100;//[0:0.1:500] 
// width of the cut out - add some play
cutout_width=40;//[0:0.1:500] 
// Overall height of the whole grill
height=10;//[0.1:0.1:500]
// Thickness of the tube wall
tube_wall=2;//[0.1:0.1:30]
//Rounding of the tube edges
tube_rounding=8;//[0.1:0.1:500]
// Width of the border that later covers the cut out gap
brim_width=10;//[0:0.1:100]
// Edge rounding of the brim
brim_radius=5;//[0.1:0.1:250]
// brim_height must be smaller or equal to height
brim_height=1;//[0.1:0.1:100]

/* [edge] */
//width of the edge
edge_width=1; //[0.1:0.1:10]
//height of the edge (0 to disable)
edge_height=1; //[0:0.1:100]


/* [grill] */
// Size of the holes in x-direction
grid_x=6;//[0.1:0.1:500]
// Size of the holes in y-direction
grid_y=6;//[0.1:0.1:500]
// Grill height
grill_height=5;//[0.1:0.1:500]
// Thickness of the ribs in x-direction
grill_thickness_x=0.8;//[0.1:0.1:100]
// Thickness of the ribs in y-direction
grill_thickness_y=0.8;//[0.1:0.1:100]
// Slanting of the X-ribs
grill_angle_x=0;//[0:1:60]
// Slanting of the Y-ribs
grill_angle_y=0;//[0:1:60]
// Shifting of the ribs in X-direction out of the middle
grill_x_shift=0;//[-1:.01:1]
// Shifting of the ribs in Y-direction out of the middle
grill_y_shift=0;//[-1:.01:1]
// Rotation angle of the grill ribs around the center
grill_rotation=0;//[0:90]


$fn=80;


/*------------------------------------------------------------------------------

NOTES about how to use :

To get round ventilation grills, set the tube_rounding to "maximum" 
and make sure cutout_length and cutout_width has the same value.
To omit either the x or y grill or both make the grill_x or grill_y value larger 
than width or height + diameter and shift the grid out of the middle using 
grill_x_shift respectively grill_y_shift.
To make the grill as high as the height, use for both the same value.
Have a look in the customizer section of OpenScad after loading file
ventilation_grid_V3.json - ther you will see some samples showing the 
possibilities
------------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------*/
/*-------                             CODE                            --------*/
/*----------------------------------------------------------------------------*/
grill_x=grid_x+grill_thickness_x/cos(grill_angle_x);
grill_y=grid_y+grill_thickness_y/cos(grill_angle_y);
mintube=cutout_length > cutout_width ? cutout_width:cutout_length;
diameter=tube_rounding > mintube-tube_wall ? mintube-tube_wall-.01:tube_rounding;   
brim_rounding=brim_radius*2>mintube+brim_width*2 ? mintube+brim_width*2-.01:brim_radius*2;
innerdiam=diameter-2*tube_wall < 0.1 ? 0.1:diameter-2*tube_wall;
length=cutout_length-2*tube_wall-innerdiam;
width=cutout_width-2*tube_wall-innerdiam;
x_center=(length+diameter)%grill_x/2-grill_thickness_x/2;
y_center=(width+diameter)%grill_y/2-grill_thickness_y/2;
realbrim=brim_width*2-brim_rounding;
slanted_grill_height=grill_height*10;
maxsize=length+width+diameter;

//when its a complete cirle, change the diameter and do not offset
edge_rounding=brim_radius*2>mintube+brim_width*2 ? brim_rounding-edge_width:brim_rounding;
//when its still a bit square, only do offsetting
edge_offset=brim_radius*2>mintube+brim_width*2 ? 0:edge_width;

//____________________________________________________________________________
color("#C0C000"){
    cut_grill();
    difference(){ //hollow the frame
      outer_shape();
      scale([1,1,1.1]) 
      translate([0,0,-0.01])  
      inner_shape();  
    }    
    edge();
}  

module edge()
{
    difference()
    {
        //outer (same as brim, but add height)
        translate([-realbrim/2-length/2-diameter/2,-realbrim/2-width/2-diameter/2,0])
        linear_extrude(brim_height+edge_height)
        minkowski(){
            square([length+diameter+realbrim,width+diameter+realbrim]);
            circle(d=brim_rounding);
        }
        
        //inner (offsetted by edge_width)
        
        translate([-realbrim/2-length/2-diameter/2+edge_offset/2,-realbrim/2-width/2-diameter/2+edge_offset/2,-0.1])
        linear_extrude(brim_height+edge_height+0.2)
        minkowski(){
            square([length+diameter+realbrim-edge_offset,width+diameter+realbrim-edge_offset]);
            circle(d=edge_rounding);
        }
    }
    
}


module cut_grill() { //     
  intersection(){
    scale([1,1,grill_height/height])  
    inner_shape();    
    grill();             
  }
}

module inner_shape() {
  translate([-length/2,-width/2,0])  
  linear_extrude(height)  
  minkowski(){
    square([length,width]);
    circle(d=innerdiam);  
  } 
}

module outer_shape(){
union(){
    translate([-realbrim/2-length/2-diameter/2,-realbrim/2-width/2-diameter/2,0])
    linear_extrude(brim_height)
    minkowski(){
        square([length+diameter+realbrim,width+diameter+realbrim]);
        circle(d=brim_rounding);
    }
    translate([-length/2,-width/2,0])
    linear_extrude(height)
    minkowski(){   // 
        square([length,width]);
        circle(d=diameter);
    }
  }
}
     
module grill() {
 rotate([0,0,grill_rotation])   
 union(){
for (a =[0:grill_x:(maxsize)])
{
    translate([(grill_x_shift*grid_x)-a,0,0])
    rotate([0,grill_angle_x,0])
    cube([grill_thickness_x,maxsize,slanted_grill_height],center=true);
    translate([a+grill_x_shift*grid_x,0,0])
    rotate([0,grill_angle_x,0])
    cube([grill_thickness_x,maxsize,slanted_grill_height],center=true);
}

for (a =[0:grill_y:(maxsize)])
{
    translate([0,grill_y_shift*grid_y-a,0])
    rotate([grill_angle_y,0,0])
    cube([maxsize,grill_thickness_y,slanted_grill_height],center=true);
    translate([0,a+grill_y_shift*grid_y,0])
    rotate([grill_angle_y,0,0])
    cube([maxsize,grill_thickness_y,slanted_grill_height],center=true);
}
}   
}