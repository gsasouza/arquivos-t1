/*
 * Gabriel Santos Souza nUSP: 11208176
 * Yann Amado Nunes Costa nUSP: 10746943
 */

#include "helpers.h"
#include "ctype.h"

/*
 * Add dynamic fields padding (@)
 */
char *add_empty_padding(char *str, int size) {
  int start = (int) strlen(str);
  str[start] = '\0';
  for (int i = start + 1; i < size; i++) {
    str[i] = '@';
  }
  return str;
}

/*
 * Open a file and handle error
 */
FILE *open_file(char filename[], char mode[]) {
  FILE *file = fopen(filename, mode);
  if (!file) {
    printf(ERROR_MESSAGE);
    exit(0);
  }
  return file;
}

/*
 * Format field with "NULO" or "" as value
 */
char *format_print_null(char value[]) {
  if ((strcmp(value, "NULO") == 0) || (strcmp(value, "") == 0)) return NULL_MESSAGE;
  return value;
}

/*
 * Format int fields that can maybe have "NULO" value
 */
int format_csv_maybe_empty_int(char *str) {
  if (strcmp(str, "NULO") == 0) return -1;
  return (int) strtod(str, NULL);
}

/*
 * Format strings that can maybe have "NULO" value
 */
char *format_csv_maybe_empty_str(char *str) {
  if (strcmp(str, "NULO") == 0) return "";
  return str;
}

/*
 * Format strings that can maybe have a removed entry (starts with "*")
 */
char format_csv_maybe_removed_str(char *str) {
  if (str[0] == '*') {
    memmove(str, str + 1, 5);
    str[5] = '\0';
    return '1';
  }
  return '0';
}

/*
 * Add ending to csv last field
 */
char *format_csv_last_field(char *str) {
  str[strlen(str) - 1] = '\0';
  return str;
}

/*
 * Add ending to a string
 */
char *add_str_end(char *str) {
  if (strcmp(str, "") == 0) return str;
  str[strlen(str)] = '\0';
  return str;
}

/*
 * Format int field with "NULO" value for printing
 */
char *format_print_null_int(int value) {
  char *buffer = malloc(sizeof(int));
  if (value == -1) return NULL_MESSAGE;
  sprintf(buffer, "%d", value);
  return buffer;
}

/*
 * Calculate size of fields that can be "NULO"
 */
size_t calculate_maybe_null_size(char str[]) {
  if (strcmp(str, "NULO") == 0 || strcmp(str, "") == 0) return 0;
  return strlen(str);
}

/*
 * Format date for priting
 */
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

/*
 * Invert remove value, because is counter intuitive to use "0" as true and "1" as false
 */
char *invert_remove(char removed) {
  if (removed == '0') return "1";
  return "0";
}

char format_status_bool(bool status) {
  if (status == false) return '0';
  return '1';
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

/*
    Converte o prefixo do veículo para int

    OBS1:   retorna -1 se o primeiro caracter é '*'

    OBS2:   retorna LIXO se a string contém algo diferente de números e letras
            maiúsculas (com excessão do caso da OBS1)

    COMO FUNCIONA:

        Para converter os prefixos para int e garantir que prefixos diferentes
        deem números diferentes interpretamos eles como números em base 36

        Um número N com 5 digitos dI em base 36 é escrito como:

            N = d0 * 36^0 + d1 * 36^1 + d2 * 36^2 + d3 * 36^3 + d4 * 36^4

        Nota-se que estamos escrevendo do digito menos significativo para o
        mais significativo

        Como o nosso prefixo têm 5 bytes e eles podem assumir 36 valores
        cada, podemos interpretar cada char dele como um digito em base 36,
        prefixos diferentes formaram números diferentes em base 36 e portanto
        números diferentes quando convertemos para um int normal
*/
int convertePrefixo(char* str) {

  /* O registro que tem essa string como chave foi removido */
  if(str[0] == '*')
    return -1;

  /* Começamos com o primeiro digito na ordem de 36^0 = 1 */
  int power = 1;

  /* Faz a conversão char por char para chegar ao resultado */
  int result = 0;
  for(int i = 0; i < 5; i++) {

    /*
        Interpreta o char atual como se fosse um digito
        em base 36. Os digitos da base 36 são:
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, A, B, C, D,
        E, F, G, H, I, J, K, L, M, N, O, P, Q, R,
        S, T, U, V, W, X, Y, Z
    */
    int cur_digit;
    /* Checa pelos digitos normais e os converte para números */
    if(str[i] >= '0' && str[i] <= '9')
      cur_digit = str[i] - '0';
      /* Checa pelas letras e as converte para números */
    else if(str[i] >= 'A' && str[i] <= 'Z')
      cur_digit = 10 + str[i] - 'A';

    /*
        Multiplica o digito atual pelo ordem da posição atual
        e adiciona no resultado
        Primeira posição:   36^0 = 1
        Segunda posição:    36^1 = 36
        Terceira posição:   36^2 = 1.296
        Quarta posição:     36^3 = 46.656
        Quinta posição:     36^4 = 1.679.616
    */
    result += cur_digit * power;

    /* Aumenta a ordem atual */
    power *= 36;

  }

  return result;

}