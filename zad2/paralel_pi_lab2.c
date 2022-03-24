#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

#define RADIUS 1


long long calc_points(int my_points){
    long long inside_wheel = 0;

    long long i;
    for(i=0; i<my_points; i++){
        double x = (double) rand() / RAND_MAX;
        double y = (double) rand() / RAND_MAX;

        if(x*x + y*y <= RADIUS ){
            inside_wheel +=1;
        }
    }

    return inside_wheel;
}

// arg1 - number of points
// arg2 - filename
int main(int argc,char **argv){ 
    
    if(argc<2){ 
        printf("Wrong given number of arguments. Give me number of points.");
        return 1;
    }

    MPI_Init(NULL, NULL);
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank); /* get current process id */
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size); /* get number of processes */
    
    srand(time(0) +2*world_rank + 2); //różny srand dla każdego procesu !!!!!!!!!!!!!

    long long points_no = strtol(argv[1], NULL, 10); 
    long long my_points = points_no / world_size;
    if(world_rank==0){
        my_points += points_no - (my_points*world_size);
    }

    double starttime, paraltime, calctime, pi;
    long long inside_wheel_all, inside_wheel;
    MPI_Barrier(MPI_COMM_WORLD);
    
    starttime = MPI_Wtime(); // start
    inside_wheel = calc_points(my_points);
    paraltime = MPI_Wtime(); // cz rownolegla

    printf("%lld %lld\n", my_points, inside_wheel);
    MPI_Reduce( &inside_wheel, &inside_wheel_all, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD );

    
    if(world_rank==0){
        pi = ((double) inside_wheel_all / points_no )*4.0;

        calctime = MPI_Wtime(); // cz sekwencyjna
        
        
        printf("Calculated pi is: %f", pi);
        if(argc>2){
            FILE *fd;
            fd = fopen(argv[2], "a" );
            fprintf(fd,"%d;%lld;%lld;%lld;%lf;%lf;%lf;%lf;\n", world_size, points_no, my_points, inside_wheel, starttime, paraltime, calctime, pi);
            fclose(fd);
        }
    }
   
   return 0;
}
