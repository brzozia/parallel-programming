#include <stdio.h>
#include <stdlib.h>


// arg[1] - times of ping-pong
int main(int argc, char** argv) {
  int msg_size = 8;
  char msg[msg_size];
//   char *msg = (char*)calloc(msg_size,sizeof(char));
// msg = (char*)calloc(msg_size,sizeof(char));
  
//   for(int i=0;i<msg_size;i++){
//       msg[i] = 'a';
//       printf("%c\n", msg[i]);
//   }
  
  msg[msg_size-1] = '\0';
  printf("%c\n", msg[7]);

  printf("msg %s msg size %d %d %d %c", msg, sizeof(msg), msg_size, sizeof(""), msg[7]);
//   int * loops = &number;

//   while(*loops > 0){

//     *loops -= 1;
//     printf("loops %d\n", *loops);
//   }
}