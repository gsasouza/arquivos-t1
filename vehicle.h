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

typedef struct vehicle {
  char prefix[6];
  char date[11];
  int seats;
  int lineCode;
  char model[100];
  char category[100];
  bool removed;
  int size;
  int sizeModel;
  int sizeCategory;
} vehicle_t;

int read_vehicles_csv (vehicle_t *vehicles);
void print_vehicle(vehicle_t vehicle);
void write_vehicle(FILE *file, vehicle_t vehicle);
vehicle_t read_vehicle(FILE *file, int rnn);
#endif //T1_VEHICLE_H
