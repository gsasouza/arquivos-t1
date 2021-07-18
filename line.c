/*
 * Gabriel Santos Souza nUSP: 11208176
 * Yann Amado Nunes Costa nUSP: 10746943
 */

#include "line.h"

/*
 * Calculate line entry size
 */
int calculate_line_size(line_t *line) {
  return 13 + line->size_color + line->size_name;
}

/*
 * Format "Aceita Cartão" value
 */
char *format_accept_card(char accepted_card) {
  if (accepted_card == 'S') return "PAGAMENTO SOMENTE COM CARTAO SEM PRESENCA DE COBRADOR";
  if (accepted_card == 'N') return "PAGAMENTO EM CARTAO E DINHEIRO";
  return "PAGAMENTO EM CARTAO SOMENTE NO FINAL DE SEMANA";
}

/*
 * Print formatted line
 */
void print_line(line_t line) {
  printf("Codigo da linha: %s\n", format_print_null_int(line.line_code));
  printf("Nome da linha: %s\n", format_print_null(line.name));
  printf("Cor que descreve a linha: %s\n", format_print_null(line.color));
  switch(line.accept_card[0]){
      case 'S':
          printf("Aceita cartao: PAGAMENTO SOMENTE COM CARTAO SEM PRESENCA DE COBRADOR");
          break;
      case 'N':
          printf("Aceita cartao: PAGAMENTO EM CARTAO E DINHEIRO");
          break;
      case 'F':
          printf("Aceita cartao: PAGAMENTO EM CARTAO SOMENTE NO FINAL DE SEMANA");
          break;
  }
}

/*
 * Read line header from csv
 */
line_header_t read_line_header_from_csv(char line[]) {
  line_header_t new_header;
  strcpy(new_header.code_description, strsep(&line, ","));
  strcpy(new_header.card_description, strsep(&line, ","));
  strcpy(new_header.name_description, strsep(&line, ","));
  strcpy(new_header.color_description, format_csv_last_field(strsep(&line, ",")));
  new_header.status = '0';
  new_header.count = 0;
  new_header.count_removed = 0;
  new_header.next_reg_byte = 0;
  return new_header;
}

/*
 * Update header after a new entry added
 */
void update_line_header(line_header_t *header, line_t *line) {
  if (line->removed == '1') header->count_removed = header->count_removed + 1;
  else header->count = header->count + 1;
}

/*
 * Read header from bin file
 */
line_header_t read_line_header(FILE *file) {
  line_header_t new_header;
  fread(&new_header.status, 1, 1, file);
  fread(&new_header.next_reg_byte, 8, 1, file);
  fread(&new_header.count, 4, 1, file);
  fread(&new_header.count_removed, 4, 1, file);
  fread(&new_header.code_description, 15, 1, file);
  fread(&new_header.card_description, 13, 1, file);
  fread(&new_header.name_description, 13, 1, file);
  fread(&new_header.color_description, 24, 1, file);
  return new_header;
}

/*
 * Read a line from csv
 */
line_t read_line_from_csv(char line[]) {
  line_t *new_line = malloc(sizeof(line_t));
  char *line_code_str = strsep(&line, ",");
  strcpy(new_line->accept_card, strsep(&line, ","));
  strcpy(new_line->name, add_str_end(format_csv_maybe_empty_str(strsep(&line, ","))));
  strcpy(new_line->color, format_csv_maybe_empty_str(format_csv_last_field(strsep(&line, ","))));
  new_line->size_name = calculate_maybe_null_size(new_line->name);
  new_line->size_color = calculate_maybe_null_size(new_line->color);
  new_line->removed = format_csv_maybe_removed_str(line_code_str);
  new_line->line_code = (int) strtod(line_code_str, NULL);
  new_line->size = calculate_line_size(new_line);
  return *new_line;
}

/*
 * Write line header in bin file
 */
void write_line_header(FILE *file, line_header_t line_header) {
  fwrite(&line_header.status, 1, 1, file);
  fwrite(&line_header.next_reg_byte, 8, 1, file);
  fwrite(&line_header.count, 4, 1, file);
  fwrite(&line_header.count_removed, 4, 1, file);
  fwrite(&line_header.code_description, 15, 1, file);
  fwrite(&line_header.card_description, 13, 1, file);
  fwrite(&line_header.name_description, 13, 1, file);
  fwrite(&line_header.color_description, 24, 1, file);
}

/*
 * Format line dynamic fields
 */
void add_end_to_fields(line_t *line) {
  line->name[line->size_name] = '\0';
  line->color[line->size_color] = '\0';
}

/*
 * Read line from bin file
 */
line_t read_line(FILE *file, long offset) {
  if (offset != 0) fseek(file, offset, SEEK_SET);
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

line_t *read_line_p(FILE *file, long offset) {
  if (offset != 0) fseek(file, offset, SEEK_SET);
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
  return new_line;
}

/*
 * Write line in bin file
 */
void write_line(FILE *file, line_t line) {
  fwrite(invert_remove(line.removed), 1, 1, file);
  fwrite(&line.size, 4, 1, file);
  fwrite(&line.line_code, 4, 1, file);
  fwrite(&line.accept_card, 1, 1, file);
  fwrite(&line.size_name, 4, 1, file);
  fwrite(&line.name, 1, line.size_name, file);
  fwrite(&line.size_color, 4, 1, file);
  fwrite(&line.color, 1, line.size_color, file);
}

/*
 * Create a new line from user input
 */
line_t create_line() {
  line_t *new_line = malloc(sizeof(line_t));
  scanf("%d", &new_line->line_code);
  scan_quote_string(new_line->accept_card);
  scan_quote_string(new_line->name);
  scan_quote_string(new_line->color);
  new_line->size_name = calculate_maybe_null_size(new_line->name);
  new_line->size_color = calculate_maybe_null_size(new_line->color);
  new_line->removed = '0';
  new_line->size = calculate_line_size(new_line);
  return *new_line;
}


/*
 * Verify if file is consistent
 */
int verify_line_header_status(line_header_t header) {
  if (header.status == '0') {
    printf(ERROR_MESSAGE);
    return 0;
  }
  return 1;
}
