$fn=100;

width=100;
length=500;
height=30;
bottom=10;
radius=width/2-10;
holes=6;


color("green"){
	difference()
	{
		cube([ width,length,height ]);
	
		for ( hole=[1:holes-1] )
		{				
			translate([width/2, hole* (length/holes)   , bottom]){
				cylinder(r=radius, h=height);
			}
		}
	}
}
 

for ( hole=[1:holes-1] )
{				
	translate([width/2, hole* (length/holes)   , bottom]){

		//aluminium houder
		difference()
		{
			color("silver") cylinder(r=radius-5, h=height);
			translate([0,0,1]) cylinder(r=radius-6, h=height);
		}
		//kaarsje en lont
		translate([0,0,2]) color("white") cylinder(r=radius-7, h=height);
		translate([0,0,15 ]) color("black") cylinder(r=1.5, h=height);

		//glaasje
		color("silver",0.5) difference()
		{
			cylinder(r1=radius-2, r2=radius, h=height*2);
			translate([0,0,1]) cylinder(r1=radius-4, r2=radius-2, h=height*2);
		}

	}
}

