//
// Created by Yann on 16/07/2021.
//

#ifndef T1_SORTING_H
#define T1_SORTING_H

#include "vehicle.h"
#include "line.h"

void mergesort_vehicle(vehicle_t* lines, int inicio, int fim, char ordering_field[]);

void mergesort_line(line_t* lines, int inicio, int fim, char ordering_field[]);

#endif //T1_SORTING_H

