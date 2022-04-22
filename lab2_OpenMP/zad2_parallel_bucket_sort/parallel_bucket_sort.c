#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

typedef struct node{
    int data;
    struct node * next;
};

typedef struct elements_count{
    int count;
    pthread_mutex_t mut;
    pthread_cond_t cond;
};


void print_bucket(struct node * bucket){   

    while(bucket != NULL){
        printf("%d ", bucket->data);
        bucket = bucket->next;
    }
    printf("\n");
}

void generate_numbers(){
    
}


// args: SIZE, MAX_VALUE, PREFFERED_ELEMENTS_IN_BUCKET, NUMBER_OF_THREADS
int main(int argc, char** argv){
    double time_generate, time_separate, time_sort, time_concat, time_all;
    time_all = omp_get_wtime();
    
    int threads=1, size = 10;
    int max = RAND_MAX;
    int mean_elements_in_bucket = 3;
    // Parsing arguments
    if(argc>=2){
        size = atoi(argv[1]);
    }
    if(argc>=3){
        max = atoi(argv[2]);
    }
    if(argc>=4){
        mean_elements_in_bucket = atoi(argv[3]);
    }
    if(argc>=5){
        threads = atoi(argv[4]);
    }
    int* array = malloc(size * sizeof(int));
    int i;
    unsigned int seed;

    // Random numbers generating
    time_generate = omp_get_wtime();
    #pragma omp parallel shared(size, max, array) private(i, seed)
    {
        seed = omp_get_thread_num()+(unsigned int)time(NULL);
        // threads = omp_get_num_threads();
        
        #pragma omp for schedule(runtime)
        for (i = 0; i<size; i++){
            array[i] = rand_r(&seed)%max;
        }
    }
    time_generate = omp_get_wtime() - time_generate;

    // Find minimum and maximum number in generated array
    int min = max;
    max = 0;
    for (i = 0; i<size; i++){
        if(array[i]<min){
            min = array[i];
        }
        if(array[i]>max){
            max = array[i];
        }
        printf("%d ",array[i]);
    }    

    
    int buckets = (size/mean_elements_in_bucket)+1;
    int range = (max-min)/buckets;

    printf("\n Range of numbers in array %d:%d \nElements to sort: %d \n", min,max,size);
    printf("Number of buckets: %d \nWhole range of numbers is divided into parts of about: %d elements\n", buckets,range);

    // Array containing all buckets
    struct node ** threads_buckets = malloc(threads * sizeof(struct node*));
    if(threads_buckets == NULL){
        printf("Error while allocating memory for buckets array");
        exit(1);
    }

    int j, my_pid, my_start_range_id, my_end_range_id, my_bucket_start_id, my_bucket_end_id;
    struct elements_count *elements = malloc(threads*sizeof(struct elements_count));
    if(elements == NULL){
        printf("Error while allocating memory for elements array");
        exit(1);
    }

    for(j=0; j<threads; j++){
        elements[j].count = -1;
        pthread_mutex_init(&elements[j].mut, NULL);
        pthread_cond_init(&elements[j].cond, NULL);
    }


    time_separate = omp_get_wtime();
    #pragma omp parallel shared(size, max, min, array, range, threads, threads_buckets, buckets, elements) private(i, j, my_pid, my_start_range_id, my_end_range_id, my_bucket_start_id, my_bucket_end_id)
    {   
        my_pid = omp_get_thread_num();
        my_start_range_id = my_pid * size / threads;
        my_bucket_start_id = my_pid * buckets/threads;
        if(my_pid == threads - 1){
            my_end_range_id = size - 1;
            my_bucket_end_id = buckets - 1;
        }
        else{
            my_end_range_id = ((my_pid+1) * size / threads) - 1;
            my_bucket_end_id = ((my_pid+1) * buckets / threads) - 1;
        }
        
        // Each thread allocates buckets for itself
        threads_buckets[my_pid] = malloc(buckets*sizeof(struct node));
        // struct node** buckets_array = threads_buckets[my_pid]
        if(threads_buckets[my_pid] == NULL){
            printf("Error while allocating memory for buckets array");
            exit(1);
        }

        for(i=0; i<buckets; i++){
            //guardian
            struct node head;
            threads_buckets[my_pid][i].data = -1;
            threads_buckets[my_pid][i].next = NULL;
        }

        // Put generated numbers in buckets  // can be parellel for also
        for(i=my_start_range_id; i<=my_end_range_id; i++){
            int bucket_id = array[i]/range;
            if(bucket_id >= buckets){
                bucket_id--;
            }
            
            struct node head;// = malloc(sizeof(struct node));
            // struct node *ptr;
            head.data = array[i];
            head.next = threads_buckets[my_pid][bucket_id].next;
            threads_buckets[my_pid][bucket_id].next = &head;
            // ptr = threads_buckets[my_pid][bucket_id];
            
            print_bucket(&threads_buckets[my_pid][bucket_id]);
        }
    }

    #pragma omp parallel shared(size, max, min, array, range, threads_buckets, buckets, threads, elements) private(i, j, my_pid, my_start_range_id, my_end_range_id, my_bucket_start_id, my_bucket_end_id)
    {

        my_pid = omp_get_thread_num();
        my_start_range_id = my_pid * size / threads;
        my_bucket_start_id = my_pid * buckets/threads;
        if(my_pid == threads - 1){
            my_end_range_id = size - 1;
            my_bucket_end_id = buckets - 1;
        }
        else{
            my_end_range_id = ((my_pid+1) * size / threads) - 1;
            my_bucket_end_id = ((my_pid+1) * buckets / threads) - 1;
        }
        // Merge thread's buckets to sort
        int my_elements = 0;
        for(i=my_bucket_start_id; i<=my_bucket_end_id; i++){
            struct node * ptr = &threads_buckets[my_pid][i];
            print_bucket(&threads_buckets[my_pid][i]);
            while(ptr->next != NULL){
                my_elements++;
                ptr = ptr->next;
            }
            for(j=0;j<threads;j++){
                if(j != my_pid){
                    ptr -> next = threads_buckets[j][i].next;
                    while(ptr->next != NULL){
                        my_elements++;
                        ptr = ptr->next;
                    }
                }
            }
            printf("thread: %d, my elements: %d\n", my_pid, my_elements);
        }


        // Sort buckets - in-place instertion sort
        // time_sort = omp_get_wtime();
        for(i=my_bucket_start_id; i<=my_bucket_end_id; i++){
            printf("sort test\n");
            print_bucket(threads_buckets[my_pid][i].next);
            struct node *ptr = threads_buckets[my_pid][i].next;
            
            while(ptr != NULL && ptr->next != NULL){

                if(ptr->data > ptr->next->data){
                    struct node *next_move = threads_buckets[my_pid][i].next;
                    struct node *prev_move = &threads_buckets[my_pid][i];

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
            print_bucket(threads_buckets[my_pid][i].next);
        }
        // time_sort = omp_get_wtime() - time_sort;


        // Write numbers to array
        // time_concat = omp_get_wtime();
        if(my_pid==0){ // last thread can fullfill from the end TODO
            pthread_mutex_lock(&elements[0].mut);
            elements[0].count = my_elements;
            pthread_cond_broadcast(&elements[0].cond);
            pthread_mutex_unlock(&elements[0].mut);
            j=0;
        }
        else{
            // get previous thread elements
            pthread_mutex_lock(&elements[my_pid-1].mut);
            while (elements[my_pid-1].count != -1) {
                pthread_cond_wait(&elements[my_pid-1].cond, &elements[my_pid-1].mut);
            }
            j = elements[my_pid-1].count;
            pthread_mutex_unlock(&elements[my_pid-1].mut);

            // update my elements
            pthread_mutex_lock(&elements[my_pid].mut);
            elements[my_pid].count = my_elements+j;
            pthread_cond_broadcast(&elements[my_pid].cond);
            pthread_mutex_unlock(&elements[my_pid].mut);
        }

        for(i=my_bucket_start_id; i<=my_bucket_end_id; i++){
            struct node *ptr = threads_buckets[my_pid][i].next;
            while(ptr != NULL){
                array[j] = ptr->data;
                j++;
                ptr = ptr->next;
            }
        }
        // time_concat = omp_get_wtime() - time_concat;

    }
    // time_separate = omp_get_wtime() - time_separate; 


    time_all = omp_get_wtime()-time_all;
    // Print results
    for(i=0;i<size;i++){
        printf("%d ", array[i]);
    }
    // printf("Time generate: %f\n Time separate: %f\n Time sort: %f\n Time concat buckets: %f\n Time all: %f", time_generate, time_separate, time_sort, time_concat, time_all);
    printf("Time generate: %f\n Time all: %f", time_generate, time_all);

    // Free memory
    for(j=0;j<threads;j++){
        free(threads_buckets[j]);
    }
     free(threads_buckets);
    free(array);

    return 0;
}
