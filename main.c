#include "vehicle.h"
#include "line.h"
#include "helpers.h"
#include "handlers.h"

void full_test_vehicle() {
  vehicle_file_t vehicle_file, bin_file;
  read_vehicles_csv(&vehicle_file, VEHICLE_FILENAME_CSV);
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
  read_lines_csv(&line_file, LINE_FILENAME_CSV);
  FILE *lines_bin = open_file(LINE_FILENAME_BIN, "w+");
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
//  full_test_line();
//  full_test_vehicle();
  parse_input();
//  vehicle_file_t f1;
//  read_vehicles_csv(&f1, "veiculo1.csv");
//  vehicle_file_t bin_file, bin2;
//  FILE *vehicles_bin = open_file("veiculo1.bin", "rb");
//  FILE *bin3 = open_file("veiculo1_saida_esperada.bin", "rb");
//  bin_file.vehicle_header = read_vehicle_header(vehicles_bin);
//  bin2.vehicle_header = read_vehicle_header(bin3);
//
//  int c1 = 0, c2 = 0, c3 = 0;
//  for (int i = 0; i < bin_file.vehicle_header.count + bin_file.vehicle_header.count_removed; i++) {
//    bin_file.data[i] = read_vehicle(vehicles_bin, 0);
//    bin2.data[i] = read_vehicle(bin3, 0);
//    c1 += bin_file.data[i].size;
//    c2 += bin2.data[i].size;
//    c3 += f1.data[i].size;
//    printf("%d %d %d", c1, c2, c3);
////    if (bin_file.data[i].removed != '0') print_vehicle(bin_file.data[i]);
//  }
//
//  fclose(bin3);
//  fclose(vehicles_bin);
  return 0;
}