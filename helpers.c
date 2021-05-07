//
// Created by Gabriel Souza on 06/05/21.
//

#include "helpers.h"

char *add_empty_padding(char *str, int size) {
  int start = (int) strlen(str);
  str[start] = '\0';
  for (int i = start + 1; i < size; i++) {
    str[i] = '@';
  }
  return str;
}

FILE *open_file(char filename[], char mode[]) {
  FILE *file = fopen(filename, mode);
  if (!file) {
    printf("Error opening file");
    exit(1);
  }
  return file;
}