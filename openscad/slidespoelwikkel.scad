//winding tool for Errel fruitmachine part:  coin slide coil.  (http://www.topgokkasten.nl/slide-errel/)
//(c)edwin@datux.nl

wand=1.5;
binnendiameter=18;
lengte=36;

//past binnen in coil + ruimte voor steun
cylinder(d=binnendiameter, h=lengte+10, $fn=100);

//past in accuboor
translate([0,0,lengte+10])
{
    cylinder(d=10, h=30, $fn=100);
}

//achtersteun
translate([-5,0,0])
{
    cube([10,20+wand,10],false);
}
translate([-5,10+wand,10])
{
    cube([10,10,3*1.6/2],false);
}


