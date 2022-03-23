#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define RADIUS 1

// arg1 - number of points
int main(int argc,char **argv){ 
    
    if(argc<2){ 
        printf("Wrong given number of arguments. Give me number of points.");
        return 1;
    }

    long long points_no = strtol(argv[1], NULL, 10); 

    long long inside_wheel = 0;

    srand(time(NULL)); 
    for(long long i=0; i<points_no; i++){
        double x = (double) rand() / RAND_MAX;
        double y = (double) rand() / RAND_MAX;

        if(x*x + y*y <= RADIUS ){
            inside_wheel +=1;
        }
    }

    printf("%lld %lld\n", points_no, inside_wheel);
    double pi = (( double) inside_wheel / points_no )*4.0;
    printf("Calculated pi is: %.20f", pi);

}