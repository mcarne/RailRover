$fa = 0.5;
$fs = 0.5;

// Sechskant
d_hex = 6.5;
d_welle = 8;
laenge_hex = 19; // +1
laenge_pent = 16;
laenge_welle = 25;
// d_aussen = 11;
d_pent = 12;
// l_pent = 10;
// wall=3;


union(){
  translate([0,0,0]) cylinder(h=laenge_hex, d=d_hex, $fn=6);
  translate([0,0,18]) cylinder(h=1.5, d=d_welle);
  translate([0,0,19.5]) cylinder(h=laenge_pent, d=d_pent, $fn=5);
  translate([0,0,35.5]) cylinder(h=laenge_welle, d=d_welle);
}

