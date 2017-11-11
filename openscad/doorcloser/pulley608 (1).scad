//
// pulley608
//
// (c)Jeremie Francois (jeremie.francois@gmail.com)
//	http://www.thingiverse.com/MoonCactus
//	http://betterprinter.blogspot.com
//
// Licence: Attribution - Share Alike - Creative Commons
//

tol=0.05;
$fn=50;

// Bearing dimensions
brg_608_th=7;
brg_608_outer_d=22;
brg_608_inner_d=8;
brg_608_inner_shoulder_d=10;

// Diameter of the pulley within the groove
pulley_cable_d= brg_608_outer_d+2*3;
pulley_shoulder= 2; // lateral shoulder thickness

nail_d= 4.6; // your nail or screw diameter (better keep it tight)

brg_tightness=-0.4; // optional additional friction to press-fit the bearing in place (inside=cover & outside=pulley) - or negative to give some free play

// The leg to get a second point of attachment for the cover/internal axis
cover_leg_outer_d= 8; // zero or negative to hide the leg
cover_leg_inner_d= 3;

// The cover properties
cover_angular_size= 70; // how wide is the cover in degrees, zero to disable
cover_gap=0.6; // gap between the cover and the bearing
cover_th= pulley_shoulder; // thickness of the cover plate


// These are not really parameters
reinforced_legs=true; // add longitudinal paths for slicers
pulley_d= pulley_cable_d+brg_608_th;
pulley_overhang_correction= pulley_d/30; // controls how deep is the groove of the pulley

module debug_bearing_608()
{
	%difference()
	{
		cylinder(r= brg_608_outer_d/2, h=brg_608_th);
		translate([0,0,-tol]) cylinder(r= brg_608_inner_d/2, h=brg_608_th + 2*tol);
		translate([0,0,-tol]) cylinder(r= 17.5/2, h=0.66);
		translate([0,0,brg_608_th-0.66+tol]) cylinder(r= 17.5/2, h=0.66);
	}
}

//translate([0,0,pulley_shoulder]) debug_bearing_608();

module pulley()
{
	difference()
	{
		cylinder(r= pulley_d/2-pulley_overhang_correction, h= pulley_shoulder*2 + brg_608_th);
		translate([0,0,-tol]) cylinder(r= brg_608_outer_d/2 - pulley_shoulder, h= pulley_shoulder+2*tol);
		
		translate([0,0,pulley_shoulder-tol]) cylinder(r= brg_608_outer_d/2-brg_tightness/2, h= brg_608_th+pulley_shoulder+2*tol);

		translate([0, 0, brg_608_th/2 + pulley_shoulder])
		rotate_extrude(convexity = 10)
			translate([pulley_d/2, 0,0])
				circle(r=brg_608_th/2);
	}
}

module cover_sector()
{
	// Use this to define how far the cover goes (here it defines a quadrant)
	translate([0,0,-1])
	hull()
	{
		for(a=[-cover_angular_size/2:30:cover_angular_size/2-0.01])
			rotate([0,0,a])
				cube([0.1,pulley_d/2+brg_608_th+3,brg_608_th+2*cover_th+2*cover_gap+3]);
		rotate([0,0,cover_angular_size/2])
			cube([0.1,pulley_d/2+brg_608_th+3,brg_608_th+2*cover_th+2*cover_gap+3]);
	}
}

cover_r= pulley_d/2+cover_gap;
//
// To reduce the gap between the cover and the pulley that results from the pulley overhang correction, you could define cover_r as:
//   cover_r= pulley_d/2+cover_gap-pulley_overhang_correction;
// But you would have to print the cover with support then, because it adds overhangs on the cover in return...
//
leg_outer_pos= pulley_d/2 + (brg_608_th/2+pulley_shoulder) + cover_leg_outer_d/2 + cover_gap;
module cover(bottom_side=true)
{
	color([0.15,0.5,0.25])
	difference()
	{
		union()
		{
			// The hollow bearing axis
			if(bottom_side==true)
			{
				cylinder(r=brg_608_inner_d/2 + brg_tightness/2, h=pulley_shoulder+brg_608_th+pulley_shoulder*0+cover_th+cover_gap);
			}
			// Bottom shoulder
			cylinder(r=brg_608_inner_shoulder_d/2, h=pulley_shoulder+cover_th+cover_gap);
			// Base plate
			hull()
			{
				cylinder(r=brg_608_inner_d/2, h=cover_th);
				if(cover_angular_size>0)
				{
					intersection()
					{
						cover_sector();
						cylinder(r= (pulley_d)/2+cover_gap+pulley_shoulder, h= cover_th);
					}
				}
			}

			// Cover outer leg
			if(cover_leg_outer_d>0)
			{
				difference()
				{
					union()
					{
						hull()
						{
							cylinder(r=brg_608_inner_d/2, h=cover_th);
							translate([0,leg_outer_pos,0])
									cylinder(r= cover_leg_outer_d/2,h= cover_th);
						}
						if(bottom_side==false)
						{
							translate([0,leg_outer_pos,0])
									cylinder(r= cover_leg_outer_d/2,h= cover_th+cover_gap*2+pulley_shoulder*2+brg_608_th);
						}
					}
					// Leg outer hole
					translate([0,leg_outer_pos,-tol])
							cylinder(r= cover_leg_inner_d/2,h= cover_th+cover_gap*2+pulley_shoulder*2+brg_608_th+2*tol);
				}
			}

			if(cover_angular_size>0 && bottom_side)
			{
				intersection()
				{
					cover_sector();
					difference()
					{
						hull()
						{
							//Rounded cover
							translate([0,0,pulley_shoulder+brg_608_th/2+cover_th+cover_gap])
								rotate_extrude(convexity = 10)
									translate([pulley_d/2, 0,0])
										circle(r=brg_608_th/2+pulley_shoulder);
							//Base junction
							translate([0,0,pulley_shoulder/2])
								rotate_extrude(convexity = 10)
									translate([pulley_d/2+pulley_shoulder, 0,0])
										circle(r=pulley_shoulder/2);
						}
						translate([0,0,-tol]) cylinder(r=cover_r, h=pulley_shoulder+brg_608_th+pulley_shoulder+cover_th+cover_gap+2*tol);

						translate([0, 0, brg_608_th/2 + pulley_shoulder + cover_th + cover_gap])
						rotate_extrude(convexity = 10)
							translate([pulley_d/2, 0,0])
								circle(r=brg_608_th/2);

					}
				}
			}
		}
		// The bearing nail/screw axis
		translate([0,0,-tol]) cylinder(r=nail_d/2, h=pulley_shoulder+brg_608_th+pulley_shoulder+cover_th+cover_gap+2*tol);
		if(reinforced_legs)
		{
			// Very thin longitudinal tubes to make sur the slicers will do something robust here
			for(i=[-brg_608_inner_d/2:brg_608_inner_d/10:brg_608_inner_d/2])
				translate([i,-10,cover_th/2])
					rotate([-90,0,0])
						cylinder(r=0.1,h= leg_outer_pos+20, $fn=4);
		}
	}
}
module cover_bottom()
{
	cover(true);
}
module cover_top()
{
	cover(false);
}


module show(show_bearing=true, show_pulley=true, show_coverb=true, show_covert=true)
{
	if(show_bearing)
		translate([0,0,cover_th+cover_gap+pulley_shoulder])
			debug_bearing_608();
	if(show_pulley)
		translate([0,0,cover_th+cover_gap])
			pulley();
	if(show_coverb)
		cover_bottom();
	if(show_covert)
		color([0.8,0.3,0.3]) scale([1,1,-1])
			translate([0,0,-(cover_th*2+cover_gap*2+pulley_shoulder*2+brg_608_th)])
				cover_top();
}

// Combined view:
	show(false,true,false,false);
// Else call separately the elements:
//
// pulley();
// cover_bottom();
// cover_top();
