$fa=.5;
$fs=.5;

// wheel body dimensions
dia1 = 50; // 
dia2 = 60;
cone = 1/20; // slope of of wheel body rolling on track 
b = 16;

// ri = 3;
d_axis = 8;
d_pent = 12; // corresponds to object "hex_axis.scad"

module motor_mount(){
  height = 10;
   difference(){
  union(){
  intersection(){
    difference(){
      translate([0,0,height/2]) cube([31, 24, height], center=true);
       union(){
         translate([-5,0,22]) rotate([0,90,0]) cylinder(d=39, h=24, center=true); 
         translate([-0,0,22]) rotate([0,90,0]) cylinder(d=36, h=40, center=true); 
         translate([13.6-5.8/2,0,22]) rotate([0,90,0]) cylinder(d=39, h=4.2, center=true); 
       }
    }
    translate([11,0,0]) rotate([0,0,0]) scale([1.5, 1, 1]) cylinder(d=30, h=40, center=true); 
 }
  translate([-10,0,-6]) cube([51, 24, 12], center=true);
  difference(){
    translate([-33.5,0,-6]) cube([8, 39, 12], center=true);
    translate([-37.5,0,-6]) cube([3, 35, 14], center=true);
  }
 }
     #translate([-28,0,-6]) rotate([0,90,0]) cylinder(d=4.2, h=20, center=true); 

 }
}

// motor_mount();

module torus(dia, thickness){
  //translate([0, 0, -thickness/2])
  rotate_extrude(angle=360, convexity = 100)
  translate([dia/2, 0, 0]) 
  circle(d = thickness);
}

module ring(da, d1, b){
  difference(){
    cylinder(h=b, d=da, center=true);
    cylinder(h=b+2, d=d1, center=true);
  }
}

module neg_rad(dia_i, camfer){
  roty = 0;
  difference(){
    union(){
      translate([0, 0, 0]) torus(dia_i, camfer*2);
      //rotate(0, roty, 0) cylinder(h=b+10, d=dia2+20, center=false);
      rotate(0, roty, 0) translate([0, 0, 0]) ring(dia_i+40, dia_i, 2*camfer);;
    };
  }
}

module wheel_body(dia1, dia2, slope, width){
    union(){
      cylinder(h=3, d=dia2, center=false);
      cylinder(h=width, d1=dia1, d2=dia1*(1-slope), center=false);
    }
}

module free_wheel(){
  difference(){
    // union(){
    //  cylinder(h=3, d=dia2, center=false);
    //  cylinder(h=b, d1=dia1, d2=dia1*(1-cone), center=false); 
    // }
    wheel_body(dia1, dia2, cone, b);
  
    // inner boring
    translate([0,0,-1]) cylinder(h=b+2, d=19, center=false);
    // bearing seats for 608 Z
    translate([0,0,-1]) cylinder(h=8, d=22, center=false);
    translate([0,0,9]) cylinder(h=8, d=22, center=false);
  }
}

// free_wheel();

module pentanut_wheel(){
  difference(){
    wheel_body(dia1, dia2, cone, b);
    translate([0, 0, -1]) cylinder(h=b+2, d=d_pent, $fn=5);}
}
pentanut_wheel();


// ring(d2+40, d2, 10);
// Top
// module top(){
//   difference(){
//      translate([0, 0, h]) roundedBox(l, b, h, r);
