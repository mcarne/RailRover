$fa = 0.5;
$fs = 0.5;

// dimensions beams
b_y = 44;
b_z = 24.5;

d_drill = 8;

// Drilling template


difference(){
  translate([0,0,5]) cube([b_z+6, b_y+6, 8], center=true);
  translate([0,0,10]) cube([b_z+8, b_y, 8], center=true);
  //bohrungen
  #translate([0,12,-2]) cylinder(h=12, d=2);
  #translate([0,-12,-2]) cylinder(h=12, d=2);
  // schiene
  #translate([-b_z/2,1,4]) cube([b_z, 30, 8]);
}

