#include <stdio.h>
#include <stdlib.h>


typedef struct node{
    int data;
    struct node * next;
};


int main(int argc, char **argv){
    long long int size = 1000000000;
    if(argc>=2){
        size = atoll(argv[1]);
    }

    int* array  = malloc(size * sizeof(int));
    struct node* buckets_array = malloc(size*sizeof(struct node));

    if(array==NULL || buckets_array==NULL){
        FILE *fd;
        fd = fopen("test_mem.txt", "a" );
        fprintf(fd,"nok %llu;\n", size);
        fclose(fd);
    }

    return 0;
}