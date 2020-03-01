#include <iostream>
#include <omp.h>
#include <stdlib.h>
#include <string.h>
const int MAX_ARCHIVO = 50;
const int MAX_CHAR = 100;
struct lista_nodos {
    char* data;
    struct lista_nodos* next;
};

void Usage(char* prog_name);
void Productor_Consumidor(int productor_contar, int cons_contador, FILE* archivos[], int contador_archivo);
void Obtener_Archivos(FILE* archivos[], int* archivo_contadorp);
void Leer_Archivo(FILE* file, struct lista_nodos** queue_head, struct lista_nodos** queue_tail, int mi_rango);
void enq(char* line, struct lista_nodos** queue_head, struct lista_nodos** queue_tail);
struct lista_nodos* Dequeue(struct lista_nodos** queue_head, struct lista_nodos** queue_tail, int mi_rango);
void tokenizacion(char* data, int mi_rango);
void pintar_queue(int mi_rango, struct lista_nodos* queue_head);

int main(int argc, char* argv[]) {
    int   productor_contar, cons_contador;
    FILE* archivos[MAX_ARCHIVO];
    int   contador_archivo;

    if (argc != 3) Usage(argv[0]);
    productor_contar = strtol(argv[1], NULL, 10);
    cons_contador = strtol(argv[2], NULL, 10);
    Obtener_Archivos(archivos, &contador_archivo);
    Productor_Consumidor(productor_contar, cons_contador, archivos, contador_archivo);
    return 0;

}



void Usage(char* prog_name) {
    fprintf(stderr, "usage: %s <producer count> <consumer count>\n",prog_name);
    exit(0);
}

void Obtener_Archivos(FILE* archivos[], int* archivo_contadorp) {
    int i = 0;
    char nombre_archivo[MAX_CHAR];
    while (scanf("%s", nombre_archivo) != -1) {
        archivos[i] = fopen(nombre_archivo, "r");
        if (archivos[i] == NULL) {
            fprintf(stderr, "No se puede abrir\n", nombre_archivo);
            exit(-1);
        }
        i++;
    }
    *archivo_contadorp = i;
}

void Productor_Consumidor(int productor_contar, int cons_contador, FILE* archivos[], int contador_archivo) {
    int thread_count = productor_contar + cons_contador;
    struct lista_nodos* queue_head = NULL;
    struct lista_nodos* queue_tail = NULL;
    int prod_done_count = 0;

}

void Leer_Archivo(FILE* file, struct lista_nodos** queue_head, struct lista_nodos** queue_tail, int mi_rango) {
    char* line = (char*)malloc(MAX_CHAR*sizeof(char));
    while (fgets(line, MAX_CHAR, file) != NULL) {
        printf("Th %d > read line: %s", mi_rango, line);
        enq(line, queue_head, queue_tail);
        line = (char*)malloc(MAX_CHAR*sizeof(char));
    }
    fclose(file);
}

void enq(char* line, struct lista_nodos** queue_head, struct lista_nodos** queue_tail) {
    struct lista_nodos* tmp_nodo = NULL;
    tmp_nodo = (lista_nodos*)malloc(sizeof(struct lista_nodos));
    tmp_nodo->data = line;
    tmp_nodo->next = NULL;
    #  pragma omp critical
    if (*queue_tail == NULL) { 
        *queue_head = tmp_nodo;
        *queue_tail = tmp_nodo;
    } else {
        (*queue_tail)->next = tmp_nodo;
        *queue_tail = tmp_nodo;
    }
}

struct lista_nodos* Dequeue(struct lista_nodos** queue_head, struct lista_nodos** queue_tail, int mi_rango) {
    struct lista_nodos* tmp_nodo = NULL;
    if (*queue_head == NULL) 
        return NULL;
    #  pragma omp critical
    {
        if (*queue_head == *queue_tail) 
            *queue_tail = (*queue_tail)->next;
        tmp_nodo = *queue_head;
        *queue_head = (*queue_head)->next;
    }
    return tmp_nodo;
}

void tokenizacion(char* data, int mi_rango) {
    char* my_token, *word;
    my_token = strtok_r(data, " \t\n", &word);
    while (my_token != NULL) {
        printf("Th %d token: %s\n", mi_rango, my_token);
        my_token = strtok_r(NULL, " \t\n", &word);

    }

}

void pintar_queue(int mi_rango, struct lista_nodos* queue_head) {
    struct lista_nodos* curr_p = queue_head;
    printf("Th %d > queue = \n", mi_rango);
    #  pragma omp critical
    while (curr_p != NULL) {
        printf("%s", curr_p->data);
        curr_p = curr_p->next;
    }
    printf("\n");
}