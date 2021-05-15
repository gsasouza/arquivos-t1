#include "helpers.h"
#include "ctype.h"

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
    printf(ERROR_MESSAGE);
    exit(0);
  }
  return file;
}

char *format_print_null(char value[]) {
  if ((strcmp(value, "NULO") == 0) || (strcmp(value, "") == 0)) return NULL_MESSAGE;
  return value;
}

int format_csv_maybe_empty_int(char *str) {
  if (strcmp(str, "NULO") == 0) return -1;
  return (int) strtod(str, NULL);
}

char *format_csv_maybe_empty_str(char *str) {
  if (strcmp(str, "NULO") == 0) return "";
  return str;
}

char format_csv_maybe_removed_str(char *str) {
  if (str[0] == '*') {
    memmove(str, str + 1, 5);
    str[5] = '\0';
    return '1';
  }
  return '0';
}

char *format_csv_last_field(char *str) {
  str[strlen(str) - 1] = '\0';
  return str;
}

char *add_str_end(char *str) {
  if (strcmp(str, "") == 0) return str;
  str[strlen(str)] = '\0';
  return str;
}

char *format_print_null_int(int value) {
  char *buffer = malloc(sizeof(int));
  if (value == -1) return NULL_MESSAGE;
  sprintf(buffer, "%d", value);
  return buffer;
}

size_t calculate_maybe_null_size(char str[]) {
  if (strcmp(str, "NULO") == 0) return 0;
  return strlen(str);
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

char *invert_remove(char removed) {
  if (removed == '0') return "1";
  return "0";
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

void scan_quote_string(char *str) {

	/*
	*	Use essa função para ler um campo string delimitado entre aspas (").
	*	Chame ela na hora que for ler tal campo. Por exemplo:
	*
	*	A entrada está da seguinte forma:
	*		nomeDoCampo "MARIA DA SILVA"
	*
	*	Para ler isso para as strings já alocadas str1 e str2 do seu programa, você faz:
	*		scanf("%s", str1); // Vai salvar nomeDoCampo em str1
	*		scan_quote_string(str2); // Vai salvar MARIA DA SILVA em str2 (sem as aspas)
	*
	*/

	char R;

	while((R = getchar()) != EOF && isspace(R)); // ignorar espaços, \r, \n...

	if(R == 'N' || R == 'n') { // campo NULO
		getchar(); getchar(); getchar(); // ignorar o "ULO" de NULO.
		strcpy(str, ""); // copia string vazia
	} else if(R == '\"') {
		if(scanf("%[^\"]", str) != 1) { // ler até o fechamento das aspas
			strcpy(str, "");
		}
		getchar(); // ignorar aspas fechando
	} else if(R != EOF){ // vc tá tentando ler uma string que não tá entre aspas! Fazer leitura normal %s então, pois deve ser algum inteiro ou algo assim...
		str[0] = R;
		scanf("%s", &str[1]);
	} else { // EOF
		strcpy(str, "");
	}
}