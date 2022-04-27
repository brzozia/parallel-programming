import subprocess

size = 200

result_file = "./results_xd.txt"
f = open(result_file, "a")
f.write("min;max;size;buckets;mean_elements_in_bucket;threads;time_allocate_sync;time_deallocate;time_all;error;\n0;0;0;0;0;0;0;0;0;0;\n")
f.close()

compile_c = "gcc -Wall -g ./parallel_bucket_sort.c -o bucket -fopenmp -lm"


proces = subprocess.Popen(compile_c.split(" "))
proces.wait()

# for x in range(0,5): 
for bucket_size in range(1,100,3):
    proces = subprocess.Popen(["bash", "-c", "export OMP_NUM_THREADS=1; ./bucket " + str(size) + " 100000 "+ str(bucket_size)+ " 1 " + result_file])
    proces.wait()
    print(proces.stdout)
