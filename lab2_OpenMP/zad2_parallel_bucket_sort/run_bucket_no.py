import subprocess

size = 10000000

result_file = "./res_buck_1_7_max.txt"
f = open(result_file, "a")
f.write("min;max;size;buckets;mean_elements_in_bucket;threads;time_allocate_sync;time_deallocate;time_all;error;0;0;0;0;0;0;0;0;\n")
f.close()

compile_c = "gcc -Wall -g ./parallel_bucket_sort.c -o bucket -fopenmp -lm"


proces = subprocess.Popen(compile_c.split(" "))
proces.wait()

# for idx,sched in enumerate(schedule):
    # print(files[idx])
    # f = open(files[idx], "a")
    # f.write("threads; size; time;\n0; 0; 0;\n")
    # f.close()
    # for size in sizes:
for x in range(0,5): 
    for bucket_size in range(1,100,1):
        proces = subprocess.Popen(["bash", "-c", "export OMP_NUM_THREADS=1; ./bucket " + str(size) + " 32766 "+ str(bucket_size)+ " 1 " + result_file])
        proces.wait()
        print(proces.stdout)
