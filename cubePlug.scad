depth=10;
difference() {
    rotate([.2,.2,0])
rotate([-36,0,0])
rotate([0,45,0]) {
difference(){
cube(30);
    translate([-depth-1,-depth-1,30])
        translate([30,-20,-42])
    rotate([0,35,45])
    cube(70);
    
    translate([depth+depth+depth+1.2,depth+depth+depth+1.2,0])
rotate([55,0,-45])
translate([0,-.5,-28])
cylinder(150,2.7,2.7);
    


translate([.2+8,.2+2,30])
rotate([55,0,-45])
translate([0,0,-5])
cylinder(10,1,1);

translate([.2+2,.2+8,30])
rotate([55,0,-45])
translate([0,0,-5])
cylinder(10,1,1);

translate([0,30,30])
rotate([0,90,-90]) {
translate([.2+8,.2+2,30])
rotate([55,0,-45])
translate([0,0,-5])
cylinder(10,1,1);

translate([.2+2,.2+8,30])
rotate([55,0,-45])
translate([0,0,-5])
cylinder(10,1,1);
}


translate([30,0,30])
rotate([-90,0,90]) {
translate([.2+8,.2+2,30])
rotate([55,0,-45])
translate([0,0,-5])
cylinder(10,1,1);

translate([.2+2,.2+8,30])
rotate([55,0,-45])
translate([0,0,-5])
cylinder(10,1,1);
}

    
}
}


translate([18.7,-7.3,-1])
cube([5.4,20,20]);

translate([21.2,12.2,2])
cylinder(6,8.5,2.7);

translate([21.2,12.2,-3.9])
cylinder(6,8.5,8.5);



}

translate([0,0,0])
difference() {
translate([21.1,12.2,-20])
rotate([0,0,-30])
cylinder(20, 24.2, 24.2, $fn=3);
    
    
translate([21.2,12.2,-21])
    cylinder(22, 8.5, 8.5);
    
translate([18.7,-7.3,-21])
cube([5.4,20,22]);
}