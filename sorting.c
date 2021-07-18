/*
 * Gabriel Santos Souza nUSP: 11208176
 * Yann Amado Nunes Costa nUSP: 10746943
 */

#include "sorting.h"

void fundir_vehicle(vehicle_t* vehicles, int inicio, int fim, char ordering_field[]){
    int tam_v1, tam_v2; //diz o tamanho total de cada um dos vetores
    int pos_v1, pos_v2; //diz em que posicao cada um dos vetores comeca
    int i, j, k; //sao os contadores das posicoes dos vetores v1, v2, v3 e aux respectivamente
    vehicle_t* aux = NULL;
    aux = (vehicle_t*)malloc((fim - inicio + 1)*sizeof(vehicle_t));
    i = 0;
    j = 0;
    k = 0;
    tam_v1 = (fim - inicio + 1)/2;  //denota o numero total de livros nessa iteracao, +1 pra transformar de pos para numero
    tam_v2 = (fim - inicio + 1)/2;
    if((fim - inicio + 1)%2 == 1)
        tam_v1 = tam_v1 + 1;
    pos_v1 = inicio;
    pos_v2 = pos_v1 + tam_v1;
    while(i < tam_v1 && j < tam_v2){
        if(vehicles[pos_v2 + j].line_code < vehicles[pos_v1 + i].line_code){
            aux[k] = vehicles[pos_v2 + j];
            j++;
        }
        else{
            aux[k] = vehicles[pos_v1 + i];
            i++;
        }
        k++;
    }
    while(i < tam_v1){
        aux[k] = vehicles[pos_v1 + i];
        i++;
        k++;
    }
    while(j < tam_v2){
        aux[k] = vehicles[pos_v2 + j];
        j++;
        k++;
    }
    for(i = inicio, k = 0; k < fim - inicio + 1; i++, k++)
        vehicles[i] = aux[k];
    free(aux);
}

void mergesort_vehicle(vehicle_t* vehicles, int inicio, int fim, char ordering_field[]){
    int meio;
    //fim - inicio + 1 descobre o tamanho do vetor
    if(fim <= inicio){
        fundir_vehicle(vehicles, inicio, fim, ordering_field);
        return;
    }
    meio = inicio + (fim - inicio)/2;
    //1 via
    mergesort_vehicle(vehicles, inicio, meio, ordering_field);
    //2 via
    mergesort_vehicle(vehicles, meio + 1, fim, ordering_field);
    //junta as 2 vias
    fundir_vehicle(vehicles, inicio, fim, ordering_field);
}

void fundir_line(line_t* lines, int inicio, int fim, char ordering_field[]){
    int tam_v1, tam_v2; //diz o tamanho total de cada um dos vetores
    int pos_v1, pos_v2; //diz em que posicao cada um dos vetores comeca
    int i, j, k; //sao os contadores das posicoes dos vetores v1, v2, v3 e aux respectivamente
    line_t* aux = NULL;
    aux = (line_t*)malloc((fim - inicio + 1)*sizeof(line_t));
    i = 0;
    j = 0;
    k = 0;
    tam_v1 = (fim - inicio + 1)/2;  //denota o numero total de livros nessa iteracao, +1 pra transformar de pos para numero
    tam_v2 = (fim - inicio + 1)/2;
    if((fim - inicio + 1)%2 == 1)
        tam_v1 = tam_v1 + 1;
    pos_v1 = inicio;
    pos_v2 = pos_v1 + tam_v1;
    while(i < tam_v1 && j < tam_v2){
        if(lines[pos_v2 + j].line_code < lines[pos_v1 + i].line_code){
            aux[k] = lines[pos_v2 + j];
            j++;
        }
        else{
            aux[k] = lines[pos_v1 + i];
            i++;
        }
        k++;
    }
    while(i < tam_v1){
        aux[k] = lines[pos_v1 + i];
        i++;
        k++;
    }
    while(j < tam_v2){
        aux[k] = lines[pos_v2 + j];
        j++;
        k++;
    }
    for(i = inicio, k = 0; k < fim - inicio + 1; i++, k++)
        lines[i] = aux[k];
    free(aux);
}


void mergesort_line(line_t* lines, int inicio, int fim, char ordering_field[]){
    int meio;
    //fim - inicio + 1 descobre o tamanho do vetor
    if(fim <= inicio){
        fundir_line(lines, inicio, fim, ordering_field);
        return;
    }
    meio = inicio + (fim - inicio)/2;
    //1 via
    mergesort_line(lines, inicio, meio, ordering_field);
    //2 via
    mergesort_line(lines, meio + 1, fim, ordering_field);
    //junta as 2 vias
    fundir_line(lines, inicio, fim, ordering_field);
}