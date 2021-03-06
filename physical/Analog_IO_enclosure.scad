// Case enclosure for:
//   Sparkfun SX1509 I/O Expander board
//   Two Adafruit ADS1015 Analog Input boards
//

// Number of facets for cylinders
$fn = 20;

enclosure_x_dim = 120;
enclosure_y_dim = 50;
enclosure_height = 20;

sx1509_standoff_x_offset = 15.4;
sx1509_standoff_y_offset = 10.4;
standoff_hole_radius = 1;

ads1015_standoff_y_offset = 11.5;

mount_x_offset = 21;
mount_y_offset = 16;
mount_hole_radius = 1.6;

base_thickness = 3;
wall_width = 2;
wall_height = enclosure_height;

BELOW_PLANE = -1;
INF = 100000;

io_shift = 25;
io_width = 25;

difference() {
    // outer extents
    cube([enclosure_x_dim ,enclosure_y_dim ,enclosure_height], center = true);

    // cutout inside to form walls
    translate([0,   0, base_thickness])cube([enclosure_x_dim -(2*wall_width),enclosure_y_dim -(2*wall_width), wall_height], center = true);

    translate([io_shift,0,0]){

        // I/O side cuts
        translate([1.5, 0, base_thickness])cube([30,INF,wall_height], center = true);
        translate([0,   2, base_thickness])cube([INF,15,wall_height], center = true);
         
        // SX1509 mounting holes
        translate([sx1509_standoff_x_offset,sx1509_standoff_y_offset, BELOW_PLANE])   cylinder(r=standoff_hole_radius, h=INF, center=true);
        translate([sx1509_standoff_x_offset,-sx1509_standoff_y_offset, BELOW_PLANE])  cylinder(r=standoff_hole_radius, h=INF, center=true);
        translate([-sx1509_standoff_x_offset,sx1509_standoff_y_offset, BELOW_PLANE])  cylinder(r=standoff_hole_radius, h=INF, center=true);
        translate([-sx1509_standoff_x_offset,-sx1509_standoff_y_offset, BELOW_PLANE]) cylinder(r=standoff_hole_radius, h=INF, center=true);

        // ADS1015 #1 mounting holes
        translate([sx1509_standoff_x_offset-(io_shift+2*io_width),ads1015_standoff_y_offset, BELOW_PLANE])  cylinder(r=standoff_hole_radius, h=INF, center=true);
        translate([sx1509_standoff_x_offset-(io_shift+2*io_width),-ads1015_standoff_y_offset, BELOW_PLANE]) cylinder(r=standoff_hole_radius, h=INF, center=true);

        // ADS1015 #2 mounting holes
        translate([sx1509_standoff_x_offset-(io_shift+io_width),ads1015_standoff_y_offset, BELOW_PLANE])  cylinder(r=standoff_hole_radius, h=INF, center=true);
        translate([sx1509_standoff_x_offset-(io_shift+io_width),-ads1015_standoff_y_offset, BELOW_PLANE]) cylinder(r=standoff_hole_radius, h=INF, center=true);
            
        // Enclosure mounting holes
        translate([mount_x_offset,mount_y_offset, BELOW_PLANE])   cylinder(r=mount_hole_radius, h=INF, center=true);
        translate([mount_x_offset,-mount_y_offset, BELOW_PLANE])  cylinder(r=mount_hole_radius, h=INF, center=true);
        translate([-mount_x_offset-(2*io_width),mount_y_offset, BELOW_PLANE])  cylinder(r=mount_hole_radius, h=INF, center=true);
        translate([-mount_x_offset-(2*io_width),-mount_y_offset, BELOW_PLANE]) cylinder(r=mount_hole_radius, h=INF, center=true);
    }
}
