include <parts.scad>

commonLegLength=0;
commonLegDiameter=0;
include <standard5mm.scad>


wall=1;

boxh=40; //height of the case 
boxw=breadboard170_l+0.4; //width of the case (fits breadboard length)
window=19; //depth of the window (so on the wood it will be boxh-window heigh)

//box length (window part + wood part)
boxlwindow=pn532_w+0.1;  //rfid fits in width direction
boxlwood=30;


//cover
cover(boxw,boxlwood+boxlwindow+wall, 12,wall,10, 0, wall);

translate([-wall,-wall,0]) crate(boxw,boxlwood+boxlwindow+wall, 7,wall);
