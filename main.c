#include "vehicle.h"
#include "line.h"
#include "helpers.h"
#include "handlers.h"

//void full_test_vehicle() {
//  vehicle_file_t vehicle_file, bin_file;
//  read_vehicles_csv(&vehicle_file, VEHICLE_FILENAME_CSV);
//  FILE *vehicles_bin = open_file(VEHICLE_FILENAME_BIN, "w+");
//  write_vehicle_header(vehicles_bin, vehicle_file.vehicle_header);
//  for (int i = 0; i < vehicle_file.vehicle_header.count; i++) {
//    write_vehicle(vehicles_bin, vehicle_file.data[i]);
//  }
//  fseek(vehicles_bin, 0, SEEK_SET);
//  bin_file.vehicle_header = read_vehicle_header(vehicles_bin);
//  for (int i = 0; i < bin_file.vehicle_header.count; i++) {
//    bin_file.data[i] = read_vehicle(vehicles_bin, 0);
//    print_vehicle(bin_file.data[i]);
//  }
//}

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

bool compare_byte_str(char str[], char str2[], int size) {
  for (int i = 0; i < size; i++) {
    if (str[i] != str2[i]) return false;
  }
  return true;
}


int main() {
//  full_test_line();
//  full_test_vehicle();
  parse_input();
  vehicle_file_t binm, binmr;
  FILE *bin = open_file("veiculo1.bin", "rb");
  FILE *binr = open_file("veiculo1_saida_esperada.bin", "rb");

  binm.vehicle_header = read_vehicle_header(bin);
  binmr.vehicle_header = read_vehicle_header(binr);

  if (!compare_byte_str(binm.vehicle_header.prefix_description, binmr.vehicle_header.prefix_description, 19)) {
    printf("diff_prefix\n");
  }

  if (!compare_byte_str(binm.vehicle_header.category_description, binmr.vehicle_header.category_description, 21)) {
    printf("diff_category\n");
  }

  if (!compare_byte_str(binm.vehicle_header.model_description, binmr.vehicle_header.model_description, 18)) {
    printf("diff_model\n");
  }

  if (!compare_byte_str(binm.vehicle_header.seats_description, binmr.vehicle_header.seats_description, 43)) {
    printf("diff_seats\n");
  }
  if (!compare_byte_str(binm.vehicle_header.date_description, binmr.vehicle_header.date_description, 36)) {
    printf("diff_date\n");
  }
  if (!compare_byte_str(binm.vehicle_header.line_description, binmr.vehicle_header.line_description, 27)) {
    printf("diff_line\n");
  }
  int c2 = 175, c3 = 175;
  for (int i = 0; i < binm.vehicle_header.count + binm.vehicle_header.count_removed; i++) {
    binm.data[i] = read_vehicle(bin, 0);
    binmr.data[i] = read_vehicle(binr, 0);
    c2 += binm.data[i].size;
    c3 += binmr.data[i].size;
    if (binm.data[i].seats != binmr.data[i].seats) { // OK
      printf("diff_seats\n");
    }
    if (binm.data[i].line_code != binmr.data[i].line_code) { //OK
      printf("diff_line_code\n");
    }
    if (!compare_byte_str(binm.data[i].model, binmr.data[i].model, 100) ) { // OK
      printf("diff_model\n");
    }
    if (!compare_byte_str(binm.data[i].category, binmr.data[i].category, 100)) { // OK
      printf("diff_category\n");
    }

    if (!compare_byte_str(binm.data[i].date, binmr.data[i].date, 11)) { //OK
      printf("diff_date\n");
    }
    if (!compare_byte_str(binm.data[i].prefix, binmr.data[i].prefix,6)) { //OK
      printf("diff_prefix\n");
    }
    if (c3 != c2) { // OK
      printf("diff_size\n");
    }
  }

  fclose(bin);
  fclose(binr);
  return 0;
}
