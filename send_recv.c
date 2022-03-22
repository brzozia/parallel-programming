#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>


double proces_0(long long loops, int msg_size, char *msg){
  double starttime, endtime;
  
  MPI_Barrier(MPI_COMM_WORLD);
  starttime = MPI_Wtime();
  while(loops > 0){
    MPI_Send(msg, msg_size, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
    MPI_Recv(msg, msg_size, MPI_CHAR, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    // printf("Process 0 received number %s from process 1. Size: %ld\n", msg, strlen(msg)+1); // +1 for '\0' sign

    loops--;
  }
  endtime = MPI_Wtime();
  return endtime - starttime;
  
}
double proces_0B(long long loops, int msg_size, char *msg){
  double starttime, endtime;
  MPI_Buffer_attach( b, n*sizeof(double) + MPI_BSEND_OVERHEAD );
  MPI_Barrier(MPI_COMM_WORLD);
  starttime = MPI_Wtime();
  while(loops > 0){
    MPI_Bsend(msg, msg_size, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
    MPI_Recv(msg, msg_size, MPI_CHAR, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    // printf("Process 0 received number %s from process 1. Size: %ld\n", msg, strlen(msg)+1); // +1 for '\0' sign

    loops--;
  }
  endtime = MPI_Wtime();
  return endtime - starttime;
  
}

void proces_1(long long loops, int msg_size, char *msg){

  MPI_Barrier(MPI_COMM_WORLD);
  while(loops > 0){
    MPI_Recv(msg, msg_size, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    // printf("Process 1 received number %s from process 0 Size: %ld\n", msg, strlen(msg)+1);// +1 for '\0' sign
    MPI_Send(msg, msg_size, MPI_CHAR, 0, 0, MPI_COMM_WORLD);

    loops--;
  }
}

void proces_1B(long long loops, int msg_size, char *msg){

  MPI_Barrier(MPI_COMM_WORLD);
  while(loops > 0){
    MPI_Recv(msg, msg_size, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    // printf("Process 1 received number %s from process 0 Size: %ld\n", msg, strlen(msg)+1);// +1 for '\0' sign
    MPI_Bsend(msg, msg_size, MPI_CHAR, 0, 0, MPI_COMM_WORLD);

    loops--;
  }
}

// arg[1] - times of ping-pong
// arg[2] - size of message in bytes
// arg[3] - results filename
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
  char *msg = (char*)calloc(msg_size,sizeof(char));
  
  // printf("msg: %s size: %ld, %d\n", msg, strlen(msg)+1, msg_size);


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


  double time;
  if (world_rank == 0) {
    int i;
    for(i=0;i<msg_size-1;i++){
        msg[i] = 'a';
    }
    msg[msg_size-1] = '\0';

    time = proces_0(loops, msg_size, msg);
    // printf("time: %f", time);

    if(argc>2){
      FILE *fd;
      fd = fopen(argv[3], "a" );
      // lseek(fd, SEEK_END);

      fprintf(fd,"%lld; %d; %f;\n", loops, msg_size, time);
      fclose(fd);
    }

  } else if (world_rank == 1) {
    proces_1(loops, msg_size, msg);
  }

  free(msg);
  MPI_Finalize();

  return 0;
}
