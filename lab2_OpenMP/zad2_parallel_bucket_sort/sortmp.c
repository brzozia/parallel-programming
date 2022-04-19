#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct node{
    int data;
    struct node * next;
};

void print_bucket(struct node * bucket){   

    while(bucket != NULL){
        printf("%d ", bucket->data);
        bucket = bucket->next;
    }
    printf("\n");
}

int main(int argc, char** argv){
    int size = atoi(argv[1]);
    int* array  = malloc(size * sizeof(int));
    int i;

    #pragma omp parallel shared(SIZE, array) private(i)
    {
        unsigned int seed = omp_get_thread_num()+(unsigned int)time(NULL);
        
        //Random numbers generating
        #pragma omp for schedule(runtime)
        for (i = 0; i<size; i++){
            array[i] = rand_r(&seed);
        }
    }

    for (i = 0; i<size; i++){
        printf("%d ",array[i]);
    }

    printf("\nMaksymalna wartosc w tablicy liczb%d \n", RAND_MAX);
    printf("Ilosc elementow do posortowania: %d \n", size);

    int mean_elements_in_bucket = 3;
    int buckets = size/3;
    printf("Ilosc kubelkow: %d \n", buckets);
    int range = RAND_MAX/buckets;
    printf("Wielkosc zakresu liczb w pojedynczym kubelku: %d n", range);

    // Array containing all buckets
    struct node** buckets_array = malloc(buckets*sizeof(struct node *));
    if(buckets_array == NULL){
        printf("Error while allocating memory for buckets array");
    }

    for(i=0; i<buckets; i++){
        buckets_array[i] = malloc(mean_elements_in_bucket*3*sizeof(struct node));
        if(buckets_array[i] == NULL){
            printf("Error while allocating memory for buckets array elements");
        }
        buckets_array[i] = NULL;
        //guardian
        struct node *head= malloc(sizeof(struct node));
        head->data = -1;
        head->next = buckets_array[i];
        buckets_array[i] = head;
    }

    // Put generated numbers in buckets
    for(i=0; i<size; i++){
        int bucket_id = array[i]/range;
        if(bucket_id >= buckets){
            bucket_id--;
        }
        
        struct node *head= malloc(sizeof(struct node));
        head->data = array[i];
        head->next = buckets_array[bucket_id]->next;
        buckets_array[bucket_id]->next = head;
        
        print_bucket(buckets_array[bucket_id]->next);
    }


    // Sort buckets - in-place instertion sort
   for(i=0; i<buckets; i++){
        printf("sort test\n");
        print_bucket(buckets_array[i]->next);
        struct node *ptr = buckets_array[i]->next;
        
        while(ptr != NULL && ptr->next != NULL){

            if(ptr->data > ptr->next->data){
                struct node *next_move = buckets_array[i]->next;
                struct node *prev_move = buckets_array[i];

                while(ptr->next->data > next_move->data){
                    next_move = next_move->next;
                    prev_move = prev_move->next;
                }

                struct node *move = ptr->next;
                ptr->next = move->next;
                prev_move->next = move;
                move->next = next_move;
            }
            else{
                ptr = ptr -> next;
            }
        }

        print_bucket(buckets_array[i]->next);
    }

    // Write numbers to array
    int j = 0;
    for(i=0; i<buckets; i++){
        struct node *bucket = buckets_array[i]->next;

        while(bucket != NULL){
            array[j++] = bucket->data;
            bucket = bucket->next;
        }
    }

    // Print results
    for(i=0;i<size;i++){
        printf("%d ", array[i]);
    }

    // Free memory
    for(i=0; i<buckets; i++){
        free(buckets_array[i]);
    }
    free(array);

    return 0;
}
