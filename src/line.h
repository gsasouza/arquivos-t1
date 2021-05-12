//
// Created by Gabriel Souza on 07/05/21.
//

#ifndef T1_LINE_H
#define T1_LINE_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "helpers.h"

#define LINE_FILENAME_CSV "linha.csv"
#define LINE_FILENAME_BIN "linha.bin"

typedef struct line_header {
  bool status;
  int next_reg_byte;
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
  bool removed;
  int size;
  int size_name;
  int size_color;
} line_t;

typedef struct line_file {
  line_header_t line_header;
  line_t data[1000];
} line_file_t;

void write_line_header(FILE *file, line_header_t line_header);

void write_line(FILE *file, line_t line);

line_header_t read_line_header(FILE *file);

line_t read_line(FILE *file, int offset);

void read_lines_csv(line_file_t *line_file, char filename[]);

void print_line(line_t line);

#endif //T1_LINE_H
