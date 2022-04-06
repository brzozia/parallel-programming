import subprocess

sizes = [x for x in range(1000,10000, 1000)] + [x for x in range(10000,100000, 10000)] + [x for x in range(100000,1000000, 100000)] + [x for x in range(1000000,10000000, 1000000)]+ [x for x in range(10000000,100000000, 10000000)]+ [x for x in range(100000000, 243138329, 100000000)] + [243138329]

result_file = "./results_static.txt"
f = open(result_file, "a")
f.write("threads; size; time;\n0; 0; 0;\n")
f.close()


compile_c = "gcc -Wall ./rly_random_num_for.c -o rand -fopenmp"
run_c = ["env OMP_NUM_THREADS="+str(x)+" ./rand" for x in range(1,5)]

proces = subprocess.Popen(compile_c.split(" "))
proces.wait()
for size in sizes:
    for x in range(1,5):
        proces = subprocess.Popen(("env OMP_NUM_THREADS=" +str(x)+" ./rand" + " " + str(size) + " " + str(result_file)).split(" "))
        proces.wait()
        print(proces.stdout)
