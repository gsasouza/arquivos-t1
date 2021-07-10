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

  //Checking for inconsistencies
  if (header.status == '0') {
    printf(ERROR_MESSAGE);
    fclose(bin_file);
    return;
  }

  vehicle_t current_vehicle;
  int found = 0;  //its used to know if it should print a vehicle
  int found_total = 0;
  for (int i = 0; i < (header.count + header.count_removed); i++) {
    current_vehicle = read_vehicle(bin_file, 0);
    //if the register was not removed
    if (current_vehicle.removed == '1') {
      //Checking where to search
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

      //if a match was found
      if (found == 1) {
        print_vehicle(current_vehicle);
        found_total++;    //updates
        found = 0;
      }
    }
  }

  //if it never found any vehicles that match
  if (found_total == 0)
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

  //Checking for inconsistencies
  if (header.status == '0') {
    printf(ERROR_MESSAGE);
    fclose(bin_file);
    return;
  }

  line_t current_line;
  int found = 0;  //its used to know if it should print a vehicle
  int found_total = 0;

  for (int i = 0; i < (header.count + header.count_removed); i++) {
    current_line = read_line(bin_file, 0);
    //if the register was not removed
    if (current_line.removed == '1') {
      //Checking where to search
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

      //if a match was found
      if (found == 1) {
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
void insert_on_vehicles(char filename[], char filename_index[], int n) {
  FILE *bin_file = open_file(filename, "rb+");
  FILE *index_file = open_file(filename_index, "rb+");
  vehicle_header_t header = read_vehicle_header(bin_file);
  btree_index_header_t *index_header = read_index_header(index_file);

  //Checking for inconsistencies
  if (header.status == 0 || index_header->status == 0) {
    printf(ERROR_MESSAGE);
    fclose(bin_file);
    fclose(index_file);
    return;
  }

  // mark as inconsistent as we are going to start editing the file
  header.status = '0';
  //Place cursor at the beginning of bin_file
  fseek(bin_file, 0, SEEK_SET);
  write_vehicle_header(bin_file, header);

  index_header->status = '0';
  //Place cursor at the beginning of index_file
  fseek(index_file, 0, SEEK_SET);
  write_index_header(index_file, index_header);

  vehicle_t current_vehicle;
  //offsets the file to where the new vehicle should be registered
  fseek(bin_file, header.next_reg_byte, SEEK_SET);

  size_t current_offset = 0;
  for (int i = 0; i < n; i++) {
    current_offset = ftell(bin_file);
    current_vehicle = create_vehicle();
    write_vehicle(bin_file, current_vehicle);
    btree_insert(index_file, index_header, convertePrefixo(current_vehicle.prefix), current_offset);
    header.count += 1;
  }

  // update header whit new values
  header.next_reg_byte = ftell(bin_file);

  // mark as consistent as we are going to close the file
  header.status = '1';
  fseek(bin_file, 0, SEEK_SET); //offsets to the beginning of the file
  write_vehicle_header(bin_file, header); //updates the header

  index_header->status = '1';
  fseek(index_file, 0, SEEK_SET); //offsets to the beginning of the file
  write_index_header(index_file, index_header); //updates the header

  //Closing files
  fclose(bin_file);
  fclose(index_file);
  binarioNaTela(filename_index);
}

/*
 * Handle insert new line on line bin file
 */
void insert_on_lines(char filename[], char filename_index[], int n) {
  line_header_t header;
  FILE *bin_file = open_file(filename, "rb+");
  FILE *index_file = open_file(filename_index, "rb+");

  header = read_line_header(bin_file);
  btree_index_header_t *index_header = read_index_header(index_file);

  //Checking for inconsistencies
  if (header.status == 0 || index_header->status == 0) {
    printf(ERROR_MESSAGE);
    fclose(bin_file);
    fclose(index_file);
    return;
  }
  // mark as inconsistent as we are going to start editing the file
  header.status = '0';
  //Place cursor at the beginning of bin_file
  fseek(bin_file, 0, SEEK_SET);
  write_line_header(bin_file, header);

  index_header->status = '0';
  //Place cursor at the beginning of index_file
  fseek(index_file, 0, SEEK_SET);
  write_index_header(index_file, index_header);

  line_t current_line;
  //offsets the file to where the new vehicle should be registered
  fseek(bin_file, header.next_reg_byte, SEEK_SET);

  size_t current_offset = 0;
  for (int i = 0; i < n; i++) {
    current_offset = ftell(bin_file);
    current_line = create_line();
    write_line(bin_file, current_line);
    btree_insert(index_file, index_header, current_line.line_code, current_offset);
    header.count += 1;
  }

  // update header whit new values
  header.next_reg_byte = ftell(bin_file);

  // mark as consistent as we are going to close the file
  header.status = '1';
  fseek(bin_file, 0, SEEK_SET); //offsets to the beginning of the file
  write_line_header(bin_file, header); //updates the header

  index_header->status = true;
  fseek(index_file, 0, SEEK_SET); //offsets to the beginning of the file
  write_index_header(index_file, index_header); //updates the header

  //Closing files
  fclose(bin_file);
  fclose(index_file);
  binarioNaTela(filename_index);
}

/*
 * Create index from vehicle file
 */
void create_index_vehicles(char filename[], char filename_index[]) {
  // open files
  FILE *index_file = open_file(filename_index, "wb+");
  FILE *bin_file = open_file(filename, "rb");

  // read indexes
  btree_index_header_t *index_header = init_index_file(index_file);
  vehicle_header_t header = read_vehicle_header(bin_file);

  //if file is not consistent
  if (header.status == '0') {
    printf(ERROR_MESSAGE);
    fclose(bin_file);
    return;
  }
  if (header.count == 0) {
    printf(EMPTY_MESSAGE);
    fclose(bin_file);
    return;
  }

  // read all vehicles and add to btree index
  for (int i = 0; i < header.count + header.count_removed; i++) {
    size_t current_offset = ftell(bin_file);
    vehicle_t vehicle = read_vehicle(bin_file, 0);
    if (vehicle.removed != '0') {
      btree_insert(index_file, index_header, convertePrefixo(vehicle.prefix), current_offset);
    }
  }

  // update index header
  index_header->status = true;
  write_index_header(index_file, index_header);

  // close
  fclose(bin_file);
  fclose(index_file);
  binarioNaTela(filename_index);
}


/*
 * Create index from line file
 */
void create_index_line(char filename[], char filename_index[]) {
  // open files
  FILE *index_file = open_file(filename_index, "wb+");
  FILE *bin_file = open_file(filename, "rb");

  // read headers
  btree_index_header_t *index_header = init_index_file(index_file);
  line_header_t header = read_line_header(bin_file);

  //if file is not consistent
  if (header.status == '0') {
    printf(ERROR_MESSAGE);
    fclose(bin_file);
    return;
  }

  // if lines is empty
  if (header.count == 0) {
    printf(EMPTY_MESSAGE);
    fclose(bin_file);
    return;
  }

  // read all lines and add to btree index
  for (int i = 0; i < header.count + header.count_removed; i++) {
    size_t current_offset = ftell(bin_file);
    line_t line = read_line(bin_file, 0);
    if (line.removed != '0') {
      btree_insert(index_file, index_header, line.line_code, current_offset);
    }
  }

  // update btree index
  index_header->status = true;
  write_index_header(index_file, index_header);

  // close files
  fclose(bin_file);
  fclose(index_file);

  binarioNaTela(filename_index);
}

/*
 * Select from vehicles file using btree index
 */
void select_from_vehicles_index(char filename[], char filename_index[], char prefix[]) {
  // open files
  FILE *index_file = open_file(filename_index, "rb");
  FILE *bin_file = open_file(filename, "rb");

  // read indexes
  btree_index_header_t *index_header = read_index_header(index_file);
  vehicle_header_t header = read_vehicle_header(bin_file);

  //if either vehicle or index is not consistent
  if (header.status == '0' || index_header->status == false) {
    printf(ERROR_MESSAGE);
    fclose(bin_file);
    fclose(index_file);
    return;
  }

  // inf file is empty
  if (header.count == 0) {
    printf(EMPTY_MESSAGE);
    fclose(bin_file);
    fclose(index_file);
    return;
  }

  // find prefix in btree index
  node_t *root_node = read_index_node(index_file, index_header->root_node_rrn, NULL);
  record_t *record = btree_find_node(index_file, index_header, root_node, convertePrefixo(prefix));

  // print if found
  if (!record) printf(EMPTY_MESSAGE);
  else {
    vehicle_t vehicle = read_vehicle(bin_file, record->value);
    print_vehicle(vehicle);
  }

  // close files
  fclose(bin_file);
  fclose(index_file);
}

/*
 * Select from lines file using btree index
 */
void select_from_lines_index(char filename[], char filename_index[], int line_code) {
  //open files
  FILE *index_file = open_file(filename_index, "rb");
  FILE *bin_file = open_file(filename, "rb");

  // read indexes
  btree_index_header_t *index_header = read_index_header(index_file);
  line_header_t header = read_line_header(bin_file);

  //if either line or index is not consistent
  if (header.status == '0' || index_header->status == false) {
    printf(ERROR_MESSAGE);
    fclose(bin_file);
    fclose(index_file);
    return;
  }

  // if lines is empty
  if (header.count == 0) {
    printf(EMPTY_MESSAGE);
    fclose(bin_file);
    fclose(index_file);
    return;
  }

  // find prefix in btree index
  node_t *root_node = read_index_node(index_file, index_header->root_node_rrn, NULL);
  record_t *record = btree_find_node(index_file, index_header, root_node, line_code);

  // print if found
  if (!record) printf(EMPTY_MESSAGE);
  else {
    line_t line = read_line(bin_file, record->value);
    print_line(line);
  }

  // close files
  fclose(bin_file);
  fclose(index_file);
}

/*
 * parses input and decide what to do
 */
void parse_input() {
  int option, new_entries_count, int_arg_1;
  char string_arg_1[30], string_arg_2[30], string_arg_3[30];
  scanf("%d", &option);
  switch (option) {
    //Create table command for vehicles
    case 1:
      scanf("%s %s", string_arg_1, string_arg_2);
      create_table_vehicle(string_arg_1, string_arg_2);
      binarioNaTela(string_arg_2);
      break;
    //Create table command for lines
    case 2:
      scanf("%s %s", string_arg_1, string_arg_2);
      create_table_line(string_arg_1, string_arg_2);
      binarioNaTela(string_arg_2);
      break;
    //Select from vehicles
    case 3:
      scanf("%s", string_arg_1);
      select_from_vehicles(string_arg_1);
      break;
    //Select from lines
    case 4:
      scanf("%s", string_arg_1);
      select_from_lines(string_arg_1);
      break;
    //Select from vehicles where ...
    case 5:
      scanf("%s %s", string_arg_1, string_arg_2);
      scan_quote_string(string_arg_3);
      find_from_vehicles(string_arg_1, string_arg_2, string_arg_3);
      break;
    //Select from lines where ...
    case 6:
      scanf("%s %s", string_arg_1, string_arg_2);
      scan_quote_string(string_arg_3);
      find_from_lines(string_arg_1, string_arg_2, string_arg_3);
      break;
    //Insert into vehicles
    case 7:
      scanf("%s %d", string_arg_1, &new_entries_count);
      insert_on_vehicles(string_arg_1, NULL, new_entries_count);
      break;
    //Insert into lines
    case 8:
      scanf("%s %d", string_arg_1, &new_entries_count);
      insert_on_lines(string_arg_1, NULL, new_entries_count);
      break;
    //Create a b-tree vehicle index
    case 9:
      scanf("%s %s", string_arg_1, string_arg_2);
      create_index_vehicles(string_arg_1, string_arg_2);
      break;
    //Create a b-tree line index
    case 10:
      scanf("%s %s", string_arg_1, string_arg_2);
      create_index_line(string_arg_1, string_arg_2);
      break;
    //Search on the b-tree vehicle index with key
    case 11:
      scanf("%s %s %s", string_arg_1, string_arg_2, string_arg_3);
      scan_quote_string(string_arg_3);
      select_from_vehicles_index(string_arg_1, string_arg_2, string_arg_3);
      break;
    //Search on the b-tree line index with key
    case 12:
      scanf("%s %s %s %d", string_arg_1, string_arg_2, string_arg_3, &int_arg_1);
      select_from_lines_index(string_arg_1, string_arg_2, int_arg_1);
      break;
    //Insert into vehicles and on the b-tree vehicle index
    case 13:
        scanf("%s %s %d", string_arg_1, string_arg_2, &int_arg_1);
        insert_on_vehicles(string_arg_1, string_arg_2, int_arg_1);
        break;
    //Insert into lines and on the b-tree line index
    case 14:
        scanf("%s %s %d", string_arg_1, string_arg_2, &int_arg_1);
        insert_on_lines(string_arg_1, string_arg_2, int_arg_1);
        break;
    //Any other unexpected command
    default:
      printf(ERROR_MESSAGE);
      break;
  }
}