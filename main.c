#include "vehicle.h"
#include "line.h"
#include "helpers.h"

void full_test_vehicle() {
  vehicle_file_t vehicle_file, bin_file;
  read_vehicles_csv(&vehicle_file);
  FILE *vehicles_bin = open_file(VEHICLE_FILENAME_BIN, "w+");
  write_vehicle_header(vehicles_bin, vehicle_file.vehicle_header);
  for (int i = 0; i < vehicle_file.vehicle_header.count; i++) {
    write_vehicle(vehicles_bin, vehicle_file.data[i]);
  }
  fseek(vehicles_bin, 0, SEEK_SET);
  bin_file.vehicle_header = read_vehicle_header(vehicles_bin);
  for (int i = 0; i < bin_file.vehicle_header.count; i++) {
    bin_file.data[i] = read_vehicle(vehicles_bin, 0);
    print_vehicle(bin_file.data[i]);
  }
}

void full_test_line() {
  line_file_t line_file, bin_file;
  read_lines_csv(&line_file);
  FILE *lines_bin = open_file(VEHICLE_FILENAME_BIN, "w+");
  write_line_header(lines_bin, line_file.line_header);
  for (int i = 0; i < line_file.line_header.count; i++) {
    write_line(lines_bin, line_file.data[i]);
  }
  fseek(lines_bin, 0, SEEK_SET);
  bin_file.line_header = read_line_header(lines_bin);
  for (int i = 0; i < bin_file.line_header.count; i++) {
    bin_file.data[i] = read_line(lines_bin, 0);
    print_line(bin_file.data[i]);
  }
}


int main() {
  full_test_line();
  full_test_vehicle();

  return 0;
}
