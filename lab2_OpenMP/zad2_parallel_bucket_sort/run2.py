import subprocess

size = 4167000
bucket_size = 20


result_file = "./results_runG20tes.txt"
f = open(result_file, "a")
f.write("min;max;size;buckets;mean_elements_in_bucket;threads;time_allocate_sync;time_deallocate;time_all;error;0;0;0;0;0;0;0;0;\n")
f.close()


compile_c = "gcc -Wall -g ./parallel_bucket_sort.c -o bucket -fopenmp -lm"

proces = subprocess.Popen(compile_c.split(" "))
proces.wait()


for x in range(0,5): 
    for proc in range(1,13):
        proces = subprocess.Popen(["bash", "-c", "export OMP_NUM_THREADS="+str(proc)+"; ./bucket " + str(size*proc) + " 0 "+ str(bucket_size)+ " " + str(proc) + " " + result_file])
        proces.wait()
        print(proces.stdout)
