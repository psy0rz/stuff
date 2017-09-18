/**
 * The good old standard 3mm LEDs.
 *
 * @filename: standard3mm.scad
 * @uri: https://github.com/htho/fzz2scad-lib/lib/electronic_components/active/semiconductors/diodes/leds/standard3mm.scad
 * @tag-list: led, 3mm
 * @category-list: electronic_component, active, semiconductor, led
 * 
 * @author: Hauke Thorenz <htho@thorenz.net>
 * @version: 0.1
 * @license-short: cc-by-sa-4.0
 * @license: This work is licensed under the Creative Commons
 * Attribution-ShareAlike 4.0 International License.
 * To view a copy of this license, visit
 * http://creativecommons.org/licenses/by-sa/4.0/.
 * @license-link: http://creativecommons.org/licenses/by-sa/4.0/
 */

/**
 * A 3mm LED. High Quality Version.
 *
 * @uri: https://github.com/htho/fzz2scad-lib/lib/electronic_components/active/semiconductors/diodes/leds/standard3mm.scad
 * @note: fzz2scad-compatible, see https://github.com/htho/fzz2scad
 * @tag-list: ed, 3mm, hq, fzz2scad-compatible
 * @category-list: electronic_component, active, semiconductor, led
 * 
 * @author: Hauke Thorenz <htho@thorenz.net>
 * @version: 0.1
 * @license-short: cc-by-sa-4.0
 * @license: This work is licensed under the Creative Commons
 * Attribution-ShareAlike 4.0 International License.
 * To view a copy of this license, visit
 * http://creativecommons.org/licenses/by-sa/4.0/.
 * @license-link: http://creativecommons.org/licenses/by-sa/4.0/
 *
 * @datasheet: https://cdn-reichelt.de/documents/datenblatt/A500/LED_3MM_GE.pdf
 * @fritzing: https://github.com/fritzing/fritzing-parts/blob/master/svg/core/pcb/3mm_LED.svg
 * 
 * @variable-dependency: commonLegDiameter
 * @variable-dependency: commonLegLength
 *
 * @param: distanceFromPCB Sometimes the LED does not touch the pcb.
 * 
 */
module led3mm_hq(distanceFromPCB=0)
{
  rm = 2.54;
  
  dSocket=3.85;
  hSocket=1.0;
  
  dMain=3.0;
  hMain=4.3;
  
  hCathode=commonLegLength-commonLegLength/2;  
  hAnode=commonLegLength;

  dCathodeAnode = commonLegDiameter;
  translate([0,0,distanceFromPCB]) //moving according to distanceFromPCB
  rotate([0,0,90]){ //rotating it so the flat edge is on the "upper" side (2D) which is the "back" side in 3D 
  translate([-(rm/2),0,0]){ //Translating so the center is on connector0
    union(){
      difference(){
        cylinder(hSocket, d=dSocket); //Socket
          translate([dSocket/2,0,hSocket/2]) cube([(dSocket-dMain), dSocket, hSocket+1], true);
        }
      
        translate([0,0,hSocket]) cylinder(hMain-(dMain/2), d=dMain); //main
        translate([0,0,hSocket]) translate([0,0,hMain-(dMain/2)]) sphere(d=dMain);
      
        translate([-(rm/2),0,-hAnode/2]) cube([dCathodeAnode,dCathodeAnode,hAnode], true); //anode
        translate([+(rm/2),0,-hCathode/2]) cube([dCathodeAnode,dCathodeAnode,hCathode], true); //cathode
      }
    }
  }
}

/**
 * A 3mm LED. Drill Only Version.
 *
 * @uri: https://github.com/htho/fzz2scad-lib/lib/electronic_components/active/semiconductors/diodes/leds/standard3mm.scad
 * @note: fzz2scad-compatible, see https://github.com/htho/fzz2scad
 * @tag-list: led, 3mm, do, fzz2scad-compatible
 * @category-list: electronic_component, active, semiconductor, led
 * 
 * @author: Hauke Thorenz <htho@thorenz.net>
 * @version: 0.1
 * @license-short: cc-by-sa-4.0
 * @license: This work is licensed under the Creative Commons
 * Attribution-ShareAlike 4.0 International License.
 * To view a copy of this license, visit
 * http://creativecommons.org/licenses/by-sa/4.0/.
 * @license-link: http://creativecommons.org/licenses/by-sa/4.0/
 *
 * @source: https://cdn-reichelt.de/documents/datenblatt/A500/LED_3MM_GE.pdf
 * @source: https://github.com/fritzing/fritzing-parts/blob/master/svg/core/pcb/3mm_LED.svg
 *
 * @param: drillDepth The depth of the drill for this part.
 * @param: distanceFromPCB Sometimes the LED does not touch the pcb.
 */
module led3mm_do(drillDepth, distanceFromPCB=0)
{
  rm = 2.54;
  dCyl = 3;

  translate([0,0,distanceFromPCB]) //moving according to distanceFromPCB
  rotate([0,0,90]){ //rotating it so the flat edge is on the "upper" side (2D) which is the "back" side in 3D
    translate([-(rm/2),0,0]){ //Translating so the center is on connector0
      cylinder(h=drillDepth, d=dCyl); //create the object.
    }
  }
}

/**
 * A 3mm LED. Low Quality Version.
 *
 * @uri: https://github.com/htho/fzz2scad-lib/lib/electronic_components/active/semiconductors/diodes/leds/standard3mm.scad
 * @note: fzz2scad-compatible, see https://github.com/htho/fzz2scad
 * @tag-list: ed, 3mm, lq, fzz2scad-compatible
 * @category-list: electronic_component, active, semiconductor, led
 *
 * @author: Hauke Thorenz <htho@thorenz.net>
 * @version: 0.1
 *
 * @license-short: cc-by-sa-4.0
 * @license: This work is licensed under the Creative Commons
 * Attribution-ShareAlike 4.0 International License.
 * To view a copy of this license, visit
 * http://creativecommons.org/licenses/by-sa/4.0/.
 * @license-link: http://creativecommons.org/licenses/by-sa/4.0/
 *
 * @source: https://cdn-reichelt.de/documents/datenblatt/A500/LED_3MM_GE.pdf
 * @source: https://github.com/fritzing/fritzing-parts/blob/master/svg/core/pcb/3mm_LED.svg
 *
 * @param: distanceFromPCB Sometimes the LED does not touch the pcb.
 * 
 */
module led3mm_lq(distanceFromPCB=0)
{
  rm = 2.54;
  dCyl = 3;
  hCyl = 4.58;

  translate([0,0,distanceFromPCB]) //moving according to distanceFromPCB
  rotate([0,0,90]){ //rotating it so the flat edge is on the "upper" side (2D) which is the "back" side in 3D 
    translate([-(rm/2),0,0]){ //Translating so the center is on connector0
      cylinder(h=hCyl, d=dCyl); //create the object.
    }
  }
}
