// Case enclosure for:
//   Adafruit TCA9548a I2C multiplexer board
//

// Number of facets for cylinders
$fn = 20;

enclosure_x_dim = 60;
enclosure_y_dim = 50;
enclosure_height = 20;

base_thickness = 3;
wall_width = 2;
wall_height = enclosure_height;

tca9548_standoff_xoffset = 12.5;
tca9548_standoff_yoffset = 12.5;
standoff_hole_radius = 1;

mount_x_offset = 21;
mount_y_offset = 16;
mount_hole_radius = 1.6;

BELOW_PLANE = -1;
INF = 100000;

difference() {
     // outer extents
    cube([enclosure_x_dim ,enclosure_y_dim ,enclosure_height], center = true);
    translate([0,   0, base_thickness])cube([56,46,20], center = true);
 
    // cutout inside to form walls
    translate([0,   0, base_thickness])cube([enclosure_x_dim -(2*wall_width),enclosure_y_dim -(2*wall_width), wall_height], center = true);
 
    // TCA9548 mounting hols
    translate([tca9548_standoff_xoffset,tca9548_standoff_yoffset,-1])   cylinder(r=standoff_hole_radius, h=20, center=true, $fn=20);
    translate([tca9548_standoff_xoffset,-tca9548_standoff_yoffset,-1])  cylinder(r=  standoff_hole_radius, h=20, center=true, $fn=20);

    // Enclosure mounting holes
    translate([mount_x_offset,mount_y_offset, BELOW_PLANE])   cylinder(r=mount_hole_radius, h=INF, center=true);
    translate([mount_x_offset,-mount_y_offset, BELOW_PLANE])  cylinder(r=mount_hole_radius, h=INF, center=true);
    translate([-mount_x_offset,mount_y_offset, BELOW_PLANE])  cylinder(r=mount_hole_radius, h=INF, center=true);
    translate([-mount_x_offset,-mount_y_offset, BELOW_PLANE]) cylinder(r=mount_hole_radius, h=INF, center=true);
}
