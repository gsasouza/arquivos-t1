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
    new_vehicle.removed = true;
    memmove(new_vehicle.prefix, new_vehicle.prefix + 1, strlen(new_vehicle.prefix));
  } else new_vehicle.removed = false;
  new_vehicle.size = 23 + new_vehicle.size_model + new_vehicle.size_category;
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
  new_header.status = false;
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
    vehicle_file->data[vehicle_file->vehicle_header.count] = new_vehicle;
    vehicle_file->vehicle_header.next_reg_byte =
      vehicle_file->vehicle_header.next_reg_byte + new_vehicle.size;
    vehicle_file->vehicle_header.count = vehicle_file->vehicle_header.count + 1;
    if (new_vehicle.removed)
      vehicle_file->vehicle_header.count_removed = vehicle_file->vehicle_header.count_removed + 1;
  }
  fclose(file);
}

void print_vehicle(vehicle_t vehicle) {
  printf("%d %d %s %s %d %d %d %s %d %s\n\n", vehicle.removed, vehicle.size, vehicle.prefix, vehicle.date,
         vehicle.seats, vehicle.line_code,
         vehicle.size_model, vehicle.model, vehicle.size_category, vehicle.category);
}


void write_vehicle(FILE *file, vehicle_t vehicle) {
  fwrite(&vehicle.removed, 1, 1, file);
  fwrite(&vehicle.size, sizeof(int), 1, file);
  fwrite(add_empty_padding(vehicle.prefix, 5), sizeof(char), 5, file);
  fwrite(add_empty_padding(vehicle.date, 10), sizeof(char), 10, file);
  fwrite(&vehicle.seats, sizeof(int), 1, file);
  fwrite(&vehicle.line_code, sizeof(int), 1, file);
  fwrite(&vehicle.size_model, sizeof(int), 1, file);
  fwrite(&vehicle.model, sizeof(char), strlen(vehicle.model), file);
  fwrite(&vehicle.size_category, sizeof(int), 1, file);
  fwrite(&vehicle.category, sizeof(char), strlen(vehicle.category), file);
}

vehicle_t read_vehicle(FILE *file, int offset) {
  vehicle_t new_vehicle;
  fread(&new_vehicle.removed, 1, offset + 1, file);
  fread(&new_vehicle.size, sizeof(int), offset + 1, file);
  fread(&new_vehicle.prefix, sizeof(char), offset + 5, file);
  fread(&new_vehicle.date, sizeof(char), offset + 10, file);
  fread(&new_vehicle.seats, sizeof(int), offset + 1, file);
  fread(&new_vehicle.line_code, sizeof(int), offset + 1, file);
  fread(&new_vehicle.size_model, sizeof(int), offset + 1, file);
  fread(&new_vehicle.model, sizeof(char), offset + new_vehicle.size_model, file);
  fread(&new_vehicle.size_category, sizeof(int), offset + 1, file);
  fread(&new_vehicle.category, sizeof(char), offset + new_vehicle.size_category, file);
  return new_vehicle;
}

vehicle_header_t read_vehicle_header(FILE *file) {
  vehicle_header_t new_header;
  fread(&new_header.status, sizeof(char), 1, file);
  fread(&new_header.next_reg_byte, sizeof(int), 2, file);
  fread(&new_header.count, sizeof(int), 1, file);
  fread(&new_header.count_removed, sizeof(int), 1, file);
  fread(&new_header.prefix_description, sizeof(char), 18, file);
  fread(&new_header.date_description, sizeof(char), 35, file);
  fread(&new_header.seats_description, sizeof(char), 42, file);
  fread(&new_header.model_description, sizeof(char), 17, file);
  fread(&new_header.category_description, sizeof(char), 20, file);
  return new_header;
}


void write_vehicle_header(FILE *file, vehicle_header_t vehicle_header) {
  fwrite(&vehicle_header.status, sizeof(char), 1, file);
  fwrite(&vehicle_header.next_reg_byte, sizeof(int), 2, file);
  fwrite(&vehicle_header.count, sizeof(int), 1, file);
  fwrite(&vehicle_header.count_removed, sizeof(int), 1, file);
  fwrite(&vehicle_header.prefix_description, sizeof(char), 18, file);
  fwrite(&vehicle_header.date_description, sizeof(char), 35, file);
  fwrite(&vehicle_header.seats_description, sizeof(char), 42, file);
  fwrite(&vehicle_header.model_description, sizeof(char), 17, file);
  fwrite(&vehicle_header.category_description, sizeof(char), 20, file);
}