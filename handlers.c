//
// Created by Gabriel Souza on 08/05/21.
//

#include "handlers.h"

void create_bin_from_csv(char filename_csv[], char filename_bin[]) {
  vehicle_file_t csv_file;
  read_vehicles_csv(&csv_file, filename_csv);
  FILE *bin_file = open_file(filename_bin, "w+");
  write_vehicle_header(bin_file, csv_file.vehicle_header);
  for (int i = 0; i < csv_file.vehicle_header.count; i++) {
    write_vehicle(bin_file, csv_file.data[i]);
  }
  fclose(bin_file);
}

void parse_input() {
  int option;
  char filename_in[30], filename_out[30];
  scanf("%d %s %s", &option, filename_in, filename_out);
  switch (option) {
    case 1:
      create_bin_from_csv(filename_in, filename_out);
      binarioNaTela(filename_out);
      return ;
    default:
      printf("Falha no processamentodo arquivo.");
      return ;
  }
}