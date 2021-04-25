whiteSquare = 65;
depth = 10;
insideBase = whiteSquare - (depth * 2);
notch = 2;
portWid = 6.5;
portHei = 2.5;
pad = 4.166;


difference() { // joins

translate([0,0,0]) {
difference(){

    cube([220,220,depth*2]);
    
    translate([-1,-1,depth+1])
    cube([300,300,depth*2]);
    
    
    /*
    translate([4.33+(whiteSquare/2)-(insideBase/2),4.33+(whiteSquare/2)-(insideBase/2),0]) {
    for (y = [0:2]) {
        for (x = [0:2]) {
            translate([((whiteSquare+8.66)*x),((whiteSquare+8.66)*y),1])
            cube([insideBase,insideBase,100]);
            
        }}
    }
    */
    
    translate([pad,pad,0]) {
        for (y = [0:2]) {
        for (x = [0:2]) {
            translate([((whiteSquare+(pad*2))*x),((whiteSquare+(pad*2))*y),depth]) {
            cube([whiteSquare,whiteSquare,depth*2]);
                
                translate([0,(notch*3),-notch*1.5]){
                cube([notch,notch,notch*20]);
                    translate([notch,0,0])
                    cube([.2,notch,notch]);
                }
                
                translate([whiteSquare-notch-(notch*3),0,-notch*1.5]){
                cube([notch,notch,notch*20]);
                    translate([0,notch,0])
                    cube([notch,.2,notch]);
                }
                
                translate([(notch*3),whiteSquare-notch,-notch*1.5]){
                cube([notch,notch,notch*20]);
                    translate([0,-.2,0])
                    cube([notch,.2,notch]);
                }
                
                translate([whiteSquare-notch,whiteSquare-notch-(notch*3),-notch*1.5]) {
                cube([notch,notch,notch*20]);
                    translate([-.2,0,0])
                    cube([.2,notch,notch]);
                }
            }
            
        }}
    }
    
    
// frusta
translate([.5*(whiteSquare - insideBase)+4.33,.5*(whiteSquare - insideBase)+pad,0]) {
        for (y = [0:2]) {
        for (x = [0:2]) {
            
            translate([((whiteSquare+(pad*2))*x),((whiteSquare+(pad*2))*y),1]) {
polyhedron( [
  [  0,  0,  0 ],  //0
  [ insideBase,  0,  0 ],  //1
  [ insideBase,  insideBase,  0 ],  //2
  [  0,  insideBase,  0 ],  //3
  [  -.5*(whiteSquare - insideBase),  -.5*(whiteSquare - insideBase),  depth+1 ],  //4
  [ -.5*(whiteSquare - insideBase)+whiteSquare,  -.5*(whiteSquare - insideBase),  depth+1 ],  //5
  [ -.5*(whiteSquare - insideBase)+whiteSquare,  -.5*(whiteSquare - insideBase)+whiteSquare,  depth+1 ],  //6
  [  -.5*(whiteSquare - insideBase),  -.5*(whiteSquare - insideBase)+whiteSquare,  depth+1 ]], [
  [0,1,2,3],  // bottom
  [4,5,1,0],  // front
  [7,6,5,4],  // top
  [5,6,2,1],  // right
  [6,7,3,2],  // back
  [7,4,0,3]] );
}
}}
}


// rims
translate([-1,0,depth+1-(2 * (sqrt(450)) )])
rotate([45,0,0])
cube([300,30,30]);


translate([-1,220,depth+1-(2 * (sqrt(450)) )])
rotate([45,0,0])
cube([300,30,30]);

translate([depth+1-(2 * (sqrt(450)) ),-1,0])
rotate([0,45,0])
cube([30,300,30]);

translate([209,-1,0])
rotate([0,45,0])
cube([30,300,30]);



// inner rails
difference() {
translate([-1,(pad*2)+whiteSquare,-depth])
rotate([45,0,0])
cube([300,14,14]);

translate([(whiteSquare/2)+pad,-10,-20])
cube([2,300,100]);
translate([(whiteSquare/2)+pad+whiteSquare+(pad*2),-10,-20])
cube([2,300,100]);
translate([(whiteSquare/2)+pad+whiteSquare+whiteSquare+(pad*4),-10,-20])
cube([2,300,100]);

}

difference() {
translate([-1,(pad*2)+whiteSquare+whiteSquare+(pad*2),-depth])
rotate([45,0,0])
cube([300,14,14]);

translate([(whiteSquare/2)+pad,-10,-20])
cube([2,300,100]);
translate([(whiteSquare/2)+pad+whiteSquare+(pad*2),-10,-20])
cube([2,300,100]);
translate([(whiteSquare/2)+pad+whiteSquare+whiteSquare+(pad*4),-10,-20])
cube([2,300,100]);
}

difference() {
translate([(pad*2)+whiteSquare+(pad*2)+whiteSquare,-20,-depth])
rotate([45,0,90])
cube([300,14,14]);

translate([-10,(whiteSquare/2)+pad,-20])
cube([300,2,100]);
translate([-10,(whiteSquare/2)+pad+whiteSquare+(pad*2),-20])
cube([300,2,100]);
translate([-10,(whiteSquare/2)+4.33+whiteSquare+whiteSquare+(pad*4),-20])
cube([300,2,100]);
}

difference() {
translate([(pad*2)+whiteSquare,-20,-depth])
rotate([45,0,90])
cube([300,14,14]);
   
translate([-10,(whiteSquare/2)+pad,-20])
cube([300,2,100]);
translate([-10,(whiteSquare/2)+pad+whiteSquare+(pad*2),-20])
cube([300,2,100]);
translate([-10,(whiteSquare/2)+pad+whiteSquare+whiteSquare+(pad*4),-20])
cube([300,2,100]);
}

// wire ports
    translate([26.33,11.5,-1])
        for (y = [0:2]) {
        for (x = [0:2]) {
            translate([((whiteSquare+(pad*2))*x),((whiteSquare+(pad*2))*y),0])   
            cube([portWid,portHei,20]);
        }}
        
    translate([41.33,59.4,-1])
        for (y = [0:2]) {
        for (x = [0:2]) {
            translate([((whiteSquare+(pad*2))*x),((whiteSquare+(pad*2))*y),0])   
            cube([portWid,portHei,20]);
        }}
        
    translate([14.1,44.33,-1])
        for (y = [0:2]) {
        for (x = [0:2]) {
            translate([((whiteSquare+(pad*2))*x),((whiteSquare+(pad*2))*y),0])   
            rotate([0,0,90])
            cube([portWid,portHei,20]);
        }}
        
    translate([62.2,26.33,-1])
        for (y = [0:2]) {
        for (x = [0:2]) {
            translate([((whiteSquare+(pad*2))*x),((whiteSquare+(pad*2))*y),0])   
            rotate([0,0,90])
            cube([portWid,portHei,20]);
        }}


translate([-10,pad+pad+whiteSquare,depth+1])
rotate([0,90,0])
scale([1,2.5,1])
cylinder(300,.5,.5);

translate([-10,pad+pad+pad+pad+whiteSquare+whiteSquare,depth+1])
rotate([0,90,0])
scale([1,2.5,1])
cylinder(300,.5,.5);
        
translate([pad+pad+whiteSquare,290,depth+1])
rotate([90,0,0])
scale([2.5,1,1])
cylinder(300,.5,.5);
        
translate([pad+pad+whiteSquare+pad+pad+whiteSquare,290,depth+1])
rotate([90,0,0])
scale([2.5,1,1])
cylinder(300,.5,.5);



// power tube
translate([depth+1.2,depth+1.2,0])
rotate([55,0,-45])
translate([0,0,-28])
cylinder(150,2.7,2.7);

translate([depth+1.2+8,depth+1.2+2,0])
rotate([55,0,-45])
translate([0,0,-5])
cylinder(10,1,1);

translate([depth+1.2+2,depth+1.2+8,0])
rotate([55,0,-45])
translate([0,0,-5])
cylinder(10,1,1);

        
        
        
 

}



//joins

translate([0,0,0]) {
translate([pad+pad+whiteSquare-1,2.4,0])
cube([1,depth+1,depth+.5]);
translate([pad+pad+whiteSquare+1.5,4,0])
cube([1,depth+1,depth+.5]);
translate([pad+pad+whiteSquare+pad+pad+whiteSquare-1,2.4,0])
cube([1,depth+1,depth+.5]);
translate([pad+pad+whiteSquare+pad+pad+whiteSquare+1.5,4,0])
cube([1,depth+1,depth+.5]);
}

translate([220,0,0])
rotate([0,0,90])
translate([0,0,0]) {
translate([pad+pad+whiteSquare-1,2.4,0])
cube([1,depth+1,depth+.5]);
translate([pad+pad+whiteSquare+1.5,4,0])
cube([1,depth+1,depth+.5]);
translate([pad+pad+whiteSquare+pad+pad+whiteSquare-1,2.4,0])
cube([1,depth+1,depth+.5]);
translate([pad+pad+whiteSquare+pad+pad+whiteSquare+1.5,4,0])
cube([1,depth+1,depth+.5]);
}

translate([220,220,0])
rotate([0,0,180])
translate([0,0,0]) {
translate([pad+pad+whiteSquare-1,2.4,0])
cube([1,depth+1,depth+.5]);
translate([pad+pad+whiteSquare+1.5,4,0])
cube([1,depth+1,depth+.5]);
translate([pad+pad+whiteSquare+pad+pad+whiteSquare-1,2.4,0])
cube([1,depth+1,depth+.5]);
translate([pad+pad+whiteSquare+pad+pad+whiteSquare+1.5,4,0])
cube([1,depth+1,depth+.5]);
}


translate([0,220,0])
rotate([0,0,270])
translate([0,0,0]) {
translate([pad+pad+whiteSquare-1,2.4,0])
cube([1,depth+1,depth+.5]);
translate([pad+pad+whiteSquare+1.5,4,0])
cube([1,depth+1,depth+.5]);
translate([pad+pad+whiteSquare+pad+pad+whiteSquare-1,2.4,0])
cube([1,depth+1,depth+.5]);
translate([pad+pad+whiteSquare+pad+pad+whiteSquare+1.5,4,0])
cube([1,depth+1,depth+.5]);
}


}

//// rounded edges
     translate([0,2,depth+1])
rotate([-45+180,0,0])
translate([-1,0,-10])
cube([230,10,20]);


translate([0,218,depth+1])
rotate([45,0,0])
translate([-1,0,-10])
cube([230,10,20]);


     translate([2,0,depth+1])
rotate([0,45+180,0])
translate([0,-1,-10])
cube([10,230,20]);

     translate([218,0,depth+1])
rotate([0,-45,0])
translate([0,-1,-10])
cube([10,230,20]);


}

           




