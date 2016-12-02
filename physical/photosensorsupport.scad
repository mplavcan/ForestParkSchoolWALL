// Raised support for photosensor
//

support_side = 20;
support_height = 11;

hole_radius = 1.5;
hole_offset = 3.8;
INF = 100000;

difference() {
    cube([support_side, support_side, support_height], center = true);
    translate([hole_offset, 0, 0]) cylinder(r=hole_radius, h=INF, center=true, $fn=20);
    translate([-hole_offset, 0, 0]) cylinder(r=hole_radius, h=INF, center=true, $fn=20);
}
