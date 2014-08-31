//			minkowski()
//			{
//				sphere(r=1, $fn=20);
//		{	}



module tealight_holder(inner_height=30, outer_height=16)
{
	color("white", 0.4)
	difference()
	{
		hull()
		{
			//outer hex
			cylinder(r=69/2, h=outer_height,$fn=6);
			//inner hex (sticks out below and above)
			translate([0,0,-(inner_height-outer_height)/2])
			{
				cylinder(r=51/2, h=inner_height, $fn=6);
			}
		}
		//hollow round inside
		translate([0,0,14 -(inner_height-outer_height)/2])
		{
			cylinder(r=41/2, h=100, $fn=30);
		}

	}
}


module tealight()
{
	$fn=200;
	color("silver")
	{
		difference()
		{
			//metal can outside
			cylinder(r=39.7/2, h=14.6);
			//inside
			translate([0,0,0.27]) cylinder(r=39.7/2-0.27, h=14.6);
		}
	}
	//candle
	color("white") cylinder(r=37.6/2, h=13.1);		
	//fuse
	color("black")
	linear_extrude(height = 20, center = false, twist = 130)
	{
		translate([2, 0, 0])
		circle(r = 1);
	}

}

tealight_holder();
translate([0,0,5]) tealight();

