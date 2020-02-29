/* File:     producer_comsumer.c* Purpose:  Implement a producer-consumer program in which some of the threads are producersand others are consumers.  The producers read text from a collection of files, one per producer.They insert lines of text into a single shared queue.  The consumers take the lines of text andtokenize them -- i.e., identify strings of characters separated by whitespace from the rest of theline.  When a consumer  finds a token, it writes to stdout.*/
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>




#define MAX_FILES 128

void Usage(char* prog_name);
const int RMAX = 10000000;
void Prod_cons(int prod_count, int cons_count, FILE* files[],  int file_count) {
    int thread_count = prod_count + cons_count;
    struct list_node_s* queue_head = NULL;
    struct list_node_s* queue_tail = NULL;
    int prod_done_count = 0;
#  pragma omp parallel num_threads(thread_count) default(none) \
    shared(file_count, queue_head, queue_tail, files, prod_count, \
    cons_count, prod_done_count){ 
    int my_rank = omp_get_thread_num(), f;
    if (my_rank < prod_count) { 
               /* Producer code *//* A cyclic partition of the files among the producers */
        for (f = my_rank; f < file_count;f += prod_count)
         {
             Read_file(files[f], &queue_head, &queue_tail, my_rank);
             }
#   pragma omp atomic
    prod_done_count++;
    } else {
        struct list_node_s* tmp_node;
        while (prod_done_count < prod_count) {
            tmp_node = Dequeue(&queue_head, &queue_tail, my_rank);
            if (tmp_node != NULL) {
                Tokenize(tmp_node->data, my_rank);
                free(tmp_node);    
            }    }
        while (queue_head != NULL) {
            tmp_node = Dequeue(&queue_head, &queue_tail, my_rank);
            if (tmp_node != NULL) {
                Tokenize(tmp_node->data, my_rank);
                free(tmp_node);      }   }     }
                } /* pragma omp parallel */
    } 
void Read_file(FILE* file, struct list_node_s** queue_head,struct list_node_s** queue_tail, int my_rank) {
    char* line = malloc(MAX_CHAR*sizeof(char));
    while (fgets(line, MAX_CHAR, file) != NULL) {
        printf("Th %d > read line: %s", my_rank, line);
        Enqueue(line, queue_head, queue_tail);
        line = malloc(MAX_CHAR*sizeof(char));
    }
    fclose(file);
}  /* Read_file */
void Enqueue(char* line, struct list_node_s** queue_head,struct list_node_s** queue_tail) {
    struct list_node_s* tmp_node = NULL;
    tmp_node = malloc(sizeof(struct list_node_s));
    tmp_node->data = line;
    tmp_node->next = NULL;
#  pragma omp criticalif (*queue_tail == NULL) {
     // list is empty
     *queue_head = tmp_node;
     *queue_tail = tmp_node;
     } else {
         (*queue_tail)->next = tmp_node;
         *queue_tail = tmp_node;
         }
    }  /* Enqueue */

    struct list_node_s* Dequeue(struct list_node_s** queue_head,struct list_node_s** queue_tail, int my_rank) {
        struct list_node_s* tmp_node = NULL;
        if (*queue_head == NULL) // empty
            return NULL;
    #  pragma omp critical
    {
        if (*queue_head == *queue_tail) // last node
            *queue_tail = (*queue_tail)->next;
        tmp_node = *queue_head;
        *queue_head = (*queue_head)->next;
        }
        return tmp_node;
        }  /* Dequeue */

}  /* main */
int main(int argc, char* argv[]) {
    int   prod_count, cons_count;
    FILE* files[MAX_FILES];
    int   file_count;
    if (argc != 3) Usage(argv[0]);
    prod_count = strtol(argv[1], NULL, 10);
    cons_count = strtol(argv[2], NULL, 10);
    /* Read in list of filenames and open files */
    //Get_files(files, &file_count);
#  ifdef DEBUG
    printf("prod_count = %d, cons_count = %d, file_count = %d\n",prod_count, cons_count, file_count);
#  endif
    /* Producer-consumer */
    //Prod_cons(prod_count, cons_count, files, file_count);
    return 0;
}