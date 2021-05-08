//
// Created by Gabriel Souza on 07/05/21.
//

#include "line.h"


line_header_t read_line_header(FILE *file) {
  line_header_t new_header;
  fread(&new_header.status, sizeof(char), 1, file);
  fread(&new_header.next_reg_byte, sizeof(int), 2, file);
  fread(&new_header.count, sizeof(int), 1, file);
  fread(&new_header.count_removed, sizeof(int), 1, file);
  fread(&new_header.code_description, sizeof(char), 15, file);
  fread(&new_header.card_description, sizeof(char), 13, file);
  fread(&new_header.name_description, sizeof(char), 13, file);
  fread(&new_header.color_description, sizeof(char), 24, file);
  return new_header;
}


void write_line_header(FILE *file, line_header_t line_header) {
  fwrite(&line_header.status, sizeof(char), 1, file);
  fwrite(&line_header.next_reg_byte, sizeof(int), 2, file);
  fwrite(&line_header.count, sizeof(int), 1, file);
  fwrite(&line_header.count_removed, sizeof(int), 1, file);
  fwrite(&line_header.code_description, sizeof(char), 15, file);
  fwrite(&line_header.card_description, sizeof(char), 13, file);
  fwrite(&line_header.name_description, sizeof(char), 13, file);
  fwrite(&line_header.color_description, sizeof(char), 24, file);
}