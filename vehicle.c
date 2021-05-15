//Yann Amado Nunes Costa nUSP: 10746943

#include "vehicle.h"

int calculate_vehicle_size(vehicle_t *vehicle) {
  return vehicle->size = 31 + vehicle->size_model + vehicle->size_category;
}

vehicle_t read_vehicle_from_csv(char line[]) {
  vehicle_t *new_vehicle = malloc(sizeof(vehicle_t));
  strcpy(new_vehicle->prefix, add_str_end(strsep(&line, ",")));
  strcpy(new_vehicle->date, add_str_end(format_csv_maybe_empty_str(strsep(&line, ","))));
  new_vehicle->seats = (int) strtod(strsep(&line, ","), NULL);
  new_vehicle->line_code = format_csv_maybe_empty_int(strsep(&line, ","));
  strcpy(new_vehicle->model, format_csv_maybe_empty_str(strsep(&line, ",")));
  strcpy(new_vehicle->category, format_csv_maybe_empty_str(format_csv_last_field(strsep(&line, ","))));
  new_vehicle->size_model = calculate_maybe_null_size(new_vehicle->model);
  new_vehicle->size_category = calculate_maybe_null_size(new_vehicle->category);
  new_vehicle->size = calculate_vehicle_size(new_vehicle);
  new_vehicle->removed = format_csv_maybe_removed_str(new_vehicle->prefix);
  return *new_vehicle;
}

vehicle_header_t read_vehicle_header_from_csv(char line[]) {
  vehicle_header_t new_header;
  strcpy(new_header.prefix_description, strsep(&line, ","));
  strcpy(new_header.date_description, strsep(&line, ","));
  strcpy(new_header.seats_description, strsep(&line, ","));
  strcpy(new_header.line_description, strsep(&line, ","));
  strcpy(new_header.model_description, strsep(&line, ","));
  strcpy(new_header.category_description, format_csv_last_field(strsep(&line, ",")));
  new_header.status = '0';
  new_header.count = 0;
  new_header.count_removed = 0;
  new_header.next_reg_byte = 0;
  return new_header;
}

void update_header(vehicle_header_t *header, vehicle_t *vehicle) {
  if (vehicle->removed == '1') header->count_removed = header->count_removed + 1;
  else header->count = header->count + 1;
}

void print_vehicle(vehicle_t vehicle) {
  printf("Prefixo do veiculo: %s\n", format_print_null(vehicle.prefix));
  printf("Modelo do veiculo: %s\n", format_print_null(vehicle.model));
  printf("Categoria do veiculo: %s\n", format_print_null(vehicle.category));
  printf("Data de entrada do veiculo na frota: %s\n", format_print_date(vehicle.date));
  printf("Quantidade de lugares sentados disponiveis: %s\n\n", format_print_null_int(vehicle.seats));
}



void write_vehicle(FILE *file, vehicle_t vehicle) {
  fwrite(invert_remove(vehicle.removed), 1, 1, file);
  fwrite(&vehicle.size, 4, 1, file);
  fwrite(add_empty_padding(vehicle.prefix, 5), 5, 1, file);
  fwrite(add_empty_padding(vehicle.date, 10), 10, 1, file);
  fwrite(&vehicle.seats, 4, 1, file);
  fwrite(&vehicle.line_code, 4, 1, file);
  fwrite(&vehicle.size_model, 4, 1, file);
  fwrite(&vehicle.model, 1, vehicle.size_model, file);
  fwrite(&vehicle.size_category, 4, 1, file);
  fwrite(&vehicle.category, 1, vehicle.size_category, file);
}


void add_end_to_vehicle_fields(vehicle_t *vehicle) {
  vehicle->model[vehicle->size_model] = '\0';
  vehicle->category[vehicle->size_category] = '\0';
  vehicle->date[10] = '\0';
  if (vehicle->date[0] == '@') vehicle->date[0] = '\0';
}

vehicle_t read_vehicle(FILE *file, int offset) {
  vehicle_t *new_vehicle = malloc(sizeof(vehicle_t));
  fread(&new_vehicle->removed, 1, 1, file);
  fread(&new_vehicle->size, 4, 1, file);
  fread(&new_vehicle->prefix, 5, 1, file);
  fread(&new_vehicle->date, 10, 1, file);
  fread(&new_vehicle->seats, 4, 1, file);
  fread(&new_vehicle->line_code, 4, 1, file);
  fread(&new_vehicle->size_model, 4, 1, file);
  fread(&new_vehicle->model, 1, new_vehicle->size_model, file);
  fread(&new_vehicle->size_category, 4, 1, file);
  fread(&new_vehicle->category, 1, new_vehicle->size_category, file);
  add_end_to_vehicle_fields(new_vehicle);
  return *new_vehicle;
}

vehicle_header_t read_vehicle_header(FILE *file) {
  vehicle_header_t *new_header = malloc(sizeof(vehicle_header_t));
  fread(&new_header->status, 1, 1, file);
  fread(&new_header->next_reg_byte, 8, 1, file);
  fread(&new_header->count, 4, 1, file);
  fread(&new_header->count_removed, 4, 1, file);
  fread(&new_header->prefix_description, 18, 1, file);
  fread(&new_header->date_description, 35, 1, file);
  fread(&new_header->seats_description, 42, 1, file);
  fread(&new_header->line_description, 26, 1, file);
  fread(&new_header->model_description, 17, 1, file);
  fread(&new_header->category_description, 20, 1, file);
  return *new_header;
}


void write_vehicle_header(FILE *file, vehicle_header_t vehicle_header) {
  fwrite(&vehicle_header.status, 1, 1, file);
  fwrite(&vehicle_header.next_reg_byte, 8, 1, file);
  fwrite(&vehicle_header.count, 4, 1, file);
  fwrite(&vehicle_header.count_removed, 4, 1, file);
  fwrite(&vehicle_header.prefix_description, 18, 1, file);
  fwrite(&vehicle_header.date_description, 35, 1, file);
  fwrite(&vehicle_header.seats_description, 42, 1, file);
  fwrite(&vehicle_header.line_description, 26, 1, file);
  fwrite(&vehicle_header.model_description, 17, 1, file);
  fwrite(&vehicle_header.category_description, 20, 1, file);
}