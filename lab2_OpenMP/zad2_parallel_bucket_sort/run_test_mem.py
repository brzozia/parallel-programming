import subprocess

sizes = [x for x in range(3000000000,7000000000 , 10000)]

compile_c = "gcc -Wall ./parralel_bucket_sort.c -o bucket -fopenmp -lm"


proces = subprocess.Popen(compile_c.split(" "))
proces.wait()

for size in sizes:
    proces = subprocess.Popen(["bash", "-c", "export OMP_NUM_THREADS=4; ./bucket" + " " + str(size) + " 0 10 1 check_paral_mem.txt"  + ])
    proces.wait()


