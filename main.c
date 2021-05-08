#include "vehicle.h"
#include "helpers.h"


int main() {
  vehicle_file_t vehicle_file, bin_file;
  read_vehicles_csv(&vehicle_file);
  FILE *vehicles_bin = open_file("vehicles.bin", "w+");
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
  return 0;
}
