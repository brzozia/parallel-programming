#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

typedef struct node{
    int data;
    struct node * next;
};

typedef struct elements_count{
    int count;
    omp_lock_t mut;
};


void print_bucket(struct node * bucket){   
    while(bucket != NULL){
        printf("%d \n", bucket->data);
        bucket = bucket->next;
    }
    printf("\n");
}

void generate_numbers(int *size, int *my_pid, int *array, int *max){
    unsigned int seed = *my_pid+(unsigned int)time(NULL);
    int i;
    //Random numbers generating
    #pragma omp for schedule(static)
    for (i = 0; i<(*size); i++){
        array[i] = rand_r(&seed)%(*max);
    }
}


void separate_numbers(int *my_start_range_id, int *my_end_range_id, int *array, int *range, int *min, int *buckets, struct node **threads_buckets, int *my_pid, char *error, int *size){
    int i;
    #pragma omp for schedule(static)
    for (i = 0; i<(*size); i++){
    // for(i=my_start_range_id; i<=my_end_range_id; i++){
        int bucket_id = (array[i]-(*min))/(*range);
        if(bucket_id >= (*buckets)){
            bucket_id--;
        }
        
        struct node *head = malloc(sizeof(struct node));
            if(head==NULL){
                *error = 'e';
                exit(1);
            }
        head->data = array[i];
        head->next = threads_buckets[(*my_pid)][bucket_id].next;
        threads_buckets[(*my_pid)][bucket_id].next = head;
        
        // print_bucket(&threads_buckets[my_pid][bucket_id]);
    }
}

void merge_elements(int *threads, int *my_bucket_start_id, int *my_bucket_end_id, int *my_pid, struct node ** threads_buckets, int *my_elements){
    int i,j;
    for(i=(*my_bucket_start_id); i<=(*my_bucket_end_id); i++){
        struct node * ptr = &threads_buckets[(*my_pid)][i];
        while(ptr->next != NULL){
            (*my_elements)++;
            ptr = ptr->next;
        }
        for(j=0;j<(*threads);j++){
            if(j != (*my_pid)){
                ptr -> next = threads_buckets[j][i].next;
                while(ptr->next != NULL){
                    (*my_elements)++;
                    ptr = ptr->next;
                }
            }
        }
        
    }
    // printf("thread: %d, my start bucket id: %d, end id %d, my elements: %d\n", *my_pid, *my_bucket_start_id, *my_bucket_end_id, *my_elements);
}

void sort_buckets(int *my_pid, int *my_bucket_start_id, int *my_bucket_end_id, struct node ** threads_buckets){
    int i;
    for(i=(*my_bucket_start_id); i<=(*my_bucket_end_id); i++){
        // printf("sort test\n");
        // print_bucket(threads_buckets[my_pid][i].next);
        struct node *ptr = threads_buckets[(*my_pid)][i].next;
        
        while(ptr != NULL && ptr->next != NULL){

            if(ptr->data > ptr->next->data){
                struct node *next_move = threads_buckets[(*my_pid)][i].next;
                struct node *prev_move = &threads_buckets[(*my_pid)][i];

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
        // print_bucket(threads_buckets[my_pid][i].next);
    }
}

void concat_numbers(int *my_pid, struct elements_count * elements, int *my_elements, int *my_bucket_start_id, int *my_bucket_end_id, int *array, struct node ** threads_buckets){
    int i, j = 0;
    if((*my_pid)==0){
        elements[0].count = (*my_elements);
        omp_unset_lock(&elements[0].mut);
        j=0;
    }
    else{
        omp_set_lock(&elements[(*my_pid)-1].mut);
        j = elements[(*my_pid)-1].count;
        omp_unset_lock(&elements[(*my_pid)-1].mut);

        // update my elements
        elements[(*my_pid)].count = (*my_elements)+j;
        omp_unset_lock(&elements[(*my_pid)].mut);
    }

    for(i=(*my_bucket_start_id); i<=(*my_bucket_end_id); i++){
        struct node *ptr = threads_buckets[(*my_pid)][i].next;
        while(ptr != NULL){
            array[j] = ptr->data;
            j++;
            ptr = ptr->next;
        }
    }
}


// args: SIZE, MAX_VALUE, PREFFERED_ELEMENTS_IN_BUCKET, NUMBER_OF_THREADS
int main(int argc, char** argv){
    double time_all, time_allocate_sync, time_c, time_deallocate, time_tmp;
    char error = 'o';
    time_all = omp_get_wtime();
    
    int threads=1, size = 1100000000;
    int max = RAND_MAX;
    int mean_elements_in_bucket = 10;
    // Parsing arguments
    if(argc>=2){
        size = atoi(argv[1]);
    }
    if(argc>=3 && strcmp(argv[2], "0")){
        max = atoi(argv[2]);
    }
    if(argc>=4){
        mean_elements_in_bucket = atoi(argv[3]);
    }
    if(argc>=5){
        threads = atoi(argv[4]);
    }

    int min=0, i,j;
    int* array = malloc(size * sizeof(int));
    int buckets = ceil((float)size/mean_elements_in_bucket);
    int range = ceil((float)(max-min)/buckets);

    // printf("\nRange of numbers in array %d:%d \nElements to sort: %d \n", min,max,size);
    // printf("Number of buckets: %d \nWhole range of numbers is divided into parts of about: %d elements\n", buckets,range);


    // Array containing all buckets
    time_allocate_sync = omp_get_wtime();
    struct node ** threads_buckets = malloc(threads * sizeof(struct node*));
    if(threads_buckets == NULL){
        printf("Error while allocating memory for buckets array");
        error = 'e';
        exit(1);
    }

    struct elements_count *elements = malloc(threads*sizeof(struct elements_count));
    if(elements == NULL){
        printf("Error while allocating memory for elements array");
        error = 'e';
        exit(1);
    }

    // double **times = malloc(threads * sizeof(double*)); //time_allocate, time_generate, time_separate, time_buckets_merge, time_sep_merge, time_sort, time_concat;
    double *times = malloc(threads * sizeof(double));
    int times_no = 7;
    time_allocate_sync = omp_get_wtime() - time_allocate_sync;

    #pragma omp parallel shared(error, size, min, max, array, range, threads, threads_buckets, buckets, elements, times_no, times) private(i,j,time_tmp, time_c)
    {
        int my_elements, my_pid, my_start_range_id, my_end_range_id, my_bucket_start_id, my_bucket_end_id;
        my_pid = omp_get_thread_num();
        elements[my_pid].count = -1; // init locks
        omp_init_lock(&elements[my_pid].mut);
        omp_set_lock(&elements[my_pid].mut);

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


        // times[my_pid] = malloc(times_no*sizeof(double)); 
        // Each thread allocates buckets for itself
        #pragma omp barrier
        time_c = omp_get_wtime();
        threads_buckets[my_pid] = malloc(buckets*sizeof(struct node));
        if(threads_buckets[my_pid] == NULL){
            printf("Error while allocating memory for buckets array");
            error = 'e';
            exit(1);
        }
        for(i=0; i<buckets; i++){
            //guardian
            threads_buckets[my_pid][i].data = -1;
            threads_buckets[my_pid][i].next = NULL;
        }
        #pragma omp barrier
        time_c = omp_get_wtime() - time_c;
        // times[my_pid][0] = time_c;
        if(my_pid==0){
            times[0] = time_c;
        }

        // Random numbers generating
        #pragma omp barrier // barrier before counting time
        time_c = omp_get_wtime(); // next barrier inside function (before and after for pragma)
        generate_numbers(&size, &my_pid, array, &max);
        #pragma omp barrier // just in case of anything
        time_c = omp_get_wtime() - time_c;
        // times[my_pid][1] = time_c;
        if(my_pid==0){
            times[1] = time_c;
        }
    

        // // Find minimum and maximum number in generated array
        // int min = max;
        // max = 0;
        // for (i = 0; i<size; i++){
        //     if(array[i]<min){
        //         min = array[i];
        //     }
        //     if(array[i]>max){
        //         max = array[i];
        //     }
        //     printf("%d ",array[i]);
        // }    
    

        // Put generated numbers in buckets  
        #pragma omp barrier 
        time_c = omp_get_wtime();
        separate_numbers( &my_start_range_id, &my_end_range_id, array, &range, &min, &buckets, threads_buckets, &my_pid, &error, &size); // barrier inside again
        #pragma omp barrier
        // times[my_pid][2] = time_c;
        if(my_pid==0){
            times[2] = omp_get_wtime() - time_c;
        }
        

        // Merge thread's buckets to sort
        #pragma omp barrier
        time_tmp = omp_get_wtime();
        my_elements = 0;
        merge_elements(&threads, &my_bucket_start_id, &my_bucket_end_id, &my_pid, threads_buckets, &my_elements);
        #pragma omp barrier
        // times[my_pid][3] = time_c;
        if(my_pid==0){
            times[3] = omp_get_wtime() - time_tmp;
            times[4] = omp_get_wtime() - time_c;
        }


        // Sort buckets - in-place instertion sort
        #pragma omp barrier
        time_c = omp_get_wtime();
        sort_buckets(&my_pid, &my_bucket_start_id, &my_bucket_end_id, threads_buckets);
        #pragma omp barrier
        time_c = omp_get_wtime() - time_c;
        // times[my_pid][4] = time_c;
        if(my_pid==0){
            times[5] = time_c;
        }

        // Write numbers to array
        #pragma omp barrier
        time_c = omp_get_wtime();
        concat_numbers(&my_pid, elements, &my_elements, &my_bucket_start_id, &my_bucket_end_id, array, threads_buckets);
        #pragma omp barrier
        time_c = omp_get_wtime() - time_c;
        // times[my_pid][5] = time_c;
        if(my_pid==0){
            times[6] = time_c;
        }
    }
    
    // printf("Time generate: %f\n Time separate: %f\n Time sort: %f\n Time concat buckets: %f\n Time all: %f", time_generate, time_separate, time_sort, time_concat, time_all);
   

    // Free memory
    time_deallocate = omp_get_wtime();
    for(j=0;j<threads;j++){
        free(threads_buckets[j]);
    }
    free(threads_buckets);
    free(elements);
    time_deallocate = omp_get_wtime() - time_deallocate;
    time_all = omp_get_wtime()-time_all;

    for(i=1;i<size;i++){
        if(array[i-1] > array[i]){
            error = 's';
        }
        printf("%d ", array[i]);
    }
   free(array);

    if(argc>=6){
        FILE *fd;
        fd = fopen(argv[5], "a" );
        fprintf(fd,"%d;%d;%d;%d;%d;%lf;%lf;%lf;%c;\n", min, max, size, buckets, threads, time_allocate_sync,  time_deallocate, time_all, error);
        // for(i=0;i<threads;i++){
        for(j=0;j<times_no;j++){
            fprintf(fd,"%lf;", times[j]);
            // fprintf(fd,"%lf;", times[i][j]); ////time_allocate, time_generate, time_separate, time_buckets_merge, time_sort, time_concat;
        }
        fprintf(fd,"\n");
        // }
        fclose(fd);
    }
    printf("\nTime all: %lf\n",  time_all);

    return 0;
}
