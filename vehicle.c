//
// Created by Gabriel Souza on 06/05/21.
//

#include "vehicle.h"

vehicle_t create_vehicle(char line[]) {
  vehicle_t new_vehicle;
  strcpy(new_vehicle.prefix, strsep(&line, ","));
  strcpy(new_vehicle.date, strsep(&line, ","));
  new_vehicle.seats = (int) strtod(strsep(&line, ","), NULL);
  new_vehicle.lineCode = (int) strtod(strsep(&line, ","), NULL);
  strcpy(new_vehicle.model, strsep(&line, ","));
  strcpy(new_vehicle.category, strsep(&line, ","));
  new_vehicle.sizeModel = (int) strlen(new_vehicle.model);
  new_vehicle.sizeCategory = (int) strlen(new_vehicle.category);
  if (new_vehicle.prefix[0] == '*') {
    new_vehicle.removed = true;
    memmove(new_vehicle.prefix, new_vehicle.prefix + 1, strlen(new_vehicle.prefix));
  } else new_vehicle.removed = false;
  new_vehicle.size = 23 + new_vehicle.sizeModel + new_vehicle.sizeCategory;
  return new_vehicle;
}

int read_vehicles_csv(vehicle_t *vehicles) {
  int count = 0;
  char line[200];
  FILE *file = fopen("veiculo.csv", "r");
  if (!file) {
    printf("Error opening file");
    exit(1);
  }
  fgets(line, 200, file); // read the header

  while (fgets(line, 200, file) != NULL) {
    vehicles[count] = create_vehicle(line);
    count++;
  }
  fclose(file);
  return count;
}

void print_vehicle(vehicle_t vehicle) {
  printf("%s %s %d %d %s %s %d %d %d %d\n\n", vehicle.prefix, vehicle.date, vehicle.seats, vehicle.lineCode,
         vehicle.model, vehicle.category, vehicle.sizeModel, vehicle.sizeCategory, vehicle.size, vehicle.removed);
}



void write_vehicle(FILE *file, vehicle_t vehicle) {
  fwrite(&vehicle.removed, 1, 1, file);
  fwrite(&vehicle.size, sizeof(int), 1, file);
  fwrite(add_empty_padding(vehicle.prefix, 5), sizeof(char), 5, file);
  fwrite(add_empty_padding(vehicle.date, 10), sizeof(char), 10, file);
  fwrite(&vehicle.seats,  sizeof(int), 1, file);
  fwrite(&vehicle.lineCode,  sizeof(int), 1, file);
  fwrite(&vehicle.sizeModel,  sizeof(int), 1, file);
  fwrite(&vehicle.model, sizeof(char),  strlen(vehicle.model),   file);
  fwrite(&vehicle.sizeCategory,  sizeof(int), 1, file);
  fwrite(&vehicle.category, sizeof(char), strlen(vehicle.category), file);
}

vehicle_t read_vehicle(FILE *file, int rnn) {
  vehicle_t new_vehicle;
  fread(&new_vehicle.removed, 1, 1, file);
  fread(&new_vehicle.size, sizeof(int), 1, file);
  fread(&new_vehicle.prefix, sizeof(char), 5, file);
  fread(&new_vehicle.date, sizeof(char), 10, file);
  fread(&new_vehicle.seats, sizeof(int), 1, file);
  fread(&new_vehicle.lineCode, sizeof(int), 1, file);
  fread(&new_vehicle.sizeModel, sizeof(int), 1, file);
  fread(&new_vehicle.model, sizeof(char), new_vehicle.sizeModel, file);
  fread(&new_vehicle.sizeCategory, sizeof(int), 1, file);
  fread(&new_vehicle.category, sizeof(char), new_vehicle.sizeCategory, file);
  return new_vehicle;
}