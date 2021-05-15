//Yann Amado Nunes Costa nUSP: 10746943

/*
 * Responsible for line related functions and data structures like reading from csv,
 * reading from binary and writing in binary
 */


#ifndef T1_LINE_H
#define T1_LINE_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "helpers.h"

typedef struct line_header {
  char status;
  long next_reg_byte;
  int count;
  int count_removed;
  char code_description[16];
  char card_description[14];
  char name_description[14];
  char color_description[26];
} line_header_t;

typedef struct line {
  int line_code;
  char name[100];
  char color[100];
  char accept_card[2];
  char removed;
  int size;
  int size_name;
  int size_color;
} line_t;

void write_line_header(FILE *file, line_header_t line_header);

void write_line(FILE *file, line_t line);

line_header_t read_line_header(FILE *file);

line_t read_line(FILE *file, int offset);

line_header_t read_line_header_from_csv(char line[]);

line_t read_line_from_csv(char line[]);

void update_line_header(line_header_t *header, line_t *line);

void print_line(line_t line);

#endif //T1_LINE_H
