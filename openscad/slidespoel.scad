//Errel fruitmachine part:  coin slide coil.  (http://www.topgokkasten.nl/slide-errel/)
//(mine melted away because of a faulty pcb ;)
//(c)edwin@datux.nl

wand=1.5;
binnendiameter=18;
kraagdiameter=40;
lengte=36;
klemhouder=10;

difference()
{
    union()
    {
        //randje voorkant
        translate([0,0,0.5])
        {
            //voorste kraag
            cylinder(d=kraagdiameter, h=wand);
        }

        //binnenkant achterkraag 
        translate([0,0,lengte-3*wand])
        {
            difference()
            {
                union()
                {
                    //binnenste achterkraag
                    translate([0,0,0*WAND])
                    {
                        cylinder(d=kraagdiameter, h=wand);
                        //klemhouder
                        translate([-(kraagdiameter/2+klemhouder),-kraagdiameter/2,0])
                        {
                            cube([kraagdiameter/2+klemhouder,kraagdiameter,wand],false);
                        }
                    }

                    //midden achterkraag (iets kleiner ivm koper begeleiding)
                    translate([0,0,1*wand])
                    {
                        cylinder(d=kraagdiameter-wand, h=wand);
                        //klemhouder
                        translate([-(kraagdiameter/2+klemhouder),-kraagdiameter/2+wand/2,0])
                        {
                            cube([kraagdiameter/2+klemhouder,kraagdiameter-wand,wand],false);
                        }
                    }
                    
                    //buitenste achterkraag
                    translate([0,0,2*wand])
                    {
                        cylinder(d=kraagdiameter, h=wand);
                        //klemhouder
                        translate([-(kraagdiameter/2+klemhouder),-kraagdiameter/2,0])
                        {
                            cube([kraagdiameter/2+klemhouder,kraagdiameter,wand],false);
                        }
                    }

                    
                }
                

                //uitsparing voor klemmen
                kd=0.8;
                kl=13;
                kb=6.36;
                translate([ -(kraagdiameter/2+klemhouder) ,kraagdiameter/4-kb/2,wand])
                {
                    cube([ kl, kb, kd], false);
                }

                translate([ -(kraagdiameter/2+klemhouder) ,-(kraagdiameter/4+kb/2),wand])
                {
                    cube([ kl, kb, kd], false);
                }

                
                //uitsparing voor koperdoorgang
                uitsparing=10;
                translate([0,-uitsparing/2,0])
                {
                    cube([kraagdiameter,uitsparing,wand*3],false);
                }
                
    
                
            }
            
            
            
        }

        //buitendiameter kern
        cylinder(d=binnendiameter+2*wand, h=lengte);
    }
    
    //binnendiameter kern uithollen
    cylinder(d=binnendiameter, h=lengte);
}

