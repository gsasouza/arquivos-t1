//
// Created by Gabriel Souza on 06/05/21.
//

#ifndef T1_VEHICLE_H
#define T1_VEHICLE_H

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "helpers.h"

#define VEHICLE_FILENAME_BIN "vehicles.bin"
#define VEHICLE_FILENAME_CSV "veiculo.csv"

typedef struct vehicle {
  char prefix[6];
  char date[11];
  int seats;
  int line_code;
  char model[100];
  char category[100];
  bool removed;
  int size;
  int size_model;
  int size_category;
} vehicle_t;

typedef struct vehicle_header {
  bool status;
  int next_reg_byte;
  int count;
  int count_removed;
  char prefix_description[19];
  char date_description[36];
  char seats_description[43];
  char line_description[27];
  char model_description[18];
  char category_description[22];
} vehicle_header_t;

typedef struct vehicle_file {
  vehicle_header_t vehicle_header;
  vehicle_t data[1000];
} vehicle_file_t;

void read_vehicles_csv(vehicle_file_t *vehicle_file, char filename[]);

void print_vehicle(vehicle_t vehicle);

void write_vehicle(FILE *file, vehicle_t vehicle);

void write_vehicle_header(FILE *file, vehicle_header_t vehicle_header);

vehicle_header_t read_vehicle_header(FILE *file);

vehicle_t read_vehicle(FILE *file, int offset);

vehicle_header_t create_vehicle_header(char line[]);

#endif //T1_VEHICLE_H
