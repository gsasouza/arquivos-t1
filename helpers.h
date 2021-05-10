//
// Created by Gabriel Souza on 06/05/21.
//

#ifndef T1_HELPERS_H
#define T1_HELPERS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NULL_MESSAGE "campo com valor nulo"

char *add_empty_padding(char *str, int size);

FILE *open_file(char filename[], char mode[]);

void binarioNaTela(char *nomeArquivoBinario);

char *format_print_date(char date[]);

char *format_print_null(char value[]);

char *format_print_null_int(int value);

#endif //T1_HELPERS_H