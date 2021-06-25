/*
 * Gabriel Santos Souza nUSP: 11208176
 * Yann Amado Nunes Costa nUSP: 10746943
 */

#include "handlers.h"

/*
 * Create all vehicle entries from a csv file
 */
void create_table_vehicle(char filename_csv[], char filename_bin[]) {
  char buffer[200];
  FILE *bin_file = open_file(filename_bin, "wb+");
  FILE *csv_file = open_file(filename_csv, "r");

  // Handle header
  fgets(buffer, 200, csv_file);
  vehicle_header_t header = read_vehicle_header_from_csv(buffer);
  write_vehicle_header(bin_file, header);

  // Handle data
  while (fgets(buffer, 200, csv_file) != NULL) {
    vehicle_t new_vehicle = read_vehicle_from_csv(buffer);
    write_vehicle(bin_file, new_vehicle);
    update_header(&header, &new_vehicle);
  }

  // Update header
  header.status = '1';
  header.next_reg_byte = ftell(bin_file);
  fseek(bin_file, 0, SEEK_SET);
  write_vehicle_header(bin_file, header);

  fclose(bin_file);
  fclose(csv_file);
}

/*
 * Create all line entries from a csv file
 */
void create_table_line(char filename_csv[], char filename_bin[]) {
  char buffer[200];
  FILE *bin_file = open_file(filename_bin, "wb+");
  FILE *csv_file = open_file(filename_csv, "r");

  // Handle header
  fgets(buffer, 200, csv_file);
  line_header_t header = read_line_header_from_csv(buffer);
  write_line_header(bin_file, header);

  // Handle data
  while (fgets(buffer, 200, csv_file) != NULL) {
    line_t new_line = read_line_from_csv(buffer);
    write_line(bin_file, new_line);
    update_line_header(&header, &new_line);
  }

  // Update header
  header.status = '1';
  header.next_reg_byte = ftell(bin_file);
  fseek(bin_file, 0, SEEK_SET);
  write_line_header(bin_file, header);

  fclose(bin_file);
  fclose(csv_file);
}

/*
 * Find all entries in vehicles bin file
 */
void select_from_vehicles(char filename[]) {
  vehicle_header_t header;
  FILE *bin_file = open_file(filename, "rb");
  header = read_vehicle_header(bin_file);
  if (header.status == '0') { //if file is not consistent
    printf(ERROR_MESSAGE);
    fclose(bin_file);
    return;
  }
  if (header.count == 0) {
    printf(EMPTY_MESSAGE);
    fclose(bin_file);
    return;
  }
  for (int i = 0; i < header.count + header.count_removed; i++) {
    vehicle_t vehicle = read_vehicle(bin_file, 0);
    if (vehicle.removed != '0') print_vehicle(vehicle);
  }
  fclose(bin_file);
}

/*
 * Find all entries in line bin file
 */
void select_from_lines(char filename[]) {
  line_header_t header;
  FILE *bin_file = open_file(filename, "rb");
  header = read_line_header(bin_file);
  if (header.status == '0') {//if file is not consistent
    printf(ERROR_MESSAGE);
    fclose(bin_file);
    return;
  }
  if (header.count == 0) {
    printf(EMPTY_MESSAGE);
    fclose(bin_file);
    return;
  }
  for (int i = 0; i < header.count + header.count_removed; i++) {
    line_t line = read_line(bin_file, 0);
    if (line.removed != '0') print_line(line);
  }
  fclose(bin_file);
}

/*
 * Find entry by field in vehicle bin file
 */
void find_from_vehicles(char filename[], char fieldname[], char value[]) {
  vehicle_header_t header;
  FILE *bin_file = open_file(filename, "rb");
  header = read_vehicle_header(bin_file);
  if (header.status == '0') { //if file is not consistent
    printf(ERROR_MESSAGE);
    fclose(bin_file);
    return;
  }
  vehicle_t current_vehicle;
  int found = 0;  //its used to know if it should print a vehicle
  int found_total = 0;
  for (int i = 0; i < (header.count + header.count_removed); i++) {
    current_vehicle = read_vehicle(bin_file, 0);
    if (current_vehicle.removed == '1') {  //if the register was not removed
      switch (fieldname[0]) {
        case 'p': //prefixo
          if (strcmp(current_vehicle.prefix, value) == 0) found = 1;
          break;
        case 'd': //data
          if (strcmp(current_vehicle.date, value) == 0) found = 1;
          break;
        case 'q': //quantidade de lugares
          if (current_vehicle.seats == atoi(value)) found = 1;
          break;
        case 'm': //modelo
          if (strcmp(current_vehicle.model, value) == 0) found = 1;
          break;
        case 'c': //categoria
          if (strcmp(current_vehicle.category, value) == 0) found = 1;
          break;
      }
      if (found == 1) {  //if found a match
        print_vehicle(current_vehicle);
        found_total++;    //updates
        found = 0;
      }
    }
  }
  if (found_total == 0) //if it never found any vehicles that match
    printf(EMPTY_MESSAGE);
  fclose(bin_file);
}


/*
 * Find entry by field in line bin file
 */
void find_from_lines(char filename[], char fieldname[], char value[]) {
  line_header_t header;
  FILE *bin_file = open_file(filename, "rb");
  header = read_line_header(bin_file);
  if (header.status == '0') { //if file is not consistent
    printf(ERROR_MESSAGE);
    fclose(bin_file);
    return;
  }
  line_t current_line;
  int found = 0;  //its used to know if it should print a vehicle
  int found_total = 0;
  for (int i = 0; i < (header.count + header.count_removed); i++) {
    current_line = read_line(bin_file, 0);
    if (current_line.removed == '1') {  //if the register was not removed
      switch (fieldname[2]) {
        case 'd': //codLinha
          if (current_line.line_code == atoi(value)) found = 1;
          break;
        case 'e': //aceitaCartao
          if (current_line.accept_card == value) found = 1;
          break;
        case 'm': //nomeLinha
          if (strcmp(current_line.name, value) == 0) found = 1;
          break;
        case 'r': //corLinha
          if (strcmp(current_line.color, value) == 0) found = 1;
          break;
      }
      if (found == 1) {  //if found a match
        print_line(current_line);
        found_total++;    //updates
        found = 0;
      }
    }
  }
  if (found_total == 0) //if it never found any vehicles that match
    printf(EMPTY_MESSAGE);
  fclose(bin_file);
}

/*
 * Handle insert new vehicle on vehicle bin file
 */
void insert_on_vehicles(char filename[], int n) {
  FILE *bin_file = open_file(filename, "rb+");
  vehicle_header_t header = read_vehicle_header(bin_file);
  if (!verify_vehicle_header_status(header)) {
    fclose(bin_file);
    return;
  }

  // mark as inconsistent as we are going to start editing the file
  header.status = '0';
  fseek(bin_file, 0, SEEK_SET);
  write_vehicle_header(bin_file, header);

  vehicle_t current_vehicle;
  fseek(bin_file, header.next_reg_byte, SEEK_SET); //offsets the file to where the new vehicle should be registered
  for (int i = 0; i < n; i++) {
    current_vehicle = create_vehicle();
    write_vehicle(bin_file, current_vehicle);
    header.count += 1;
  }

  // update header whit new values
  header.next_reg_byte = ftell(bin_file);
  header.status = '1';
  fseek(bin_file, 0, SEEK_SET); //offsets to the beginning of the file
  write_vehicle_header(bin_file, header); //updates the header
  fclose(bin_file);
  binarioNaTela(filename);
}

/*
 * Handle insert new line on line bin file
 */
void insert_on_lines(char filename[], int n) {
  line_header_t header;
  FILE *bin_file = open_file(filename, "rb+");
  header = read_line_header(bin_file);
  if (!verify_line_header_status(header)) { // file is incosistent
    fclose(bin_file);
    return;
  }

  // mark as inconsistent as we are going to start editing the file
  header.status = '0';
  fseek(bin_file, 0, SEEK_SET);
  write_line_header(bin_file, header);

  line_t current_line;
  fseek(bin_file, header.next_reg_byte, SEEK_SET); //offsets the file to where the new vehicle should be registered
  for (int i = 0; i < n; i++) {
    current_line = create_line();
    write_line(bin_file, current_line);
    header.count += 1;
  }

  // update header whit new values
  header.next_reg_byte = ftell(bin_file);
  header.status = '1';
  fseek(bin_file, 0, SEEK_SET); //offsets to the beginning of the file
  write_line_header(bin_file, header); //updates the header
  fclose(bin_file);
  binarioNaTela(filename);
}

/*
 * Create index from vehicle file
 */
void create_index_vehicles(char filename[], char filename_index[]) {
  vehicle_t v;
  int c = 0;
  FILE *index_file = open_file(filename_index, "wb+");
  FILE *bin_file = open_file(filename, "rb");
  btree_index_header_t *index_header = init_index_file(index_file);
  vehicle_header_t header = read_vehicle_header(bin_file);
  if (header.status == '0') { //if file is not consistent
    printf(ERROR_MESSAGE);
    fclose(bin_file);
    return;
  }
  if (header.count == 0) {
    printf(EMPTY_MESSAGE);
    fclose(bin_file);
    return;
  }
  for (int i = 0; i <400; i++) {
    size_t current_offset = ftell(bin_file);
    vehicle_t vehicle = read_vehicle(bin_file, 0);
    if (i == 65) v = vehicle;
    if (vehicle.removed != '0') {
      btree_insert(index_file, index_header, convertePrefixo(vehicle.prefix), current_offset);
    }
  }

  fclose(bin_file);
  fclose(index_file);
  index_file = open_file(filename_index, "rb");
  node_t *node = read_index_node(index_file, index_header->root_node_rrn, NULL);
  record_t *record = btree_find_node(index_file, index_header, node, convertePrefixo(v.prefix));
//  node = read_index_node(index_file, 1, NULL);
//  node = read_index_node(index_file, 2, NULL);
  print_in_order(index_file);
  fclose(index_file);
}

/*
 * parses input and decide what to do
 */
void parse_input() {
  int option, new_entries_count;
  char string_arg_1[30], string_arg_2[30], string_arg_3[30];
  scanf("%d", &option);
  switch (option) {
    case 1:
      scanf("%s %s", string_arg_1, string_arg_2);
      create_table_vehicle(string_arg_1, string_arg_2);
      binarioNaTela(string_arg_2);
      break;
    case 2:
      scanf("%s %s", string_arg_1, string_arg_2);
      create_table_line(string_arg_1, string_arg_2);
      binarioNaTela(string_arg_2);
      break;
    case 3:
      scanf("%s", string_arg_1);
      select_from_vehicles(string_arg_1);
      break;
    case 4:
      scanf("%s", string_arg_1);
      select_from_lines(string_arg_1);
      break;
    case 5:
      scanf("%s %s", string_arg_1, string_arg_2);
      scan_quote_string(string_arg_3);
      find_from_vehicles(string_arg_1, string_arg_2, string_arg_3);
      break;
    case 6:
      scanf("%s %s", string_arg_1, string_arg_2);
      scan_quote_string(string_arg_3);
      find_from_lines(string_arg_1, string_arg_2, string_arg_3);
      break;
    case 7:
      scanf("%s %d", string_arg_1, &new_entries_count);
      insert_on_vehicles(string_arg_1, new_entries_count);
      break;
    case 8:
      scanf("%s %d", string_arg_1, &new_entries_count);
      insert_on_lines(string_arg_1, new_entries_count);
      break;
    case 9:
      scanf("%s %s", string_arg_1, string_arg_2);
      create_index_vehicles(string_arg_1, string_arg_2);
      binarioNaTela(string_arg_2);
      break;
    default:
      printf(ERROR_MESSAGE);
      break;
  }
}