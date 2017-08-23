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

        //achterkraag 
        translate([0,0,lengte-3*wand])
        {
            difference()
            {
                union()
                {
                    //achterkraag
                    cylinder(d=kraagdiameter, h=wand*3);
                    //klemhouder
                    translate([-(kraagdiameter/2+klemhouder),-kraagdiameter/2,0])
                    {
                        cube([kraagdiameter/2+klemhouder,kraagdiameter,wand*3],false);
                    }
                }

                //uitsparing voor klemhouder
                translate([-(kraagdiameter/3),-kraagdiameter/2,wand])
                {
                    cube([kraagdiameter/2,kraagdiameter,wand],false);
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
                
                //uitsparing voor koper rondom
                translate([0,0,wand])
                {
                    cylinder(d=kraagdiameter, h=wand);

                }
                
            }
            
            
            
        }

        //buitendiameter kern
        cylinder(d=binnendiameter+2*wand, h=lengte);
    }
    
    //binnendiameter kern uithollen
    cylinder(d=binnendiameter, h=lengte);
}

