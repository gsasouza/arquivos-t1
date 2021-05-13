//
// Created by Gabriel Souza on 07/05/21.
//

#include "line.h"

char *format_accept_card(char accepted_card) {
  if (accepted_card == 'S') return "PAGAMENTO SOMENTE COM CARTAO SEM PRESENCA DE COBRADOR";
  if (accepted_card == 'N') return "PAGAMENTO EM CARTAO E DINHEIRO";
  return "PAGAMENTO EM CARTAO SOMENTE NO FINAL DE SEMANA";
}

void print_line(line_t line) {
  printf("Codigo da linha: %s\n", format_print_null_int(line.line_code));
  printf("Nome da linha: %s\n", format_print_null(line.name));
  printf("Cor que descreve a linha: %s\n", format_print_null(line.color));
  printf("Aceita cartao: %s\n\n", format_accept_card(line.accept_card[0]));
}

line_header_t create_line_header(char line[]) {
  line_header_t new_header;
  strcpy(new_header.code_description, strsep(&line, ","));
  strcpy(new_header.card_description, strsep(&line, ","));
  strcpy(new_header.name_description, strsep(&line, ","));
  strcpy(new_header.color_description, strsep(&line, ","));
  new_header.color_description[strlen(new_header.color_description) - 1] = '\0';
  new_header.status = false;
  new_header.count = 0;
  new_header.count_removed = 0;
  new_header.next_reg_byte = 0;
  return new_header;
}

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

line_t create_line(char line[]) {
  line_t new_line;
  char *line_code_str = strsep(&line, ",");
  new_line.line_code = (int) strtod(line_code_str, NULL);
  strcpy(new_line.accept_card, strsep(&line, ","));
  strcpy(new_line.name, strsep(&line, ","));
  strcpy(new_line.color, strsep(&line, ","));
  new_line.color[strlen(new_line.color) - 1] = '\0';
  new_line.size_name = (int) strlen(new_line.name);
  new_line.size_color = (int) strlen(new_line.color);
  if (line_code_str[0] == '*') {
    new_line.removed = true;
  } else new_line.removed = false;
  new_line.size = 18 + new_line.size_color + new_line.size_name;
  return new_line;
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

void add_end_to_fields(line_t *line) {
  line->name[line->size_name] = '\0';
  line->color[line->size_color] = '\0';
}

line_t read_line(FILE *file, int offset) {
  line_t *new_line = malloc(sizeof(line_t));
  fread(&new_line->removed, 1, 1, file);
  fread(&new_line->size, 4, 1, file);
  fread(&new_line->line_code, 4, 1, file);
  fread(&new_line->accept_card, 1, 1, file);
  fread(&new_line->size_name, 4, 1, file);
  fread(&new_line->name, 1, new_line->size_name, file);
  fread(&new_line->size_color, 4, 1, file);
  fread(&new_line->color, 1, new_line->size_color, file);
  add_end_to_fields(new_line);
  return *new_line;
}


void write_line(FILE *file, line_t line) {
  fwrite(&line.removed, sizeof(char), 1, file);
  fwrite(&line.size, sizeof(int), 1, file);
  fwrite(&line.line_code, sizeof(int), 1, file);
  fwrite(&line.accept_card, sizeof(char), 1, file);
  fwrite(&line.size_name, sizeof(int), 1, file);
  fwrite(&line.name, sizeof(char), line.size_name, file);
  fwrite(&line.size_color, sizeof(int), 1, file);
  fwrite(&line.color, sizeof(char), line.size_name, file);
}

void read_lines_csv(line_file_t *line_file, char filename[]) {
  char line[200];
  FILE *file = open_file(filename, "r");
  fgets(line, 200, file); // read the header
  line_file->line_header = create_line_header(line);

  while (fgets(line, 200, file) != NULL) {
    line_t new_line = create_line(line);
    line_file->data[line_file->line_header.count] = new_line;
    line_file->line_header.next_reg_byte =
      line_file->line_header.next_reg_byte + new_line.size;

    if (new_line.removed)
      line_file->line_header.count_removed = line_file->line_header.count_removed + 1;
    else
      line_file->line_header.count = line_file->line_header.count + 1;
  }
  fclose(file);
}
