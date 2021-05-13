#include "handlers.h"

void create_table_vehicle(char filename_csv[], char filename_bin[]) {
  char buffer[200];
  FILE *bin_file = open_file(filename_bin, "w+");
  FILE *csv_file = open_file(filename_csv, "r");

  // Handle header
  fgets(buffer, 200, csv_file);
  vehicle_header_t header = read_vehicle_header_from_csv(buffer);
  write_vehicle_header(bin_file, header);

  // Handle data
  while (fgets(buffer, 200, csv_file) != NULL) {
    vehicle_t new_vehicle = read_vehicle_from_csv(buffer);
    write_vehicle(bin_file, new_vehicle);
    update_header(&header, &new_vehicle);
  }

  // Update header
  header.status = '1';
  header.next_reg_byte = ftell(bin_file);
  fseek(bin_file, 0, SEEK_SET);
  write_vehicle_header(bin_file, header);

  fclose(bin_file);
  fclose(csv_file);
}

void create_table_line(char filename_csv[], char filename_bin[]) {
  char buffer[200];
  FILE *bin_file = open_file(filename_bin, "w+");
  FILE *csv_file = open_file(filename_csv, "r");

  // Handle header
  fgets(buffer, 200, csv_file);
  line_header_t header = read_line_header_from_csv(buffer);
  write_line_header(bin_file, header);

  // Handle data
  while (fgets(buffer, 200, csv_file) != NULL) {
    line_t new_line = read_line_from_csv(buffer);
    write_line(bin_file, new_line);
    update_line_header(&header, &new_line);
  }

  // Update header
  header.status = '1';
  header.next_reg_byte = ftell(bin_file);
  fseek(bin_file, 0, SEEK_SET);
  write_line_header(bin_file, header);

  fclose(bin_file);
  fclose(csv_file);
}

void select_from_vehicles(char filename[]) {
  vehicle_header_t header;
  FILE *bin_file = open_file(filename, "rb");
  header = read_vehicle_header(bin_file);
  if (header.count == 0) {
    printf(EMPTY_MESSAGE);
    exit(0);
  }
  for (int i = 0; i < header.count + header.count_removed; i++) {
    vehicle_t vehicle = read_vehicle(bin_file, 0);
    if (vehicle.removed != '0') print_vehicle(vehicle);
  }
  fclose(bin_file);
}

void select_from_lines(char filename[]) {
  line_header_t header;
  FILE *bin_file = open_file(filename, "rb");
  header = read_line_header(bin_file);
  if (header.count == 0) {
    printf(EMPTY_MESSAGE);
    exit(0);
  }
  for (int i = 0; i < header.count + header.count_removed; i++) {
    line_t line = read_line(bin_file, 0);
    if (line.removed != '0') print_line(line);
  }
  fclose(bin_file);
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
      return;
    case 4:
      scanf("%s", filename_in);
      select_from_lines(filename_in);
      return;
    default:
      printf(ERROR_MESSAGE);
      return ;
  }
}