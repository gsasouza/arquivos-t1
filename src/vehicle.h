/*
 * Gabriel Santos Souza nUSP: 11208176
 * Yann Amado Nunes Costa nUSP: 10746943
 */
/*
 * Responsible for vehicle related functions and data structures like reading from csv,
 * reading from binary and writing in binary
 */

#ifndef T1_VEHICLE_H
#define T1_VEHICLE_H

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "helpers.h"
#include "line.h"

typedef struct vehicle {
  char prefix[6];
  char date[11];
  int seats;
  int line_code;
  char model[100];
  char category[100];
  char removed;
  int size;
  int size_model;
  int size_category;
} vehicle_t;

typedef struct vehicle_header {
  char status;
  long next_reg_byte;
  int count;
  int count_removed;
  char prefix_description[19];
  char date_description[36];
  char seats_description[43];
  char line_description[27];
  char model_description[18];
  char category_description[21];
} vehicle_header_t;

void print_vehicle(vehicle_t vehicle);

void write_vehicle(FILE *file, vehicle_t vehicle);

void write_vehicle_header(FILE *file, vehicle_header_t vehicle_header);

vehicle_header_t read_vehicle_header(FILE *file);

vehicle_t read_vehicle(FILE *file, long offset);
vehicle_t *read_vehicle_p(FILE *file, long offset);

vehicle_header_t read_vehicle_header_from_csv(char line[]);

vehicle_t read_vehicle_from_csv(char line[]);

void update_header(vehicle_header_t *header, vehicle_t *vehicle);

vehicle_t create_vehicle();

int verify_vehicle_header_status(vehicle_header_t header);

#endif //T1_VEHICLE_H
