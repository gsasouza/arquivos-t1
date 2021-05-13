//
// Created by Gabriel Souza on 06/05/21.
//

#ifndef T1_HELPERS_H
#define T1_HELPERS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NULL_MESSAGE "campo com valor nulo"
#define ERROR_MESSAGE "Falha no processamento do arquivo."
#define EMPTY_MESSAGE "Registro inexistente."

char *add_empty_padding(char *str, int size);

FILE *open_file(char filename[], char mode[]);

void binarioNaTela(char *nomeArquivoBinario);

char *format_print_date(char date[]);

char *format_print_null(char value[]);

char *format_print_null_int(int value);

size_t calculate_maybe_null_size(char str[]);

int format_csv_maybe_empty_int(char *str);

char *format_csv_maybe_empty_str(char *str);

char format_csv_maybe_removed_str(char *str);

char *format_csv_last_field(char *str);

char *add_str_end(char *str);

char *invert_remove(char removed);

#endif //T1_HELPERS_H