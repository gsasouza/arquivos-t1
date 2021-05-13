//
// Created by Gabriel Souza on 06/05/21.
//

#include "vehicle.h"

vehicle_t create_vehicle(char line[]) {
  vehicle_t new_vehicle;
  strcpy(new_vehicle.prefix, strsep(&line, ","));
  strcpy(new_vehicle.date, strsep(&line, ","));
  new_vehicle.seats = (int) strtod(strsep(&line, ","), NULL);
  new_vehicle.line_code = (int) strtod(strsep(&line, ","), NULL);
  strcpy(new_vehicle.model, strsep(&line, ","));
  strcpy(new_vehicle.category, strsep(&line, ","));
  new_vehicle.category[strlen(new_vehicle.category) - 1] = '\0';
  new_vehicle.size_model = (int) strlen(new_vehicle.model);
  new_vehicle.size_category = (int) strlen(new_vehicle.category);
  if (new_vehicle.prefix[0] == '*') {
    new_vehicle.removed = '1';
    memmove(new_vehicle.prefix, new_vehicle.prefix + 1, strlen(new_vehicle.prefix));
  } else new_vehicle.removed = '0';
  new_vehicle.size = 31 + new_vehicle.size_model + new_vehicle.size_category;
  return new_vehicle;
}

vehicle_header_t create_vehicle_header(char line[]) {
  vehicle_header_t new_header;
  strcpy(new_header.prefix_description, strsep(&line, ","));
  strcpy(new_header.date_description, strsep(&line, ","));
  strcpy(new_header.seats_description, strsep(&line, ","));
  strcpy(new_header.line_description, strsep(&line, ","));
  strcpy(new_header.model_description, strsep(&line, ","));
  strcpy(new_header.category_description, strsep(&line, ","));
  new_header.category_description[strlen(new_header.category_description) - 1] = '\0';
  new_header.status = '0';
  new_header.count = 0;
  new_header.count_removed = 0;
  new_header.next_reg_byte = 0;
  return new_header;
}

void read_vehicles_csv(vehicle_file_t *vehicle_file, char filename[]) {
  char line[200];
  FILE *file = open_file(filename, "r");
  fgets(line, 200, file); // read the header
  vehicle_file->vehicle_header = create_vehicle_header(line);

  while (fgets(line, 200, file) != NULL) {
    vehicle_t new_vehicle = create_vehicle(line);
    vehicle_file->data[vehicle_file->vehicle_header.count + vehicle_file->vehicle_header.count_removed] = new_vehicle;
    vehicle_file->vehicle_header.next_reg_byte =
      vehicle_file->vehicle_header.next_reg_byte + new_vehicle.size;
    if (new_vehicle.removed == '0')
      vehicle_file->vehicle_header.count_removed = vehicle_file->vehicle_header.count_removed + 1;
    else vehicle_file->vehicle_header.count = vehicle_file->vehicle_header.count + 1;

  }
  fclose(file);
}

void print_vehicle(vehicle_t vehicle) {
  printf("Prefixo do veiculo: %s\n", format_print_null(vehicle.prefix));
  printf("Modelo do veiculo: %s\n", format_print_null(vehicle.model));
  printf("Categoria do veiculo: %s\n", format_print_null(vehicle.category));
  printf("Data de entrada do veiculo na frota: %s\n", format_print_date(vehicle.date));
  printf("Quantidade de lugares sentados disponiveis: %s\n\n", format_print_null_int(vehicle.seats));
}


void write_vehicle(FILE *file, vehicle_t vehicle) {
  fwrite(&vehicle.removed, 1, 1, file);
  fwrite(&vehicle.size, 4, 1, file);
  fwrite(add_empty_padding(vehicle.prefix, 5), 5, 1, file);
  fwrite(add_empty_padding(vehicle.date, 10), 10, 1, file);
  fwrite(&vehicle.seats, 4, 1, file);
  fwrite(&vehicle.line_code, 4, 1, file);
  fwrite(&vehicle.size_model, 4, 1, file);
  fwrite(&vehicle.model, 1, vehicle.size_model, file);
  fwrite(&vehicle.size_category, 4, 1, file);
  fwrite(&vehicle.category, 1, vehicle.size_category, file);
}


void add_end_to_vehicle_fields(vehicle_t *vehicle) {
  vehicle->model[vehicle->size_model] = '\0';
  vehicle->category[vehicle->size_category] = '\0';
  if (vehicle->date[0] == '@') vehicle->date[0] = '\0';
}

vehicle_t read_vehicle(FILE *file, int offset) {
  vehicle_t *new_vehicle = malloc(sizeof(vehicle_t));
  fread(&new_vehicle->removed, 1, 1, file);
  fread(&new_vehicle->size, 4, 1, file);
  fread(&new_vehicle->prefix, 5, 1, file);
  fread(&new_vehicle->date, 10, 1, file);
  fread(&new_vehicle->seats, 4, 1, file);
  fread(&new_vehicle->line_code, 4, 1, file);
  fread(&new_vehicle->size_model, 4, 1, file);
  fread(&new_vehicle->model, 1, new_vehicle->size_model, file);
  fread(&new_vehicle->size_category, 4, 1, file);
  fread(&new_vehicle->category, 1, new_vehicle->size_category, file);
  add_end_to_vehicle_fields(new_vehicle);
  return *new_vehicle;
}

vehicle_header_t read_vehicle_header(FILE *file) {
  vehicle_header_t *new_header = malloc(sizeof(vehicle_header_t));
  fread(&new_header->status, 1, 1, file);
  fread(&new_header->next_reg_byte, 8, 1, file);
  fread(&new_header->count, 4, 1, file);
  fread(&new_header->count_removed, 4, 1, file);
  fread(&new_header->prefix_description, 18, 1, file);
  fread(&new_header->date_description, 35, 1, file);
  fread(&new_header->seats_description, 42, 1, file);
  fread(&new_header->line_description, 26, 1, file);
  fread(&new_header->model_description, 17, 1, file);
  fread(&new_header->category_description, 20, 1, file);
  return *new_header;
}


void write_vehicle_header(FILE *file, vehicle_header_t vehicle_header) {
  fwrite(&vehicle_header.status, 1, 1, file);
  fwrite(&vehicle_header.next_reg_byte, 8, 1, file);
  fwrite(&vehicle_header.count, 4, 1, file);
  fwrite(&vehicle_header.count_removed, 4, 1, file);
  fwrite(&vehicle_header.prefix_description, 18, 1, file);
  fwrite(&vehicle_header.date_description, 35, 1, file);
  fwrite(&vehicle_header.seats_description, 42, 1, file);
  fwrite(&vehicle_header.line_description, 26, 1, file);
  fwrite(&vehicle_header.model_description, 17, 1, file);
  fwrite(&vehicle_header.category_description, 20, 1, file);
}