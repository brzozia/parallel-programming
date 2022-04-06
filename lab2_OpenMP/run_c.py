import subprocess

sizes = [x for x in range(1000,10000, 3000)] + [x for x in range(10000,100000, 30000)] + [x for x in range(100000,1000000, 100000)] + [x for x in range(1000000,10000000, 1000000)]+ [x for x in range(10000000,100000000, 10000000)]+ [x for x in range(100000000, 243138329, 100000000)] + [243138329]

result_file = "./results_static.txt"
f = open(result_file, "a")
f.write("threads; size; time;\n0; 0; 0;\n")
f.close()


compile_c = "gcc -Wall ./rly_random_num_for.c -o rand -fopenmp"
schedule = ['static', 'static,4', 'dynamic', 'dynamic,4', 'dynamic,50', 'guided,4', 'guided']
files = ['./results_'+str(x.replace(',', '_'))+".txt" for x in schedule]
# run_c = ["export OMP_NUM_THREADS="+str(x)+ " ./rand" for x in range(1,5)]


proces = subprocess.Popen(compile_c.split(" "))
proces.wait()
for idx,sched in enumerate(schedule):
    print(files[idx])
    f = open(files[idx], "a")
    f.write("threads; size; time;\n0; 0; 0;\n")
    f.close()
    for size in sizes:
        for x in range(1,5):
            # print("export OMP_SCHEDULE=\'" + sched +"\'; export OMP_NUM_THREADS=" +str(x)+"; ./rand" + " " + str(size) + " " + files[idx])
            proces = subprocess.Popen(["bash", "-c", "export OMP_SCHEDULE=\'" + sched +"\';  export OMP_NUM_THREADS=" +str(x)+"; ./rand" + " " + str(size) + " " + files[idx]])
            proces.wait()
            print(proces.stdout)
