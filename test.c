#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void proces_0(int msg_size, char *msg){


    printf("Process 0 received number %s from process 1. Size: %ld\n", msg, strlen(msg)+1);

  
}

// arg[1] - times of ping-pong
int main(int argc, char** argv) {
  int msg_size = 6;
   char *msg=(char*)calloc(msg_size,sizeof(char));
//   char *msg = (char*)calloc(msg_size,sizeof(char));
// msg = (char*)calloc(msg_size,sizeof(char));
  
  for(int i=0;i<msg_size;i++){
      msg[i] = 'a';
      printf("%c\n", msg[i]);
  }
  
  msg[msg_size-1] = '\0';
  printf("%c\n", msg[7]);

  printf("msg %s msg size %d %d %d %c", msg, sizeof(*msg), msg_size, sizeof(""), msg[7]);

  proces_0(msg_size, msg);
  free(msg);

//   int * loops = &number;

//   while(*loops > 0){

//     *loops -= 1;
//     printf("loops %d\n", *loops);
//   }
}