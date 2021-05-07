#include "vehicle.h"
#include "helpers.h"


int main() {
  vehicle_t vehicles[1000];
  int count = read_vehicles_csv(vehicles);

  FILE *vehicles_file = open_file("vehicles.bin", "w+");
  for (int i = 0; i < count; i++) {
    print_vehicle(vehicles[i]);
    write_vehicle(vehicles_file, vehicles[i]);
  }
  fseek(vehicles_file, 0, SEEK_SET);
  for (int i = 0; i < count; i++) {
    vehicle_t new_vehicle = read_vehicle(vehicles_file, 0);
    print_vehicle(new_vehicle);
  }
  fclose(vehicles_file);
  return 0;
}
