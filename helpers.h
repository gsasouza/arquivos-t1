/*
 * Gabriel Santos Souza nUSP: 11208176
 * Yann Amado Nunes Costa nUSP: 10746943
 */

/*
 * Responsible for helpers like csv and bin formatters, default messages and pretty print formatters
 */

#ifndef T1_HELPERS_H
#define T1_HELPERS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

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

void scan_quote_string(char *str);

char format_status_bool(bool status);

int convertePrefixo(char* str);
 
#endif //T1_HELPERS_H