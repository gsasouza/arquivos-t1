//
// Created by Gabriel Souza on 06/05/21.
//

#include "helpers.h"

char *add_empty_padding(char *str, int size) {
  int start = (int) strlen(str);
  if (strcmp(str, "NULO") == 0) start = 0;

//  str[start] = '\0';

  for (int i = start; i < size; i++) {
    str[i] = '@';
  }
  return str;
}

FILE *open_file(char filename[], char mode[]) {
  FILE *file = fopen(filename, mode);
  if (!file) {
    printf(ERROR_MESSAGE);
    exit(0);
  }
  return file;
}

char *format_print_null(char value[]) {
  if ((strcmp(value, "NULO") == 0) || (strcmp(value, "") == 0)) return NULL_MESSAGE;
  return value;
}

char *format_print_null_int(int value) {
  char *buffer = malloc(sizeof(int));
  if (value == -1) return NULL_MESSAGE;
  sprintf(buffer, "%d", value);
  return buffer;
}


char *format_print_date(char date[]) {
  char *buffer = malloc(sizeof(char) * 100);
  char *months[] = {"janeiro", "fevereiro", "março", "abril", "maio", "junho", "julho", "agosto", "setembro", "outubro",
                    "novembro", "dezembro"};

  if ((strcmp(date, "NULO") == 0) || (strcmp(date, "") == 0)) return format_print_null(date);

  int year = (int) strtod(strsep(&date, "-"), NULL);
  int month = (int) strtod(strsep(&date, "-"), NULL);
  int day = (int) strtod(strsep(&date, "-"), NULL);
  sprintf(buffer, "%02d de %s de %d", day, months[month - 1], year);
  return buffer;
}


void binarioNaTela(char *nomeArquivoBinario) { /* Você não precisa entender o código dessa função. */

  /* Use essa função para comparação no run.codes. Lembre-se de ter fechado (fclose) o arquivo anteriormente.
  *  Ela vai abrir de novo para leitura e depois fechar (você não vai perder pontos por isso se usar ela). */

  unsigned long i, cs;
  unsigned char *mb;
  size_t fl;
  FILE *fs;
  if (nomeArquivoBinario == NULL || !(fs = fopen(nomeArquivoBinario, "rb"))) {
    fprintf(stderr,
            "ERRO AO ESCREVER O BINARIO NA TELA (função binarioNaTela): não foi possível abrir o arquivo que me passou para leitura. Ele existe e você tá passando o nome certo? Você lembrou de fechar ele com fclose depois de usar?\n");
    return;
  }
  fseek(fs, 0, SEEK_END);
  fl = ftell(fs);
  fseek(fs, 0, SEEK_SET);
  mb = (unsigned char *) malloc(fl);
  fread(mb, 1, fl, fs);

  cs = 0;
  for (i = 0; i < fl; i++) {
    cs += (unsigned long) mb[i];
  }
  printf("%lf\n", (cs / (double) 100));
  free(mb);
  fclose(fs);
}
