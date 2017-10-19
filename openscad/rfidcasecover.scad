include <parts.scad>

commonLegLength=0;
commonLegDiameter=0;
include <standard5mm.scad>



wall=1;
window=19;
boxh=40;
woodboxw=30;


//cover
cover(breadboard170_l,pn532_w+wall+woodboxw, 12,wall,10, 0, wall);

//we need extra room :(
translate([-wall,-wall,0]) crate(breadboard170_l,pn532_w+wall+woodboxw, 7,wall);
