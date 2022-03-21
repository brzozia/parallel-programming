#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>


void proces_0(long long *loops, int msg_size, char* msg){
  double starttime, endtime;
  starttime = MPI_Wtime();

  while(*loops > 0){
    MPI_Send(&msg, msg_size, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
    MPI_Recv(&msg, msg_size, MPI_CHAR, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    printf("Process 0 received number %s from process 1\n", msg);

    *loops--;
  }

  endtime   = MPI_Wtime();
  
}

void proces_1(long long *loops, int msg_size, char* msg){

  while(*loops > 0){
    MPI_Recv(&msg, msg_size, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    printf("Process 1 received number %s from process 0\n", msg);
    MPI_Send(&msg, msg_size, MPI_CHAR, 0, 0, MPI_COMM_WORLD);

    *loops--;
  }
}

// arg[1] - times of ping-pong
// arg[2] - size of message in bytes
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

  int msg_size = 1;

  if(argc >= 2){
    msg_size = strtoll(argv[2], NULL, 10);
    
    if(msg_size<1){
        msg_size = 1;
    }
  }

  char msg[msg_size];
  msg[msg_size-1] = '\0';

  printf("msg: %s size:", msg, sizeof(msg));


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


  if (world_rank == 0) {
    proces_0(&loops, msg_size, &msg);

  } else if (world_rank == 1) {
    proces_1(&loops, msg_size, &msg);
  }
  MPI_Finalize();

  return 0;
}
