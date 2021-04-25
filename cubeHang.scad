// flip it for printing
rotate([180,0,0]) {
difference() {
    union(){
    difference() {
    cylinder(65, 100, 100);
        
        translate([0,0,-2])
        cylinder(68, 98, 98);    
    }
    difference() {
    cylinder(18, 100, 100);
        
        translate([0,0,-2])
        cylinder(22, 92, 92);   
   
        translate([0,0,-79.99])
        cylinder(98, 0, 98);
    }
    }

    // support screw holes
    for (i = [0:2]) {
    rotate([0,0,i*120])
    for (t = [0:2]) {
    rotate([0,0,40 + (t * 5)])
    translate([85,0,8.2])
    rotate([0,90,0])
    cylinder(20, 2.2, 2.2, $fn=16);
    }
    }
    
    // cord wedge
    translate([5,60,4])
    rotate([0,10,105])
    cube([50,50,11]);
    
    // ventilation holes
    for (i = [0:19]) {
        rotate([0,0,i*18])
        translate([90,0,18])
        rotate([45,0,0])
        cube(10);
    }
    for (i = [0:19]) {
        rotate([0,0,(i*18)+(9)])
        translate([90,0,33])
        rotate([45,0,0])
        cube(10);
    }
    for (i = [0:19]) {
        rotate([0,0,(i*18)])
        translate([90,0,48])
        rotate([45,0,0])
        cube(10);
    }
}

// bottom
difference() {
    translate([0,0,65])
    cylinder(15, 100, 85);
        
    translate([0,0,62])
    cylinder(20,8.5,28.5);
    
    translate([0,0,64.99])
    cylinder(13, 98, 83);
}

// cord hole
difference() {
    union() {
        translate([0,0,60])
        cylinder(20,8.5,28.5);
            
        rotate([0,0,16])
        translate([-10,10,70])
        rotate([90,0,0])
        cylinder(20, 10, 10);
    }

    translate([0,0,58])
    cylinder(30,8.5,8.5);

    translate([0,0,62])
    cylinder(20,8.5,28.5);
    
    translate([0,0,40.8])
    cylinder(20,28.5,8.5);
}



// cord post
difference() {
    translate([-70,-20,55])
    rotate([0,30,20])
    cylinder(30, 6, 6);

    translate([-70,-25,80])
        cube(20);
}


}

