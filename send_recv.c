#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>


void proces_0(long long *loops){
  int number;

  while(&loops > 0){
    number = 0;
    MPI_Send(&number, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
    MPI_Recv(&number, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    printf("Process 0 received number %d from process 1\n", number);

    &loops -= 1;
  }
  
}

void proces_1(long long *loops){
  int number;

  while(&loops > 0){
    MPI_Recv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    printf("Process 1 received number %d from process 0\n", number);
    number = 1;
    MPI_Send(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);

    &loops -= 1;
  }
}

// arg[1] - times of ping-pong
int main(int argc, char** argv) {
  if(argc < 2){
    printf("You need to give an argument - how many times send-receive should be done.");
    return 1;
  }

  long long loops = strtoll(argv[1], NULL, 10);
  if(loops <= 0){
    printf("Send-receive argument has to be bigger than 0.");
    return 1;
  }

  MPI_Init(NULL, NULL);
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank); /* get current process id */
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size); /* get number of processes */

  // We are assuming at least 2 processes for this task
  if (world_size < 2) {
    fprintf(stderr, "World size must be greater than 1 for %s\n", argv[0]);
    MPI_Abort(MPI_COMM_WORLD, 1); 
  }


  // int number;
  if (world_rank == 0) {
    proces_0(&loops);
    // If we are rank 0, set the number to -1 and send it to process 1
    // number = 0;
    // MPI_Send(&number, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
    // MPI_Recv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    // printf("Process 0 received number %d from process 1\n", number);

  } else if (world_rank == 1) {
    proces_1(&loops);
    // MPI_Recv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    // printf("Process 1 received number %d from process 0\n", number);
    // number = 1;
    // MPI_Send(&number, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
  }
  MPI_Finalize();
}
