//
// Created by Gabriel Souza on 08/05/21.
//

#include "handlers.h"

void create_table_vehicle(char filename_csv[], char filename_bin[]) {
  vehicle_file_t csv_file;
  read_vehicles_csv(&csv_file, filename_csv);
  FILE *bin_file = open_file(filename_bin, "w+");
  write_vehicle_header(bin_file, csv_file.vehicle_header);
  for (int i = 0; i < csv_file.vehicle_header.count; i++) {
    write_vehicle(bin_file, csv_file.data[i]);
  }
  fseek(bin_file, 0, SEEK_SET);
  csv_file.vehicle_header.status = 1;
  write_vehicle_header(bin_file, csv_file.vehicle_header);
  fclose(bin_file);
}

void create_table_line(char filename_csv[], char filename_bin[]) {
  line_file_t csv_file;
  read_lines_csv(&csv_file, filename_csv);
  FILE *bin_file = open_file(filename_bin, "wb");
  write_line_header(bin_file, csv_file.line_header);
  for (int i = 0; i < csv_file.line_header.count; i++) {
    write_line(bin_file, csv_file.data[i]);
  }
  fseek(bin_file, 0, SEEK_SET);
  csv_file.line_header.status = 1;
  write_line_header(bin_file, csv_file.line_header);
  fclose(bin_file);
}

void select_from_vehicles(char filename[]) {
  vehicle_file_t bin_file;
  FILE *vehicles_bin = open_file(filename, "rb");
  bin_file.vehicle_header = read_vehicle_header(vehicles_bin);
  for (int i = 0; i < bin_file.vehicle_header.count; i++) {
    bin_file.data[i] = read_vehicle(vehicles_bin, 0);
    print_vehicle(bin_file.data[i]);
  }
  fclose(vehicles_bin);
}

void select_from_lines(char filename[]) {
  line_file_t bin_file;
  FILE *lines_bin = open_file(filename, "rb");
  bin_file.line_header = read_line_header(lines_bin);
  for (int i = 0; i < bin_file.line_header.count; i++) {
    bin_file.data[i] = read_line(lines_bin, 0);
    print_line(bin_file.data[i]);
  }
  fclose(lines_bin);
}

void parse_input() {
  int option;
  char filename_in[30], filename_out[30];
  scanf("%d", &option);
  switch (option) {
    case 1:
      scanf("%s %s", filename_in, filename_out);
      create_table_vehicle(filename_in, filename_out);
      return binarioNaTela(filename_out);
    case 2:
      scanf("%s %s", filename_in, filename_out);
      create_table_line(filename_in, filename_out);
      return binarioNaTela(filename_out);
    case 3:
      scanf("%s", filename_in);
      select_from_vehicles(filename_in);
      return binarioNaTela(filename_out);
    case 4:
      scanf("%s", filename_in);
      select_from_lines(filename_in);
      return;
    default:
      printf("Falha no processamentodo arquivo.");
      return ;
  }
}