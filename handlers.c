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
 * Join vehicles and lines using nested loop join
 */
void nested_loop_join(char filename_vehicles[], char filename_lines[], char field_vehicles[], char field_lines[]) {
  vehicle_t vehicle;
  line_t line;
  int count = 0;
  // open files
  FILE *vehicles_file = open_file(filename_vehicles, "rb");
  FILE *lines_file = open_file(filename_lines, "rb");

  // read indexes
  vehicle_header_t vehicle_header = read_vehicle_header(vehicles_file);
  line_header_t line_header = read_line_header(lines_file);

  //if either vehicle or line is not consistent
  if (vehicle_header.status == '0' || line_header.status == '0') {
    printf(ERROR_MESSAGE);
    fclose(vehicles_file);
    fclose(lines_file);
    return;
  }

  // inf file is empty
  if (vehicle_header.count == 0) {
    printf(EMPTY_MESSAGE);
    fclose(vehicles_file);
    fclose(lines_file);
    return;
  }

  // nested loop join
  for (int i = 0; i < vehicle_header.count + vehicle_header.count_removed; i++) {
    vehicle = read_vehicle(vehicles_file, 0);
    if (vehicle.removed == '0') continue;
    // go back to lines start
    fseek(lines_file, 82, SEEK_SET);
    // loop through lines
    for (int j = 0; j < line_header.count + line_header.count_removed; j++) {
      line = read_line(lines_file, 0);
      if (line.removed == '0') continue;
      if (vehicle.line_code == line.line_code) {
        print_vehicle(vehicle);
        print_line(line);
        printf("\n\n");
        count++;
        continue;
      }
    }
  }

  // no joins found
  if (count == 0) {
    printf(EMPTY_MESSAGE);
  }

  // close files
  fclose(vehicles_file);
  fclose(lines_file);

}

/*
 * Join vehicles and lines using nested loop join
 */
void single_loop_join(char filename_vehicles[], char filename_lines[], char field_vehicles[], char field_lines[],
                      char filename_line_index[]) {
  vehicle_t vehicle;
  line_t line;
  int count = 0;
  // open files
  FILE *vehicles_file = open_file(filename_vehicles, "rb");
  FILE *lines_file = open_file(filename_lines, "rb");
  FILE *lines_index_file = open_file(filename_line_index, "rb");

  // read indexes
  vehicle_header_t vehicle_header = read_vehicle_header(vehicles_file);
  line_header_t line_header = read_line_header(lines_file);
  btree_index_header_t *lines_index_header = read_index_header(lines_index_file);

  //if either vehicle, line or line header is not consistent
  if (vehicle_header.status == '0' || line_header.status == '0' || lines_index_header->status == false) {
    printf(ERROR_MESSAGE);
    fclose(vehicles_file);
    fclose(lines_file);
    fclose(lines_index_file);
    return;
  }

  // inf file is empty
  if (vehicle_header.count == 0) {
    printf(EMPTY_MESSAGE);
    fclose(vehicles_file);
    fclose(lines_file);
    fclose(lines_index_file);
    return;
  }

  // read btree root
  node_t *root_node = read_index_node(lines_index_file, lines_index_header->root_node_rrn, NULL);
  record_t *record;

  // single loop join
  for (int i = 0; i < vehicle_header.count + vehicle_header.count_removed; i++) {
    vehicle = read_vehicle(vehicles_file, 0);
    if (vehicle.removed == '0') continue;
    record = btree_find_node(lines_index_file, lines_index_header, root_node, vehicle.line_code);
    if (!record) continue;
    line = read_line(lines_file, record->value);
    if (line.removed == '0') continue;
    print_vehicle(vehicle);
    print_line(line);
    printf("\n\n");
    count++;
  }

  if (count == 0) {
    printf(EMPTY_MESSAGE);
  }

  // close files
  fclose(vehicles_file);
  fclose(lines_file);

}



/*
 * sort vehicles based on sorting field
 */
void sort_vehicles(char unsorted_vehicle_filename[], char sorted_vehicle_filename[], char sorting_field[], int op) {

    // open unsorted file
    FILE *unsorted_vehicle_file = fopen(unsorted_vehicle_filename, "rb");

    // read index
    vehicle_header_t unsorted_vehicle_header = read_vehicle_header(unsorted_vehicle_file);

    // if vehicle header is not consistent
    if (unsorted_vehicle_header.status == '0') {
        //if its the 17th op
        if(op == 17)
            printf(ERROR_MESSAGE);
        fclose(unsorted_vehicle_file);
        return;
    }

    // if file is empty
    if (unsorted_vehicle_header.count == 0) {
        if(op == 17)
            printf(EMPTY_MESSAGE);
        fclose(unsorted_vehicle_file);
        return;
    }

    // create space for all vehicles
    vehicle_t* vehicles = malloc((unsorted_vehicle_header.count)*sizeof(vehicle_t));

    // auxiliary vehicle to check if it is removed;
    vehicle_t current_vehicle;

    // counts how many vehicles are not logically removed
    int n_vehicles = 0;

    int total_vehicles = unsorted_vehicle_header.count + unsorted_vehicle_header.count_removed;

    // pulling all vehicles into RAM
    for(int i = 0; i < total_vehicles; i++){

        // passing one vehicle to RAM, offset is not needed since its reading sequentially
        current_vehicle = read_vehicle(unsorted_vehicle_file, 0);

        // if its not logically removed
        if(current_vehicle.removed == '1'){
            vehicles[n_vehicles] = current_vehicle;
            n_vehicles++;
        }
    }
    // sorting
    mergesort_vehicle(vehicles, 0, n_vehicles-1, sorting_field);

    //open file to sort
    FILE *sorted_vehicle_file = fopen(sorted_vehicle_filename, "wb");

    // create index, copies most of the useful info, other infos will be updated later
    vehicle_header_t sorted_vehicle_header = unsorted_vehicle_header;
    sorted_vehicle_header.status = '0';

    // updating header infos
    sorted_vehicle_header.count = n_vehicles;
    sorted_vehicle_header.count_removed = 0;



    // writing header on sorted file
    write_vehicle_header(sorted_vehicle_file, sorted_vehicle_header);

    // pushing all vehicles to sorted file
    for(int i = 0; i < n_vehicles; i++){
        // the write vehicle func inverts the removed notation, so '0' means not removed
        vehicles[i].removed = '0';
        write_vehicle(sorted_vehicle_file, vehicles[i]);
    }

    //writing the byteoffset
    sorted_vehicle_header.next_reg_byte = ftell(sorted_vehicle_file);

    //updating header status to stable
    sorted_vehicle_header.status = '1';

    fseek(sorted_vehicle_file, 0, SEEK_SET);
    write_vehicle_header(sorted_vehicle_file, sorted_vehicle_header);

    if(op == 17){
        if (n_vehicles == 0)
            printf(EMPTY_MESSAGE);
        else
            binarioNaTela(sorted_vehicle_filename);
    }



    // close files
    fclose(unsorted_vehicle_file);
    fclose(sorted_vehicle_file);

    // frees all pointers
    free(vehicles);
}

/*
 * sort lines based on sorting field
 */
void sort_lines(char unsorted_line_filename[], char sorted_line_filename[], char sorting_field[], int op) {

    // open unsorted file
    FILE *unsorted_line_file = fopen(unsorted_line_filename, "rb");

    // read index
    line_header_t unsorted_line_header = read_line_header(unsorted_line_file);

    // if line  header is not consistent
    if (unsorted_line_header.status == '0') {
        //if its the 18th op
        if(op == 18)
            printf(ERROR_MESSAGE);
        fclose(unsorted_line_file);
        return;
    }

    // if file is empty
    if (unsorted_line_header.count == 0) {
        if(op == 18)
            printf(EMPTY_MESSAGE);
        fclose(unsorted_line_file);
        return;
    }

    // create space for all lines
    line_t* lines = malloc((unsorted_line_header.count)*sizeof(line_t));

    // auxiliary line to check if it is removed;
    line_t current_line;

    // counts how many lines are not logically removed
    int n_lines = 0;

    int total_lines = unsorted_line_header.count + unsorted_line_header.count_removed;

    // pulling all lines into RAM
    for(int i = 0; i < total_lines; i++){

        // passing one line to RAM, offset is not needed since its reading sequentially
        current_line = read_line(unsorted_line_file, 0);

        // if its not logically removed
        if(current_line.removed == '1'){
            lines[n_lines] = current_line;
            n_lines++;
        }
    }
    // sorting
    mergesort_line(lines, 0, n_lines-1, sorting_field);

    //open file to sort
    FILE *sorted_line_file = fopen(sorted_line_filename, "wb");

    // create index, copies most of the useful info, other infos will be updated later
    line_header_t sorted_line_header = unsorted_line_header;
    sorted_line_header.status = '0';

    // updating header infos
    sorted_line_header.count = n_lines;
    sorted_line_header.count_removed = 0;



    // writing header on sorted file
    write_line_header(sorted_line_file, sorted_line_header);

    // pushing all lines to sorted file
    for(int i = 0; i < n_lines; i++){
        // the write line func inverts the removed notation, so '0' means not removed
        lines[i].removed = '0';
        write_line(sorted_line_file, lines[i]);
    }

    //writing the byteoffset
    sorted_line_header.next_reg_byte = ftell(sorted_line_file);

    //updating header status to stable
    sorted_line_header.status = '1';

    fseek(sorted_line_file, 0, SEEK_SET);
    write_line_header(sorted_line_file, sorted_line_header);

    if(op == 18){
        if (n_lines == 0)
            printf(EMPTY_MESSAGE);
        else
            binarioNaTela(sorted_line_filename);
    }



    // close files
    fclose(unsorted_line_file);
    fclose(sorted_line_file);

    // frees all pointers
    free(lines);
}

void show_all_compatible_registers(char vehicle_filename[], char line_filename[],
                                   char vehicle_sorting_field[], char line_sorting_field[]){

    // Use previous functions to sort
    sort_vehicles(vehicle_filename, vehicle_filename, vehicle_sorting_field, 19);
    sort_lines(line_filename, line_filename, line_sorting_field, 19);

    //Opening files
    FILE *vehicle_file = fopen(vehicle_filename, "rb");
    FILE *line_file = fopen(line_filename, "rb");

    // read indexes
    vehicle_header_t vehicle_header = read_vehicle_header(vehicle_file);
    line_header_t line_header = read_line_header(line_file);

    // if line or vehicle header is not consistent
    if (vehicle_header.status == '0' || line_header.status == '0') {
        printf(ERROR_MESSAGE);
        fclose(vehicle_file);
        fclose(line_file);
        return;
    }

    // if one of the files is empty
    if (vehicle_header.count == 0 || line_header.count == 0) {
        printf(EMPTY_MESSAGE);
        fclose(vehicle_file);
        fclose(line_file);
        return;
    }
    // n will be the one with least registers between lines and vehicles
    int vehicle_total = vehicle_header.count + vehicle_header.count_removed;
    int line_total = line_header.count + line_header.count_removed;

    // creating the vehicle and lines variables to read from file
    vehicle_t current_vehicle = read_vehicle(vehicle_file, 0);
    line_t current_line = read_line(line_file, 0);

    // counting how many registers were printed
    int register_total = 0;
    // reading both files and checking if the vehicle and line match
    for(int v = 0, l = 0; v < vehicle_total && l < line_total;){
        if(current_vehicle.line_code == current_line.line_code){
            print_vehicle(current_vehicle);
            print_line(current_line);
            printf("\n\n");
            register_total++;
            v++;
            //line is not updated because 1 vehicle only goest o 1 line, but 1 line can go to several vehicles
            if(v < vehicle_total)
                current_vehicle = read_vehicle(vehicle_file, 0);
        }
        //iterate through vehicles until find ones with same line_code
        else if(current_vehicle.line_code < current_line.line_code){
            current_vehicle = read_vehicle(vehicle_file, 0);
            v++;
        }
        //if the vehicles now have bigger line_codes
        //vehicle is not updated because the previous vehicle could match with the next line
        else{
            current_line = read_line(line_file, 0);
            l++;
        }


    }
    //if nothing was printed
    if(register_total == 0)
        printf(EMPTY_MESSAGE);

}

/*
 * parses input and decide what to do
 */
void parse_input() {
  int option, new_entries_count, int_arg_1;
  char string_arg_1[30], string_arg_2[30], string_arg_3[30], string_arg_4[30], string_arg_5[30];
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

    //Do join using nested loop
    case 15:
      scanf("%s %s %s %s", string_arg_1, string_arg_2, string_arg_3, string_arg_4);
      nested_loop_join(string_arg_1, string_arg_2, string_arg_3, string_arg_4);
      break;
    //Do join using single loop
    case 16:
      scanf("%s %s %s %s %s", string_arg_1, string_arg_2, string_arg_3, string_arg_4, string_arg_5);
      single_loop_join(string_arg_1, string_arg_2, string_arg_3, string_arg_4, string_arg_5);
      break;
    // Sorts vehicles based on field
    case 17:
      scanf("%s %s %s", string_arg_1, string_arg_2, string_arg_3);
      sort_vehicles(string_arg_1, string_arg_2, string_arg_3, 17);
      break;
    // Sorts lines based on field
    case 18:
      scanf("%s %s %s", string_arg_1, string_arg_2, string_arg_3);
      sort_lines(string_arg_1, string_arg_2, string_arg_3, 18);
      break;
    // Prints all registers if they are compatible on both files
    case 19:
      scanf("%s %s %s %s", string_arg_1, string_arg_2, string_arg_3, string_arg_4);
      show_all_compatible_registers(string_arg_1, string_arg_2, string_arg_3, string_arg_4);
      break;
    //Any other unexpected command
    default:
      printf(ERROR_MESSAGE);
      break;
  }
}